#include <vector>
#include <stack>
#include <algorithm>

#include "Graph.hpp"

template <typename Tv> 
struct Vertex
{
    Tv data;
    int inDegree, outDegree;
    VStatus status;
    int dTime, fTime;
    int parent;
    int priority;

    Vertex(const Tv & d = (Tv)0 ):
        data( d), inDegree(0), outDegree(0), status(UNDISCOVERED),
        dTime(-1), fTime(-1), parent(-1), priority(std::numeric_limits<int>::max())
    {

    }
};

template <typename Te>
struct Edge
{
    Te data;
    int weight;
    EType type;
    Edge( Te const& d, int w):
        data(d), weight(w), type( UNDETERMINED)
    {

    }
};

template <typename Tv, typename Te>
class GraphMatrix: public Graph<Tv, Te>
{
private: 
    std::vector<Vertex<Tv>> V; // the set of vertices
    std::vector<std::vector<Edge<Te>*>> E; // the set of edges
public:
    GraphMatrix() { Graph<Tv, Te>::n = Graph<Tv, Te>::e = 0; }
    GraphMatrix(const std::initializer_list<Tv>& input)
    {
        Graph<Tv, Te>::n = Graph<Tv, Te>::e = 0; 
        for( const Tv& d : input)
        {
            insert(d);
        }
        insert(Te(0), 11, 0, 1);
        insert(Te(0), 22, 1, 0);
        insert(Te(0), 33, 0, 3);
        insert(Te(0), 44, 2, 0);
        insert(Te(0), 55, 1, 2);
        insert(Te(0), 66, 3, 1);
        insert(Te(0), 77, 2, 3);
        insert(Te(0), 88, 3, 2);

    }
    ~GraphMatrix() 
    {
        for( int i = 0; i < Graph<Tv, Te>::n; ++i)
            for( int j = 0; j < Graph<Tv, Te>::n; ++j)
                delete E[i][j];
    }

    // the base operation of vertex

    //retrieve the vertex
    virtual Tv& vertex( int i ) { return V[i].data; }

    virtual  int inDegree( int i ) { return V[i].inDegree; }

    virtual int outDegree( int i ) { return V[i].outDegree; }

    // the next neighbor of i corresponding to j
    virtual int nextNbr( int i, int j) // O(n)
    {
        while( (-1 < j) && ( !exists(i, --j)) );
        return j;
    }

    virtual int firstNbr( int i) { return nextNbr(i, Graph<Tv, Te>::n); }

    virtual VStatus& status(int i) { return V[i].status; }

    virtual int& dTime(int i) { return V[i].dTime; }

    virtual int& fTime(int i) { return V[i].fTime; }

    // the father of vertex i in the traversal tree
    virtual int& parent(int i) { return V[i].parent; }

    virtual int& priority( int i) { return V[i].priority; }

    // the dynamic operations of vertex
    virtual int insert (const Tv& vertex)
    {
        for( int j = 0; j < Graph<Tv, Te>::n; ++j) E[j].push_back(nullptr);
        Graph<Tv, Te>::n++;
        E.push_back(std::vector<Edge<Te>*>(Graph<Tv, Te>::n , nullptr));
        V.push_back(Vertex<Tv>(vertex));
        return V.size();
    }

    virtual Tv remove( int i)
    {
        for( int j = 0; j < Graph<Tv, Te>::n; ++j)
            if( exists(i, j) )
            {
                delete E[i][j];
                V[j].inDegree--;
            }
        E.erase(E.begin() + i);
        Graph<Tv, Te>::n--;
        Tv vBak = vertex( i);
        V.erase(V.begin() + i);
        for( int j = 0; j < Graph<Tv, Te>::n; ++j)
        {
            Edge<Te>* e = E[j][i];
            E[j].erase(E[j].begin() + i);
            V[j].outDegree--;
            if(e)
                delete e;
        }
        return vBak;
    }

    virtual bool exists( int i, int j)
    {
        return (0 <= i) && (i < Graph<Tv, Te>::n) && (0 <= j) && ( j < Graph<Tv, Te>::n ) && E[i][j] != nullptr;
    }

    // the base operation of edge
    virtual EType& type( int i, int j) { return E[i][j]->type; }

    virtual Te& edge( int i, int j) { return E[i][j]->data; }

    virtual int& weight( int i, int j) { return E[i][j]->weight; }

    // the dynamic operation of edge

    // insert a edge (i , j) with weight w;
    virtual void insert( const Te& edge, int w, int i, int j)
    {
        if( exists( i, j) ) return ;
        E[i][j] = new Edge<Te>( edge, w);
        Graph<Tv, Te>::e++;
        V[i].outDegree++;
        V[j].inDegree++;
    }

    // exit(i,j) ?
    virtual Te remove( int i, int j) 
    {
        Te eBak = edge( i, j); 
        delete E[i][j];
        E[i][j] = nullptr;
        V[i].outDegree--;
        V[j].inDegree--;
        return eBak;
    }
};
