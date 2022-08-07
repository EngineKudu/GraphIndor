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
#include <iostream>
#include <unistd.h>
#include <stdio.h>

long long extend_result;
const int NUM_THREADS=3;

void extend(Embedding *e,std::vector<Embedding*>* vec)
{
    extend_result = 0;
    if (e->get_size() == 1)
    {
        Edges **list = e->get_list();
        int cnt = list[0]->e_cnt;
        for (int i = 0; i < cnt; i++)
        {
            Embedding* ep=new Embedding(e, list[0]->vet[i]);
            vec->push_back(ep);
        }
    }
    else
    {
        Edges **list = e->get_list();
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
                extend_result++; //计数
        }
    }
}

void computation(Embedding *e, Task_Queue* task)
{
    std::vector<Embedding*>* vec=new std::vector<Embedding*>;
    printf("Hi,compute e:[size=%d,last=%d]\n",e->get_size(),e->get_request());
    fflush(stdout);
    e->print_list();
    extend(e,vec);
    printf("extend make %d new embeddings\n",(int)vec->size());
    fflush(stdout);
    for (int i = 0; i < (int)vec->size(); i++)
        task->insert(vec->at(i));
    e->set_state(2);
}

long long graph_mining(Graph_D* graph)
{
    int K;
    MPI_Comm_rank(MPI_COMM_WORLD, &K);
    Comm* comm=new Comm(graph);
    Task_Queue* task=new Task_Queue(graph);
    for (int i = graph->range_l; i <= graph->range_r; i++) //加入一个点的embedding
    {
        Embedding* new_e=new Embedding(task->nul, i);
        task->insert(new_e, true);
    }
    task->current_depth = 1;
    task->current_machine[task->current_depth] = K;
    task->commu[task->current_depth] = K;
    #pragma omp parallel num_threads(NUM_THREADS) shared(task)
    {
        int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();
        printf("I'm thread %d of machine %d.\n",my_rank,K);
        fflush(stdout);
        int machine_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &machine_rank);
        //if (my_rank == 0) comm->give_ans(); else 
        if (my_rank == 1) comm->ask_ans(task);
        else if (my_rank > 1)
        {
            while (true)
            {
                #pragma omp flush(task)
                Embedding* e;
                #pragma omp critical
                    e=task->new_task();
                if (e->get_size() == 0)
                    break;
                computation(e, task);
            }
            comm->computation_done();
        }
    }
    //Todo: 向其他机器发送结束信号
    return extend_result;
}