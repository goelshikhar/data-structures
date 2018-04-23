#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

typedef struct road
{
    int end;
    int btime;
    int ttime;
    road *next;
} road;

typedef struct junction
{
    int tltime;
    road *next;
} junction;

typedef junction *junctionptr;

road *newRoad(int j, int m, int n, road *x)
{
    road *r= new road;
    r->end= j;
    r->btime= m;
    r->ttime= n;
    r->next= x;
    return r;
}

junctionptr *junctions= new junctionptr[1000001];
int first=-1;
int vertices=0;

void addJunction(int x, int y)
{
    junctions[x]= new junction;
    junctions[x]->tltime= y;
    junctions[x]->next= nullptr;
    if (first==-1)
        first= x;
    vertices+=1;
}

void addRoad(int i, int j, int m, int n)
{
    junctions[i]->next= newRoad(j, m, n, junctions[i]->next);
    junctions[j]->next= newRoad(i, m, n, junctions[j]->next);
}

void demolishEdge(int i, int j)
{
    if (junctions[i]==nullptr)
        return;
    
    if (junctions[i]->next==nullptr)
        return;

    if (junctions[i]->next->end==j)
    {
        junctions[i]->next= junctions[i]->next->next;
        return;
    }

    road *t= junctions[i]->next;
    while(t!=nullptr && t->next!=nullptr)
    {
        if(t->next->end==j)
            t->next= t->next->next;
        t= t->next;
    }
}

void demolishRoad(int i, int j)
{
    demolishEdge(i, j);
    demolishEdge(j, i);
}

typedef struct edge
{
    int u;
    int v;
    int w;
} edge;

edge* createEdge(int x, int y, int z)
{
    edge *e= new edge;
    e->u=x;
    e->v=y;
    e->w=z;
    return e;
}

class comparator1
{
    public: int operator() (edge e1, edge e2)
    {
        return e1.w > e2.w;
    }
};

bool VinTV(int v, int *TV, int countVertices)
{
    for(int i=0; i<countVertices; i++)
        if (TV[i]==v)
            return true;
    return false;
}

bool compare(edge e1, edge e2)
{
    if (e1.u < e2.u)
        return true;
    else if (e1.u > e2.u)
        return false;
    else if (e1.u < e2.u)
        return true;
    else
        return false;
}

void printMST()
{
    if (first==-1 || junctions[first]->next==nullptr)
    {
        cout << -1 << endl;
        return;
    }

    priority_queue< edge, vector<edge>, comparator1> pq;
    edge *MST= new edge[vertices-1];
    int *TV= new int[vertices];
    int countEdges=0;
    int countVertices=1;
    TV[0]= first;
    
    while(countEdges<(vertices-1))
    {
        road *t= junctions[TV[countVertices-1]]->next;
        while(t!=nullptr)
        {
            pq.push(*createEdge(TV[countVertices-1], t->end, t->btime));
            t=t->next;
        }

        while(!pq.empty() && VinTV(pq.top().v, TV, countVertices))
        {
            // cout << "Popping " << pq.top().u << " to " << pq.top().v << ", time: " <<  pq.top().w << endl;
            pq.pop();
        }

        if (pq.empty())
        {
            cout << "-1" << endl;
            return;
        }

        TV[countVertices]= pq.top().v;
        countVertices+=1;
        MST[countEdges]= pq.top();
        countEdges+=1;
    }


    for (int i=0; i<countEdges; i++)
    {
        if (MST[i].u > MST[i].v)
        {
            int s= MST[i].v;
            MST[i].v=MST[i].u;
            MST[i].u=s;
        }
    }

    sort(MST, MST+countEdges,compare);

    int W= 0;
    for (int i=0; i<countEdges ; i++)
        W+= MST[i].w;

    cout << countEdges << " " << W;

        for (int i=0; i<countEdges ; i++)
        cout << " " << MST[i].u << " " << MST[i].v;
    cout << endl;

}

int *dist;

typedef struct vertexNode
{
    int id;
    int distance;
} vertexNode;

vertexNode *createVertexNode(int i, int j)
{
    vertexNode *v= new vertexNode;
    v->id= i;
    v->distance= j;
    return v;
}

class comparator2
{
    public: int operator() (vertexNode a, vertexNode b)
    {
        return a.distance > b.distance;
    }
};

typedef struct pathNode
{
    int pathVertex;
    pathNode *next;
} pathNode;

pathNode *createPathNode(int a, pathNode* b)
{
    pathNode *p= new pathNode;
    p->pathVertex= a;
    p->next= b;
    return p;
}

typedef pathNode* pathNodeptr;

int junctionPass(int u)
{
    if (dist[u]%(junctions[u]->tltime)==0)
        return dist[u];
    else
        return (dist[u]/(junctions[u]->tltime) + 1)*junctions[u]->tltime;
}

void quickTravel(int source, int destination)
{
    dist= new int[1000001];
    for (int i=0; i<1000001; i++)
        dist[i]= INT_MAX;
    dist[source]=0;

    pathNodeptr *path= new pathNodeptr[1000001];
    for (int i=0; i<1000001; i++)
        path[i]=nullptr;

    priority_queue<vertexNode, vector<vertexNode>, comparator2> pq;
    pq.push(*createVertexNode(source, 0));

    while (!pq.empty())
    {
        int u= pq.top().id;
        pq.pop();

        road *t= junctions[u]->next;
        while (t!=nullptr)
        {
            int v= t->end;

            if (dist[v]> junctionPass(u)+t->ttime)
            {
                dist[v]= junctionPass(u)+t->ttime;
                // path[v]= path[u];
                if (path[u]== nullptr)
                {
                    path[v]= createPathNode(v, nullptr);
                }
                else
                {
                    path[v]= createPathNode(path[u]->pathVertex, nullptr);
                    pathNodeptr t3= path[u]->next;
                    pathNodeptr t4= path[v];
                    while(t3!=nullptr)
                    {
                        t4->next= createPathNode(t3->pathVertex, nullptr);
                        t4=t4->next;
                        t3=t3->next;
                    }
                    t4->next= createPathNode(v, nullptr);
                }
                pq.push(*createVertexNode(v, dist[v]));
            }

            t=t->next;
        }
    }

    if (dist[destination]!=INT_MAX)
    {
        cout << dist[destination];
        int pathLength =1;
        pathNodeptr t2= path[destination];
        while(t2!=nullptr)
        {
            pathLength+=1;
            t2= t2->next;
        }
        cout << " " << pathLength << " " << source;
        t2= path[destination];
        while(t2!=nullptr)
        {
            cout << " " << t2->pathVertex;
            t2= t2->next;
        }
        cout << endl;
    }
    else
        cout << -1 << endl;
}


        // // FOR DEBUGGING
        // void print()
        // {
        //     for (int i=0; i<1000001; i++)
        //     {
        //         if (junctions[i]!=nullptr)
        //         {
        //             cout << "Junction " << i << " has Traffic Light Time " << junctions[i]->tltime << "\n";
        //             road *t= junctions[i]->next;
        //             while(t!=nullptr)
        //             {
        //                 cout << "Connects junction " << t->end << ", build time= " << t->btime << ", traverse time= " << t->ttime << endl;
        //                 t= t->next;
        //             }
        //             cout << endl;
        //         }
        //     }
        // }

        // int main()
        // {
        //     for(int i=0; i<1000001; i++)
        //     {
        //         junctions[i]= nullptr;
        //     }

        //     int opCode;
        //     while (true)
        //     {
        //         cin >> opCode;
        //         if (opCode==1)
        //         {
        //             int o1, o2;
        //             cin >> o1 >> o2;
        //             addJunction(o1, o2);
        //         }
        //         else if (opCode==2)
        //         {
        //             int o1, o2, o3, o4;
        //             cin >> o1 >> o2 >> o3 >> o4;
        //             addRoad(o1, o2, o3, o4);
        //         }
        //         else if (opCode==3)
        //         {
        //             int o1, o2;
        //             cin >> o1 >> o2;
        //             demolishRoad(o1, o2);
        //         }
        //         else if (opCode==4)
        //             printMST();
        //         else if (opCode==5)
        //         {
        //             int o1, o2;
        //             cin >> o1 >> o2;
        //             quickTravel(o1, o2);
        //         }
        //         print();
        //     }
        // }

// INSTRUCTED CODE
int main()
{
    for(int i=0; i<1000001; i++)
    {
        junctions[i]= nullptr;
    }

    int V, E;
    cin >> V >> E;
    for(int i=0; i<V; i++)
    {
        int o1, o2;
        cin >> o1 >> o2;
        addJunction(o1, o2);
    }
    
    for(int i=0; i<E; i++)
    {
        int o1, o2, o3, o4;
        cin >> o1 >> o2 >> o3 >> o4;
        addRoad(o1, o2, o3, o4);
    }

    int ops;
    cin >> ops;
    
    for(int i=0; i<ops; i++)
    {
        int opCode;
        cin >> opCode;
        if (opCode==1)
        {
            int o1, o2;
            cin >> o1 >> o2;
            addJunction(o1, o2);
        }
        else if (opCode==2)
        {
            int o1, o2, o3, o4;
            cin >> o1 >> o2 >> o3 >> o4;
            addRoad(o1, o2, o3, o4);
        }
        else if (opCode==3)
        {
            int o1, o2;
            cin >> o1 >> o2;
            demolishRoad(o1, o2);
        }
        else if (opCode==4)
            printMST();
        else if (opCode==5)
        {
            int o1, o2;
            cin >> o1 >> o2;
            quickTravel(o1, o2);
        }
    }
}