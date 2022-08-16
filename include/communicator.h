#pragma once
# include <cstdint>
# include <vector>
# include "embedding.h"
# include "edges.h"
# include "task_queue.h"
# include <map>

typedef int32_t v_index_t ;
typedef int64_t e_index_t ; 

const int buffer_size=100000;
const int hash_mod=1000003;
//Todo:先获取maxdegree再生成Comm
class Comm
{
public:
    int* ask_buffer;
    int* recv_buffer;
    bool all_solved;
    e_index_t max_degree;
    Graph_D* graph;
    Edges** hash_table;
    int* hash_value;
    /*
    int cache_size,cache_now_size;
    Edges** edge_cache;
    std::map<int,int> in_cache;
    */

    Comm(Graph_D* G) 
    {
        graph=G;
        all_solved=0;
        ask_buffer=new int[buffer_size];
        max_degree=G->maxdegree;
        recv_buffer=new int[max_degree];
        hash_table=new Edges*[hash_mod];
        hash_value=new int[hash_mod];
        for (int i=0;i<hash_mod;++i) hash_value[i]=-1,hash_table[i]=new Edges();
        /*
        cache_size=G->all_vertex/10;
        cache_now_size=0;
        edge_cache=new Edges*[cache_size];
        */
    }

    ~Comm() 
    {
        if(ask_buffer != nullptr) delete[] ask_buffer;
        if(recv_buffer != nullptr) delete[] recv_buffer;
        delete[] hash_table;
        delete[] hash_value;
        //in_cache.clear();
    }

    void give_ans(); //线程0
    void ask_ans(Task_Queue* task); //线程1
    void computation_done();
    void set_max_degree(e_index_t s);
};