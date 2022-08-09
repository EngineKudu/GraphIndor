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

long long extend_result=0;
const int NUM_THREADS=3;
int S = 0, SS = 0;

void extend(Embedding *e,std::vector<Embedding*>* vec)
{
//    printf("..%d\n", e->get_size());
//    fflush(stdout);
    if (e->get_size() == 1)
    {
        S++;
        Edges **list = e->get_list();
        int cnt = list[0]->e_cnt;
        for (int i = 0; i < cnt; i++)
        {
            SS++;
            Embedding* ep=new Embedding(e, list[0]->vet[i], 0/*(e->is_last)&&(i+1==cnt)*/ );
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

void computation(Embedding *e, Task_Queue* task,int debug)
{
    std::vector<Embedding*>* vec=new std::vector<Embedding*>;
    if(debug)
    {
        printf("Hi,compute e:[size=%d,last=%d]\n",e->get_size(),e->get_request());
        fflush(stdout);
        e->print_list();
    }
    extend(e,vec);
    if(debug)
    {
        printf("extend make %d new embeddings\n",(int)vec->size());
        fflush(stdout);
    }
    for (int i = 0; i < (int)vec->size(); i++)
    {
        Embedding* new_e=vec->at(i);
        task->insert(new_e, new_e->is_last , (i + 1) == (int)(vec->size()));
        if(debug)
        {
            printf("INS%d\n", new_e->get_size());
            fflush(stdout);
        }
    }
    e->set_state(2);
}

long long graph_mining(Graph_D* graph,int debug)
{
    int K;
    MPI_Comm_rank(MPI_COMM_WORLD, &K);
    Comm* comm=new Comm(graph);
    Task_Queue* task=new Task_Queue(graph);
    for (int i = graph->range_l; i <= graph->range_r; i++) //加入一个点的embedding
    {
        Embedding* new_e=new Embedding(task->nul, i, (i==graph->range_r));
        task->insert(new_e, new_e->is_last, i == (graph->range_r));
    }
    task->current_depth = 1;
        printf("%d %d %d\n", graph->range_l, graph->range_r, task->size[task->current_depth]);
        fflush(stdout);
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
        else if (my_rank == 2)//> 1)
        {
            while (true)
            {
                #pragma omp flush(task)
                Embedding* e;
                #pragma omp critical
                    e=task->new_task();
                if (e->get_size() == 0)
                    break;
                if(debug) printf("S%d\n", e->get_size());
                fflush(stdout);
                computation(e, task,debug);
            }
            comm->computation_done();
        }
    }
    //Todo: 向其他机器发送结束信号
    std::cout << "()" << extend_result << std::endl;
    int SSS = 0;
    Edges* e;
    for (int i = 0; i < 7115; i++)
    {
        e = new Edges();
        graph->get_neighbor(i, e);
        SSS += e->e_cnt;
    }
    printf("%d %d %d\n", S, SS, SSS);
    fflush(stdout);
    return extend_result;
}