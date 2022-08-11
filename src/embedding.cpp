#include "../include/edges.h"
#include "../include/embedding.h"

int Embedding::get_state()
{
    return state;
}

int Embedding::get_size()
{
    return size;
}

void Embedding::set_state(int st)
{
    state = st;
}

int Embedding::get_request()
{
    return last;
}

void Embedding::add_edge(Edges* edge)
{
    list[size - 1] =edge;
    state = 1;
}

Embedding* Embedding::get_father()
{
    return father;
}

Edges** Embedding::get_list()
{
    return list;
}

Edges* Embedding::get_edge(int u)
{
    for (int i = 0; i < size; i++)
    {
        if ((list[i]->v) == u)
        {
            return list[i];
        }
    }
    return nullptr;
}

void Embedding::print_list()
{
    printf("Edge_list=[");
    for (int i=0;i<list[ size-1 ]->e_cnt;++i)
        printf("%d,",list[ size-1 ]->vet[i]);
    printf("]\n");
}