#include "graph_d.h"
#include "embedding.h"
#include <vector>
#include <algorithm>

Class Task_Queue
{
public:
    Task_Queue(Graph_D *G) //G表示分布式图储存
    {
        graph = G;
        //
    }
    ~Task_Queue()
    {
        //
    }
    const int Max_size;//每层达到Max_size后就将此层设为当前层
    int current_depth;//当前正在扩展哪一层
    int current_machine[pattern_size];//每层当前在哪台机器
    int size[pattern_size];//记录每层目前大小
    std::vector<Embedding> q[pattern_size][machine_cnt]; //每一层，每一台机器，开一个vector储存
    void insert(Embedding new_e);//加入一个embedding
    Embedding new_task();//获取一个新任务,层数与embedding的size相同
private:
    Graph_D *graph;
}
