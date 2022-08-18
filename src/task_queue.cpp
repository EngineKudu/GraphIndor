#include "../include/graph_d.h"
#include "../include/embedding.h"
#include "../include/task_queue.h"
#include <omp.h>
#include <stdio.h>
#include <iostream>

//Todo: 多线程
void Task_Queue::insert(Embedding* new_e, bool is_last, bool is_end)
{
//    printf("...%d %d %d\n", new_e->get_size(), new_e->get_request(),current_depth);
//    fflush(stdout);
    q[current_depth + 1][graph->get_block_index(new_e->get_request())].push_back(new_e);
    size[current_depth + 1]++;
    //printf("Ins::%d %d %d\n",graph->range_l, current_depth+ 1,graph->get_block_index(new_e->get_request()));
    //fflush(stdout);
    /*if (is_last)
    {
        printf("wow!\n");
        fflush(stdout);
    }*/
    if (is_last || (is_end && size[current_depth + 1] >= Max_size))
    {
/*        printf("!!!!!!!down\n");
        fflush(stdout);*/
//        omp_set_lock(&lock_cc);
        current_depth++;
/*        printf("DOWN%d\n", current_depth);
        fflush(stdout);*/
        int N = graph->get_machine_cnt(); //Todo: 机器数量
        int K = graph->get_machine_id(); //Todo: 当前机器的编号
        current_machine[current_depth] = K;
        commu[current_depth] = K;
        size[current_depth + 1] = 0;
        for (int i = 0; i < N; i++)
        {
            index[current_depth][i] = 0;
            is_commued[current_depth][i] = 0;
        }
//        omp_unset_lock(&lock_cc);
    }
}

bool Task_Queue::insert_vec(Embedding* e, std::vector<Embedding*>* vec)
{
    for (int i = 0; i < (int)vec->size(); i++)
    {
        zero = -1;
        Embedding* new_e=vec->at(i);
        #pragma omp critical
        {
            assert(new_e->get_size() == current_depth + 1);
        //task->insert(new_e, (e->is_last) && ((i + 1) == (int)(vec->size())) , (i + 1) == (int)(vec->size()));
            q[current_depth + 1][graph->get_block_index(new_e->get_request())].push_back(new_e);
            size[current_depth + 1]++;
        }
        /*if(debug)
        {
            printf("INS%d\n", new_e->get_size());
            fflush(stdout);
        }*/
    }
    return ((e->is_last) || size[current_depth + 1] >= Max_size);
/*    if ((e->is_last) || size[current_depth + 1] >= Max_size)
    {
        printf("!!!!!!!down\n");
        fflush(stdout);
//        omp_set_lock(&lock_cc);
        printf("DOWN%d %d\n", current_depth, e->get_size());
        current_depth++;
        fflush(stdout);
        int N = graph->get_machine_cnt(); //Todo: 机器数量
        int K = graph->get_machine_id(); //Todo: 当前机器的编号
        current_machine[current_depth] = K;
        commu[current_depth] = K;
        size[current_depth + 1] = 0;
        for (int i = 0; i < N; i++)
        {
            index[current_depth][i] = 0;
            is_commued[current_depth][i] = 0;
        }
//        omp_unset_lock(&lock_cc);
    }*/
}

//Todo: 多线程
Embedding* Task_Queue::new_task()
{
    int N = graph->get_machine_cnt(); 
    int K = graph->get_machine_id();
    Embedding* new_t = nul;
    /*while (current_depth >= 1)
    {*/
/*        printf("in%d\n", omp_get_thread_num());
        fflush(stdout);*/
        int depth, machine;
        //printf("![%d %d %d]\n", K, current_depth, current_machine[current_depth]);
//        omp_set_lock(&lock_cc);
        while (index[current_depth][current_machine[current_depth]] == (int)q[current_depth][current_machine[current_depth]].size() && (current_machine[current_depth] + 1) % N != K)
        {
/*            printf("ask\n");
            fflush(stdout);*/
            (current_machine[current_depth] += 1) %= N;
        }
        depth = current_depth;
        machine = current_machine[current_depth];
//        omp_unset_lock(&lock_cc);
        //printf("[%d %d %d]\n", K, current_depth, current_machine[current_depth]);
        //printf("{%d %d}\n", index[current_depth][current_machine[current_depth]], (int)q[current_depth][current_machine[current_depth]].size());
        //fflush(stdout);
        if (index[depth][machine] < (int)q[depth][machine].size())
        {
            //zero = -1;
            //return &nul;
            //printf("False%d %d %d %d\n", K, current_depth, current_machine[current_depth], index[current_depth][current_machine[current_depth]]);
            //fflush(stdout);
            Embedding* e = q[depth][machine][index[depth][machine]];
            //printf("ask%d %d %d\n", depth, machine, index[depth][machine]);
            //fflush(stdout);
            while (1)
            {   
                printf("wait%d\n", current_depth);
                fflush(stdout);
                if(e->get_state()==1) break;
            }
            //printf("middle%d\n", omp_get_thread_num());
            //fflush(stdout);
            size[depth]--;
            new_t = q[depth][machine][index[depth][machine]];
            index[depth][machine]++;
            //printf("INDEX%d %d %d %d\n", index[current_depth][current_machine[current_depth]], (int)q[current_depth][current_machine[current_depth]].size(), (current_machine[current_depth] + 1) % N, K);
            //fflush(stdout);
            if (size[depth] == 0)
            {
                new_t->is_last = true;
                /*printf("%d %d %d&*&(*\n", size[current_depth], size[current_depth + 1], current_depth);
                fflush(stdout);*/
            }
            //break;
        }
        //else{printf("asdf   9r8ynoaiuhfamsdf\n");fflush(stdout);}
/*        printf("out%d\n", omp_get_thread_num());
        fflush(stdout);*/
        
        /*zero++;
        while (current_depth == depth)
        {
            if (zero == omp_get_num_threads() - 2)
            {
                #pragma omp single
                {
                    omp_set_lock(&lock_cc);*/
                    /*for (int i = 0; i < N; i++)
                    {
                        q[current_depth][i].clear();
                    }
                    //printf("go up\n");
                    //fflush(stdout);
                    assert(size[current_depth] == 0);
                    current_depth--;*/
                    /*
                    omp_unset_lock(&lock_cc);
                }
            }
        }
        zero--;*/
        
    //}
    return new_t;
}