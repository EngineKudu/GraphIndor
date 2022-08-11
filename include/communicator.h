#pragma once
# include <cstdint>
# include <vector>
# include "embedding.h"
# include "edges.h"
# include "task_queue.h"

typedef int32_t v_index_t ;
typedef int64_t e_index_t ; 

const int buffer_size=100000;
//Todo:先获取maxdegree再生成Comm

class Comm
{
public:
    int* ask_buffer;
    int* recv_buffer;
    bool all_solved;
    e_index_t max_degree;
    Graph_D* graph;

    Comm(Graph_D* G) 
    {
        graph=G;
        all_solved=0;
        ask_buffer=new int[buffer_size];
        recv_buffer=new int[buffer_size];
        max_degree=buffer_size;
    }

    ~Comm() 
    {
        if(ask_buffer != nullptr) delete[] ask_buffer;
        if(recv_buffer != nullptr) delete[] recv_buffer;
    }

    void give_ans(); //线程0
    void ask_ans(Task_Queue* task); //线程1
    void computation_done();
    void set_max_degree(e_index_t s);
};