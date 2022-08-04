#pragma once
#include "edges.h"
#include "stdio.h"

class Embedding //维护嵌入的点集、活动边列表
{
public:
    Embedding()
    {
        size = 0;
        state = 1;
        last = -1;
        father = nullptr;
    }
    Embedding(Embedding *fa, int v) //在它的父亲Extendable Embedding中增加一个新的点，即伪代码中的create_extendable_embedding，状态为Pending
    {
        father = fa;
        size = (fa->size) + 1;
        state = 0;
        last = v;
        for (int i = 0; i < size - 1; i++)
        {
            list[i] = fa->list[i];
        }
        list[size - 1] = nullptr;
    }
    ~Embedding()
    {
        if (size > 0) delete[] list[size - 1];
        father = nullptr;
        size = 0;
        state = 3;
        last = 0;
    }
    int get_state();
    int get_size();
    void set_state(int st);
    int get_request(); //若当前embedding为Pending状态，则返回需要访问的节点编号，否则返回-1
    void add_edge(Edges* edge); //传入需要的边列表，将状态变为Ready
    Embedding* get_father();
    Edges **get_list(); //返回活动边列表的指针数组
    Edges* get_edge(int u); //返回u节点的所有边，不存在则返回nullptr
    //Edges get_union_list(int *vet); //Todo 返回一个点集的公共邻点列表，使用Vertical computation sharing优化（是否可行未知
private:
    int state; //state = 0, 1, 2, 3 分别表示Pending, Ready, Zombie, Terminated
    int size;
    int last;
    Edges *list[4]; //pattern_size;
    Embedding *father;
};