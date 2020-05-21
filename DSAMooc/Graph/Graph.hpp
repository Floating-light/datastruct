#include <stack>
#include <queue>
#include <limits>

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; // The status of vertex
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus; // The type of edges int traversal

template <typename Tv, typename Te> // the type of vertex and edge.
class Graph
{
private:
    void reset() // reset the status of information in vertex and edge
    {
        for( int i = 0; i < n; ++i )
        {
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1;
            parent(i) = -1;
            priority(i) = std::numeric_limits<int>::max();
            for( int j = 0; j < n; ++j)
            {
                if( exists(i, j)) type(i, j) = UNDETERMINED;
            }
        }
    }

    void BFS( int v, int& clock)
    {
        std::queue<int> q;
        status(v) = DISCOVERED;
        q.push(v);
        while(!q.empty())
        {
            v = q.front();
            q.pop();
            dTime(v) = ++clock;
            for( int u = firstNbr(v); -1< u;u = nextNbr(v, u) )
            {
                if( status(u) == UNDISCOVERED)
                {
                    q.push(u);
                    status(u) = DISCOVERED;
                    type(v, u) = TREE;
                    parent(u) = v;
                }
                else
                {
                    type(v, u) = CROSS;
                }
            }
            std::cout << vertex(v) << "--->" << std::endl;
            // visited operation
            status(v) = VISITED; //
        }
    }
    void DFS( int v, int& clock )
    {
        dTime(v) = ++clock;
        status(v) = DISCOVERED;
        std::cout << vertex(v) << " dTime: " << dTime(v) << std::endl;
        for(int u = firstNbr(v); -1 < u; u = nextNbr(v , u))
        {
            if(status(u) == UNDISCOVERED)
            {
                type(v, u) = TREE;
                parent(u) = v;
                DFS(u, clock);
            }
            else if(status(u) == DISCOVERED)
            {
                type(v, u) =BACKWARD;
            }
            else // VISITED
            {
                type(v, u) = dTime(v) < dTime(u) ? FORWARD : CROSS;
            }
            
        }
        status(v) = VISITED;
        fTime(v) = ++clock;
        std::cout << vertex(v) << " fTime: " << fTime(v) << std::endl;
    }
    void BCC( int, int&, std::stack<int>& ); // (连通域) 基于DFS的双连通分量分解算法
    bool TSort( int, int&, std::stack<Tv>* ); // (连通域) 基于DFS的拓扑排序算法
    template <typename PU> void PFS ( int, PU); // 优先级搜索框架
public:
// for vertex
    int n; // total vertex number
    virtual int insert( const Tv&) = 0; 
    virtual Tv remove( int) = 0;
    virtual Tv& vertex( int) = 0; // get the data of vertex 
    virtual int inDegree( int) = 0;
    virtual int outDegree( int) = 0;
    virtual int firstNbr( int) =0; // get the first neighbor vertex
    virtual int nextNbr( int, int) =0 ; // the next neighbor of vertex v corresponding to vertex j.
    virtual VStatus& status( int ) = 0; // the status of vertex i
    virtual int& dTime(int ) = 0;
    virtual int& fTime( int) = 0;
    virtual int& parent(int ) = 0; // the parent of vertex v in traversal tree
    virtual int& priority( int ) = 0; 

// for edges, all undirected edges represented by two directed edges.
    int e;  
    virtual bool exists( int, int) = 0; // edge(u, v) is exists ?
    virtual void insert( const Te & , int, int, int) = 0; // insert a edge e with weight w between u and v
    virtual Te remove( int, int) = 0; // remove edge e between u and v
    // the type of edge (u,v);
    virtual EStatus& type( int, int) = 0; 
    virtual Te& edge( int, int) = 0; // the data of edge (u,v)
    virtual int& weight(int , int ) = 0; // weight of edge (u,v)

// algorithm
    void bfs(int s)
    {
        reset();
        int clock = 0;
        int v = s;
        do
        {
            if(status(v) == UNDISCOVERED)
            {
                BFS(v, clock);
            }
        }while( s!= ( v = (++v%n)));
    }
    void dfs(int s)
    {
        reset();
        int clock = 0;
        int v = s;
        do
        {
            if( status(v) == UNDISCOVERED)
            {
                DFS(v, clock);
            }
        }while( s!= (v = (++v%n)));
    }
    void bcc(int ); // 基于DFS的双连通分量分解算法
    std::stack<Tv>* tSort( int); // 基于DFS的拓扑排序算法
    void prim( int); // 最小支撑树prim 算法
    void dijklstra(int ); // 最短路径：Dijkstra
    template <typename PU> void pfs( int , PU); // 优先级搜索框架
};
