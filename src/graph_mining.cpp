#include "../include/graph.h"
#include "../include/graph_d.h"
#include "../include/embedding.h"
#include "../include/task_queue.h"
#include "../include/communicator.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <mpi.h>
#include <omp.h>
#include <unistd.h>
#include <stdio.h>

long long extend_result;
const int NUM_THREADS=3;

void computation(std::vector<Embedding> (*extend)(Embedding *e), Embedding *e, Task_Queue* task)
{
    printf("Try to computation\n");
    fflush(stdout);
    std::vector<Embedding> vec = (*extend)(e);
    for (int i = 0; i < (int)vec.size(); i++)
    {
        #pragma omp flush(task)
        task->insert(vec[i]);
    }
    (*e).set_state(2);
}

std::vector<Embedding> triangle_extend(Embedding *e)
{
    extend_result = 0;
    std::vector<Embedding> vec;
    if ((*e).get_size() == 1)
    {
        Edges **list = (*e).get_list();
        int cnt = list[0]->e_cnt;
        for (int i = 0; i < cnt; i++)
        {
            Embedding ep(e, list[0]->vet[i]);
            vec.push_back(ep);
        }
    }
    else
    {
        Edges **list = (*e).get_list();
        int cnt1 = list[0]->e_cnt;
        int cnt2 = list[1]->e_cnt;
        for (int i = 0; i < cnt1; i++)
        {
            bool flag = false;
            for (int j = 0; j < cnt2; j++)
            {
                if ((list[0]->vet[i]) == (list[1]->vet[j]))
                {
                    flag = true;
                }
            }
            if (flag)
            {
                extend_result++; //计数
                /*
                Embedding ep(e, list[0]->vet[i]);
                vec.push_back(ep);
                */
            }
        }
    }
    return vec;
}

long long graph_mining(std::vector<Embedding> (*extend)(Embedding *e), Graph_D* graph)
{
    Comm* comm=new Comm();
    Task_Queue* task=new Task_Queue(graph);
    printf("cnt%d\n", (graph->range_r) - (graph->range_l));
    for (int i = graph->range_l; i < graph->range_r; i++) //加入一个点的embedding
    {
        Embedding new_e(&task->nul, i);
        task->insert(new_e, true);
    }

    int K;
    MPI_Comm_rank(MPI_COMM_WORLD, &K);
    task->current_depth = 1;
    task->current_machine[task->current_depth] = K;
    task->commu[task->current_depth] = K;

    #pragma omp parallel num_threads(NUM_THREADS) shared(task)
    {
        int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();
        int machine_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &machine_rank);
        printf("I'm thread %d in machine %d.\n",my_rank,machine_rank);
        fflush(stdout);
        if (my_rank == 0) comm->give_ans();
        else if (my_rank == 1) comm->ask_ans(task);
        else if (my_rank > 1)
        {
            while (true)
            {
                #pragma omp flush(task)
                Embedding* e = task->new_task();
                printf("size%d\n", e->get_size());
                fflush(stdout);
                if (e->get_size() == 0)
                    break;
                computation(extend, e, task);
                break;
            }
        }
    }
    //Todo: 向其他机器发送结束信号
    return extend_result;
}