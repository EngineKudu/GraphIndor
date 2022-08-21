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
static omp_lock_t lock, lock_d, lock_u;
volatile int ccnt, cccnt, has_down, has_up, dcnt;

v_index_t get_union(e_index_t len1,e_index_t len2,v_index_t* t1,v_index_t* t2)
{
    int h1=0,h2=0;
    v_index_t ans=0;
    while( (h1<len1)&&(h2<len2) )
    {
        if(t1[h1]<t2[h2]) ++h1;
        else if(t1[h1]>t2[h2]) ++h2;
        else
        {
            ++h1,++h2;
            ans++;
        }
    }
    return ans;
}

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
            Embedding* ep=new Embedding(e, list[0]->vet[i], 0/*(e->is_last)&&(i+1==cnt)*/);
            vec->push_back(ep);
        }
    }
    else
    {
        Edges **list = e->get_list();
        int cnt1 = list[0]->e_cnt;
        int cnt2 = list[1]->e_cnt;
        v_index_t new_ans=get_union(cnt1,cnt2,list[0]->vet,list[1]->vet);
        #pragma omp atomic
            extend_result+=new_ans;
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
//    printf("begin%d\n", omp_get_thread_num());
//    fflush(stdout);
    extend(e,vec);
//    printf("end\n");
//    fflush(stdout);
    if(debug)
    {
        printf("extend make %d new embeddings\n",(int)vec->size());
        fflush(stdout);
    }
    //            printf("XXX\n");
    //            fflush(stdout);
    omp_set_lock(&lock);
//    task->nex++;
    bool ff = task->insert_vec(e, vec);
    #pragma omp atomic
    task->nex--;
    /*
    for (int i = 0; i < (int)vec->size(); i++)
    {
        Embedding* new_e=vec->at(i);
        #pragma omp critical
            task->insert(new_e, (e->is_last) && ((i + 1) == (int)(vec->size())) , (i + 1) == (int)(vec->size()));
        if(debug)
        {
            printf("INS%d\n", new_e->get_size());
            fflush(stdout);
        }
    }*/
    omp_unset_lock(&lock);
//    printf("Now%d %d\n", omp_get_thread_num(), task->nex);
    fflush(stdout);
    /*
    if (task->ext)//if (task->nex)
    {
        while (task->ext == true){printf("???\n");fflush(stdout);}
    }
    else
    {
        if (ff == true)
        {
            dcnt = 0;
            printf("to zero %d\n", omp_get_thread_num());
            fflush(stdout);
            has_down = true;
            task->ext = true;
            while (task->nex != 0){printf("wait%d\n", task->nex);fflush(stdout);};
            omp_set_lock(&lock);
//            printf("DOWN!!!!!\n");
            fflush(stdout);
            task->current_depth++;
            int N = task->graph->get_machine_cnt(); //Todo: 机器数量
            int K = task->graph->get_machine_id(); //Todo: 当前机器的编号
            task->current_machine[task->current_depth] = K;
            task->commu[task->current_depth] = K;
            task->size[task->current_depth + 1] = 0;
            for (int i = 0; i < N; i++)
            {
                task->index[task->current_depth][i] = 0;
                task->is_commued[task->current_depth][i] = 0;
            }
            task->zero = -1;
            omp_unset_lock(&lock);
            task->ext = false;
        }
    }*/
    omp_set_lock(&lock_d);
    if (ff == true)
    {
        if (! has_down)
        {
            dcnt = 0;
//            printf("to zero %d\n", omp_get_thread_num());
//            fflush(stdout);
            has_down = true;
            task->ext = true;
            while (task->nex != 0){/*printf("wait%d\n", task->nex);fflush(stdout);*/};
            omp_set_lock(&lock);
//            printf("DOWN!!!!!\n");
            fflush(stdout);
            task->current_depth++;
            int N = task->graph->get_machine_cnt(); //Todo: 机器数量
            int K = task->graph->get_machine_id(); //Todo: 当前机器的编号
            task->current_machine[task->current_depth] = K;
            task->commu[task->current_depth] = K;
            task->size[task->current_depth + 1] = 0;
            for (int i = 0; i < N; i++)
            {
                task->index[task->current_depth][i] = 0;
                task->is_commued[task->current_depth][i] = 0;
            }
            task->zero = -1;
            omp_unset_lock(&lock);
            task->ext = false;
        }
    }
    omp_unset_lock(&lock_d);
//    printf("down done%d %d\n", omp_get_thread_num(), dcnt);
//    fflush(stdout);
    e->set_state(2);
}

long long graph_mining(Graph_D* graph,int debug)
{
    omp_init_lock(&lock);
    omp_init_lock(&lock_d);
    omp_init_lock(&lock_d);
    printf("???\n");
    fflush(stdout);
    int K,N;
    MPI_Comm_rank(MPI_COMM_WORLD, &K);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    Comm* comm=new Comm(graph, &lock);
    Task_Queue* task=new Task_Queue(graph);
    for (int i = graph->range_l; i <= graph->range_r; i++) //加入一个点的embedding
    {
        Embedding* new_e=new Embedding(task->nul, i, (i==graph->range_r));
        task->insert(new_e, new_e->is_last, i == (graph->range_r));
    }
    task->current_depth = 1;
//    printf("X%d %d %d\n", graph->range_l, graph->range_r, task->size[task->current_depth]);
//    fflush(stdout);
    task->current_machine[task->current_depth] = K;
    task->commu[task->current_depth] = K;
//    omp_init_lock(&lock_cc);
    #pragma omp parallel num_threads(NUM_THREADS) shared(task)
    {
        int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();
//        printf("I'm thread %d of machine %d.\n",my_rank,K);
//        fflush(stdout);
        int machine_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &machine_rank);
        if (my_rank == 0) comm->give_ans(); 
        else if (my_rank == 1) comm->ask_ans(task);
        else if (my_rank > 1)//> 1)
        {
            while (true)
            {
                //omp_set_lock(&lock_d);
                #pragma omp atomic
                task->nex++;
                //omp_unset_lock(&lock_d);
                #pragma omp flush(task)
                Embedding* e;
                omp_set_lock(&lock);
//                printf("Add%d %d\n", omp_get_thread_num(), task->nex);
//                fflush(stdout);
//                printf("lock%d\n", omp_get_thread_num());
//                fflush(stdout);
                e=task->new_task();
//                printf("unlock%d\n", omp_get_thread_num());
//                fflush(stdout);
                omp_unset_lock(&lock);
//                printf("Got%d\n", omp_get_thread_num());
//                fflush(stdout);
                if (e->get_size() == 0)
                {
//                    dcnt++;
//                    printf("Del%d\n", omp_get_thread_num());
//                    fflush(stdout);
                    //printf("*****\n");
                    //fflush(stdout);
                    omp_set_lock(&lock);
                    #pragma omp critical
                    {
                        has_up = false;
//                        cccnt = 0;
                        task->zero = ((task->zero) == -1) ? 1 : ((task->zero) + 1);
                        /*if (has_down)
                        {
                            task->zero = -1;
                        }*/
                    }
                    omp_unset_lock(&lock);
                    int now_depth = task->current_depth;
                    #pragma omp atomic
                    task->nex--;
//                    printf("Zero:%d %d %d\n", task->zero, task->current_depth, omp_get_thread_num());
//                    fflush(stdout);
//                    omp_set_lock(&lock);
//                    printf("ffzero%d\n", task->zero);
//                    omp_unset_lock(&lock);
//                    fflush(stdout);
                    while ((task->zero) != -1 && (task->zero) != omp_get_num_threads() - 2)
                    {
//                        printf("zero%d %d %d\n", task->zero, task->nex, has_down);
//                        fflush(stdout);
                        //if (now_depth != task->current_depth)
                        if (has_down)
                        {
                            task->zero = -1;
                            break;
                        }
                    }
//                    dcnt--;
//                    printf("Out%d\n", omp_get_thread_num());
//                    fflush(stdout);
                    omp_set_lock(&lock_u);
                    if ((task->zero) != -1)
                    {
//                        #pragma omp critical
//                        cccnt++;
//                        while (cccnt != omp_get_num_threads() - 2);
//                        printf("Up!!!! %d %d\n", task->zero, now_depth);
//                        fflush(stdout);
                        if (now_depth == 1)
                        {
//                            printf("break!\n");
//                            fflush(stdout);
                            omp_unset_lock(&lock_u);
                            break;
                        }
                        else
                        {
                            if (! has_up)
                            {
                                ccnt = 0;
                                cccnt = 0;
                                has_up = true;
//                                printf("ready%d\n", omp_get_thread_num());
//                                fflush(stdout);
                                omp_set_lock(&lock);
//                                printf("go\n");
//                                fflush(stdout);
                                for (int i = 0; i < N; i++)
                                {
                                    task->q[task->current_depth][i].clear();
                                }
                                assert(task->size[task->current_depth] == 0);
                                task->current_depth--;
//                                printf("gone\n");
//                                fflush(stdout);
                                omp_unset_lock(&lock);
                            }
                        }
                    }
                    omp_unset_lock(&lock_u);
                    if (has_up)
                    {
                        #pragma omp atomic
                        ccnt++;
//                        printf("now ccnt:%d %d\n", omp_get_thread_num(), ccnt);
                        while (ccnt != omp_get_num_threads() - 2);
                        task->zero = -1;
                        #pragma omp atomic
                        cccnt++;
                        while (cccnt != omp_get_num_threads() - 2);
//                        printf("hahaha %d\n", task->zero);
//                        fflush(stdout);
                    }
                    /*
                    if ((task->zero) != -1)
                    {
//                        #pragma omp critical
//                        cccnt++;
//                        while (cccnt != omp_get_num_threads() - 2);
                        printf("Up!!!! %d %d\n", task->zero, now_depth);
                        fflush(stdout);
                        if (now_depth == 1)
                        {
                            printf("break!\n");
                            fflush(stdout);
                            break;
                        }
                        else
                        {
                            if (omp_get_thread_num() == omp_get_num_threads() - 1)
                            {
                            }
                            else
                            {
                                while (task->zero != -1)
                                {
                                    printf("WWW%d\n", task->zero);
                                    fflush(stdout);
                                }
                            }
                            #pragma omp atomic
                            ccnt++;
                            printf("now ccnt:%d %d\n", omp_get_thread_num(), ccnt);
                            while (ccnt != omp_get_num_threads() - 2);
                            printf("hahaha %d\n", task->zero);
                            fflush(stdout);
                        }
                    }*/
//                    printf("ZeroX:%d %d\n", task->zero, task->current_depth);
//                    fflush(stdout);
                }
                else
                {
//                    printf("!!!%d %d %d\n", omp_get_thread_num(), e->get_size(), task->current_depth);
//                    fflush(stdout);
                    computation(e, task,debug);
//                    printf("xxx %d\n", omp_get_thread_num());
//                    fflush(stdout);
                    if(debug)
                    {
                        //printf("Machine%d——Hi,compute e:[size=%d,last=%d]\n",machine_rank,e->get_size(),e->get_request());
                        e->print_list();
                        //printf("state=%d\n",e->get_state());
                        //fflush(stdout);
                    }
                }
                if (has_down)
                {
                    #pragma omp atomic
                    dcnt++;
//                    printf("has_down%d %d\n", omp_get_thread_num(), dcnt);
//                    fflush(stdout);
                    while (dcnt != omp_get_num_threads() - 2)
                    {
                        //printf("wait_dcnt %d %d\n", dcnt, omp_get_thread_num());
                        fflush(stdout);
                    }
//                    printf("out\n");
//                    fflush(stdout);
                    has_down = 0;
                }
            }
            MPI_Barrier(MPI_COMM_WORLD);
            comm->computation_done();
        }
    }
    if(K==0)
    {
        for (int i=1;i<N;++i)
        {
            long long ans;
            MPI_Recv(&ans,1,MPI_LONG,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            extend_result+=ans;
        }
    }
    else
        MPI_Send(&extend_result,1,MPI_LONG,0,0,MPI_COMM_WORLD);
    omp_destroy_lock(&lock);
    return extend_result;
}