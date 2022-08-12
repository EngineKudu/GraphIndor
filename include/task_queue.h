#pragma once
#include "graph_d.h"
#include "embedding.h"
#include <omp.h>
#include <cstring>
#include <vector>
#include <algorithm>

const int pattern_size = 3 + 1;//模式的大小
const int machine_cnt = 4;//机器的数量

class Task_Queue
{
public:
    Task_Queue(Graph_D *G) //G表示分布式图储存
    {
        graph = G;
        current_depth = 0;
        count = 0;
        zero = -1;
        nex = 0;
        ext = false;
//        omp_init_lock(&lock_cc);
        current_machine = new volatile int[pattern_size];
        size = new volatile int[pattern_size];
        commu = new volatile int[pattern_size];
        for (int i = 0; i < pattern_size; i++)
        {
            current_machine[i] = 0;
            size[i] = 0;
            commu[i] = 0;
            for (int j = 0; j < machine_cnt; j++)
            {
                index[i][j] = 0;
                is_commued[i][j] = 0;
            }
        }
        nul = new Embedding();
    }
    ~Task_Queue()
    {
//        omp_destroy_lock(&lock_cc);
        if (current_machine != nullptr) delete[] current_machine;
        if (size != nullptr) delete[] size;
        if (commu != nullptr) delete[] commu;
        for (int i = 0; i < pattern_size; i++)
            for (int j = 0; j < machine_cnt; j++)
                index[i][j] = 0;
        /*if (is_commued != nullptr)
        {
            for (int i = 0; i < pattern_size; i++)
            {
                if (is_commued[i] != nullptr ) delete[] is_commued[i];
            }
            delete[] is_commued;
        }*/
    }
    const int Max_size = 100000;//每层达到Max_size后就将此层设为当前层
    int current_depth;//当前正在扩展哪一层
    volatile int* current_machine;//每层当前在哪台机器
    volatile int* size;//记录每层目前大小
    std::vector<Embedding*> q[pattern_size][machine_cnt]; //每一层，每一台机器，开一个vector储存
    volatile int index[pattern_size][machine_cnt]; //每一层，每一台机器，已经算到了第几个
    //int **is_commued; //每一层，每一台机器，是否已经通信完了
    volatile int is_commued[pattern_size][machine_cnt];
    volatile int *commu; //每一层，已经通信到了第几个
    Graph_D *graph;
    Embedding* nul;
    void insert(Embedding* new_e, bool is_last, bool is_root = false);//加入一个embedding
    bool insert_vec(Embedding* e, std::vector<Embedding*>* vec);//加入一组embedding
    Embedding* new_task();//获取一个新任务,层数与embedding的size相同
    volatile int count;//同时insert且此时需要向下拓一层的进程数
//    omp_lock_t lock_cc;
    volatile int zero, nex;
    volatile int ext;
};