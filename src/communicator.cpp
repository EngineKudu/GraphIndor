#include "../include/communicator.h"
#include "../include/graph_d.h"
#include "../include/task_queue.h"
#include <mpi.h>
#include <omp.h>
#include <iostream>

void Comm::give_ans() //线程0-回复其他机器的询问
{
    printf("Try to give_ans\n");
    fflush(stdout);
    int comm_sz,my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int** ask;
    MPI_Request* recv_r;
    Edges** e;
    MPI_Status status;
    int flag=0;
    ask=new int*[comm_sz+1];
    recv_r=new MPI_Request[comm_sz+1];
    e=new Edges*[comm_sz+1];
    for (int i=0;i<comm_sz;++i)
    {
        if(i==my_rank) continue;
        ask[i]=new int[ max_degree ];
        MPI_Irecv(ask[i],max_degree,MPI_INT,i,graph->all_vertex+i,MPI_COMM_WORLD,&recv_r[i]);
        e[i]=new Edges();
    }
    while(!all_solved)
    {
        for (int i=0;i<comm_sz;++i)
        {
            if(i==my_rank) continue;
            MPI_Test(&recv_r[i],&flag,&status);
            if(flag==0) continue;
//            printf("Machine %d respond Machine%d:%d\n",my_rank,i,ask[i]);
  //          fflush(stdout);
            int ask_cnt;
            MPI_Get_count(&status,MPI_INT,&ask_cnt);
            //printf("machine %d Respond machine %d,size=%d\n",my_rank,i,ask_cnt);
            //fflush(stdout);
            for (int j=0;j<ask_cnt;++j)
            {
                //printf("ask[%d][%d]=%d\n",i,j,ask[j]);
                //fflush(stdout);
                graph->get_neighbor(ask[i][j],e[i]);
                //printf("Send_size=%lu\n",e[i]->e_cnt);
                //fflush(stdout);
                MPI_Send(e[i]->vet,e[i]->e_cnt,MPI_INT,i,ask[i][j],MPI_COMM_WORLD);
            }
            MPI_Irecv(ask[i],max_degree,MPI_INT,i,graph->all_vertex+i,MPI_COMM_WORLD,&recv_r[i]);
            //TODO：改为Isend，但要检查一下上次的有没有发出
        }
    }
}

void Comm::ask_ans(Task_Queue* task)//线程1
{
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    while (!all_solved)
    {
        #pragma omp flush(task)
        int depth = task->current_depth;
        int index = task->commu[depth];
        (task->commu[depth] += 1) %= comm_sz;
        if (depth == 0)
        {
            break;
        }
        std::vector<Embedding*>& vec=task->q[depth][index];
        Edges* edge;
        if (! task->is_commued[depth][index])
        {
            if(index==my_rank)
            {
                v_index_t x;
                // printf("depth=%d index=%d size=%d\n",depth,index,(int)vec.size());
                for (int i=0;i<(int)vec.size();++i)
                {
                    edge=new Edges();
                    x=vec[i]->get_request(); 
                    graph->get_neighbor(x,edge);
                    vec[i]->add_edge(edge);
//                  printf("V%d %d %d %d\n", my_rank, depth, index, i);
                }
            }
            else
            {
                int size=vec.size();
                if(size==0) continue;
                /*
                printf("BEGIN\n");
                fflush(stdout);
                printf("size====[%d,%d,%d]\n",depth,index,size);
                fflush(stdout);
                */

                MPI_Status status;
                int ask_size=0;
                for (int i=0;i<size;++i)
                {
                    int x=vec[i]->get_request();
                    ask_buffer[ask_size++]=x;
                }
                MPI_Send(ask_buffer,ask_size,MPI_INT,index,graph->all_vertex+my_rank,MPI_COMM_WORLD);
                /*
                printf("machine %d ask:size=%d\n",my_rank,ask_size);
                fflush(stdout);
                for (int i=0;i<size;++i)
                    printf("[%d]",ask_buffer[i]);
                fflush(stdout);
                */
                for (int i=0;i<size;++i)
                {
                    MPI_Recv(recv_buffer,max_degree,MPI_INT,index,vec[i]->get_request(),MPI_COMM_WORLD,&status);
                    Edges* edge=new Edges();
                    edge->v=vec[i]->get_request();
                    int cnt=0;
                    MPI_Get_count(&status,MPI_INT,&cnt);
                    edge->e_cnt=cnt;
                    edge->vet=new v_index_t[edge->e_cnt];
                    for (int j=0;j<edge->e_cnt;++j)
                        edge->vet[j]=recv_buffer[j];
                    vec[i]->add_edge(edge);
                    /*
                    printf("Recv %u:size=%d\n[",edge->v,cnt);
                    fflush(stdout);
                    
                    for (int j=0;j<edge->e_cnt;++j)
                        printf("%u,",edge->vet[j]);
                    printf("]\n");
                    fflush(stdout);
                    */
                }
                //printf("End\n");
                //fflush(stdout);
            }
            task->is_commued[depth][index] = 1;
        }
    }
}

void Comm::computation_done()
{
    all_solved=true;
}

void Comm::set_max_degree(e_index_t s)
{
    max_degree=s;
}

/*
void Comm::comm_recv() //线程1
{
    while(!all_task_solved)
    {
        if(l_ask==r_ask) continue;
        int pos=(l_ask+1)%size,tar;
        tar=graph->get_block_index(asks[pos]);
        v_index_t now=asks[pos];
        MPI_Isend(&asks[pos],1,MPI_INT,tar,0,MPI_COMM_WORLD);
        assert( (r_ans+1)%size==l_ans && "等待回答队列长度不足" );
        r_ans=(r_ans+1)%size;
        ans[r_ans].v=asks[pos];
        anse[r_ans]=aske[pos];
        ans[r_ans].vet=new v_index_t[max_degree];
        MPI_Irecv(ans[r_ans].vet,max_degree,MPI_INT,tar,asks[pos],MPI_COMM_WORLD,recv_request[r_ans]);
        l_ask=(l_ask+1)%size;
    }
    while(!all_task_solved)
    {
        if(l_ans==r_ans) continue;
        int pos=l_ans;
        MPI_Status status;
        MPI_Wait(&recv_request[pos],&status);
        anse[pos]->add_edge(ans[r_ans]);
        anse[pos]->set_state(1);
        l_ans=(l_ans+1)%size;
        int nl=l_ans,nr=r_ans; //在等待过程中可能会出现新询问，所以记录一下当前的lr
        MPI_Status status;
        int flag=0; 
        for (int i=nl;i<=nr;++i)
        {
            MPI_Test(&recv_request[i],&flag,&status);
            if(flag)
            {
                if(i==l_ans) l_ans=(l_ans+1)%size;
            }
        }
*/
