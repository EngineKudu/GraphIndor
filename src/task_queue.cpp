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
    printf("Ins::%d %d %d\n",graph->range_l, current_depth+ 1,graph->get_block_index(new_e->get_request()));
    fflush(stdout);
    /*if (is_last)
    {
        printf("wow!\n");
        fflush(stdout);
    }*/
    if (is_last || (is_end && size[current_depth + 1] >= Max_size))
    {
        current_depth++;
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
    }
}

//Todo: 多线程
Embedding* Task_Queue::new_task()
{
    int N = graph->get_machine_cnt(); 
    int K = graph->get_machine_id();
    Embedding* new_t = nul;
    while (current_depth >= 1)
    {
        //printf("![%d %d %d]\n", K, current_depth, current_machine[current_depth]);
        while (index[current_depth][current_machine[current_depth]] == (int)q[current_depth][current_machine[current_depth]].size() && (current_machine[current_depth] + 1) % N != K)
        {
            (current_machine[current_depth] += 1) %= N;
        }
        //printf("[%d %d %d]\n", K, current_depth, current_machine[current_depth]);
        //printf("{%d %d}\n", index[current_depth][current_machine[current_depth]], (int)q[current_depth][current_machine[current_depth]].size());
        //fflush(stdout);
        if (index[current_depth][current_machine[current_depth]] < (int)q[current_depth][current_machine[current_depth]].size())
        {
            //return &nul;
            //printf("False%d %d %d %d\n", K, current_depth, current_machine[current_depth], index[current_depth][current_machine[current_depth]]);
            //fflush(stdout);
            Embedding* e = q[current_depth][current_machine[current_depth]][index[current_depth][current_machine[current_depth]]];
            while (e->get_state()!=1);
            //printf("True\n");
            //fflush(stdout);
            size[current_depth]--;
            new_t = q[current_depth][current_machine[current_depth]][index[current_depth][current_machine[current_depth]]];
            index[current_depth][current_machine[current_depth]]++;
            //printf("INDEX%d %d %d %d\n", index[current_depth][current_machine[current_depth]], (int)q[current_depth][current_machine[current_depth]].size(), (current_machine[current_depth] + 1) % N, K);
            //fflush(stdout);
            if (index[current_depth][current_machine[current_depth]] == (int)q[current_depth][current_machine[current_depth]].size() && (current_machine[current_depth] + 1) % N == K)
            {
                //printf("^^^\n");
                //fflush(stdout);
                new_t->is_last = true;
                /*printf("%d %d %d&*&(*\n", size[current_depth], size[current_depth + 1], current_depth);
                fflush(stdout);*/
            }
            break;
        }
        for (int i = 0; i < N; i++)
        {
            q[current_depth][i].clear();
        }
        //printf("go up\n");
        //fflush(stdout);
        assert(size[current_depth] == 0);
        current_depth--;
    }
    return new_t;
}