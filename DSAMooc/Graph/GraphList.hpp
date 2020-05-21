#include <vector>

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
    EStatus type;

    int endVertex;
    Edge<Te>* nextEdge;

    Edge( Te const& d, int w):
        data(d), weight(w), type( UNDETERMINED), nextEdge(nullptr), endVertex(-1)
    {

    }
};

template <typename Tv, typename Te>
class GraphList : public Graph<Tv, Te>
{
private:
    std::vector<Vertex<Tv>> V;
    std::vector<Edge<Te>*> E;

public:
    GraphList() { Graph<Tv, Te>::n = Graph<Tv, Te>::e = 0; }
    GraphList(const std::initializer_list<Tv>& input)
    {
        Graph<Tv, Te>::n = Graph<Tv, Te>::e = 0; 
        for( const Tv& d : input)
        {
            insert(d);
        }
        insert(Te(123), 11, 0, 1);
        insert(Te(123), 22, 1, 0);
        insert(Te(123), 33, 0, 3);
        insert(Te(123), 44, 2, 0);
        insert(Te(123), 55, 1, 2);
        insert(Te(123), 66, 3, 1);
        insert(Te(123), 77, 2, 3);
        insert(Te(123), 88, 3, 2);

    }
    ~GraphList() 
    {
        for( int i = 0; i < Graph<Tv, Te>::n; ++i)
        {
            Edge<Te>* curEdge = E[i];
            while(curEdge)
            {
                Edge<Te>* temp = curEdge;
                curEdge = temp->nextEdge;
                delete temp;
            }
        }
    }

    // the base operation of vertex

    //retrieve the vertex
    virtual Tv& vertex( int i ) { return V[i].data; }

    virtual  int inDegree( int i ) { return V[i].inDegree; }

    virtual int outDegree( int i ) { return V[i].outDegree; }

    // the next neighbor of i corresponding to j
    virtual int nextNbr( int i, int j) // O(n)
    {
        Edge<Te>* cur = E[i]->nextEdge;
        while(cur)
        {
            if(cur->endVertex == j)
            {
                if(cur->nextEdge)
                    j = cur->nextEdge->endVertex;
                else
                    j = -1;
                    
                break;
            }   
            cur = cur->nextEdge; 
        }
        return j;
    }

    virtual int firstNbr( int i) { return E[i]->nextEdge->endVertex; }

    virtual VStatus& status(int i) { return V[i].status; }

    virtual int& dTime(int i) { return V[i].dTime; }

    virtual int& fTime(int i) { return V[i].fTime; }

    // the father of vertex i in the traversal tree
    virtual int& parent(int i) { return V[i].parent; }

    virtual int& priority( int i) { return V[i].priority; }

    // the dynamic operations of vertex
    virtual int insert (const Tv& vertex)
    {
        Edge<Te>* e = new Edge<Te>(Te(233), -1);
        e->nextEdge = nullptr;
        E.push_back(e);
        Graph<Tv, Te>::n++;
        V.push_back(Vertex<Tv>(vertex));
        return V.size();
    }

    virtual Tv remove( int i)
    {
        Edge<Te>* curE = E[i];
        while(curE)
        {
            Edge<Te>* temp = curE;
            V[temp->endVertex].inDegree--;
            curE = curE->nextEdge;
            delete temp;
            Graph<Tv, Te>::e--;
        }
        E.erase(E.begin() + i);

        Graph<Tv, Te>::n--;
        
        Tv vBak = vertex( i);
        V.erase(V.begin() + i);

        for(int vi = 0; vi < Graph<Tv, Te>::n; ++vi)
        {
            Edge<Te>* e = E[vi];
            while(e)
            {
                if( e->nextEdge && e->nextEdge->endVertex == i)
                {
                    Edge<Te>* temp = e->nextEdge;
                    e->nextEdge = temp->nextEdge;
                    delete temp;
                    Graph<Tv, Te>::e--;
                    V[vi].outDegree--;
                }
                else
                {
                    e = e->nextEdge;
                }
            }
        }
        return vBak;
    }
    // O(1)
    virtual bool exists( int i, int j)
    {
        if( (0 <= i) && (i < Graph<Tv, Te>::n) && (0 <= j) && ( j < Graph<Tv, Te>::n ))
        {
            Edge<Te>* e = E[i]->nextEdge;
            while(e)
            {
                if( e->endVertex == j)
                    return true;
                e = e->nextEdge;
            }
        }
        return false;
    }

    // the base operation of edge
    virtual EStatus& type( int i, int j) { 
        Edge<Te>* e = E[i]->nextEdge;
        while(e)
        {
            if( e->endVertex == j)
                break;
            e = e->nextEdge;
        }    
        return e->type;
    }

    virtual Te& edge( int i, int j) 
    { 
        Edge<Te>* e = E[i]->nextEdge;
        while(e)
        {
            if( e->endVertex == j)
                break;
            e = e->nextEdge;
        }    
        return e->data;
     }

    virtual int& weight( int i, int j) { 
        Edge<Te>* e = E[i]->nextEdge;
        while(e)
        {
            if( e->endVertex == j)
                break;
            e = e->nextEdge;
        }    
        return e->weight;
     }

    // the dynamic operation of edge

    // insert a edge (i , j) with weight w;
    virtual void insert( const Te& edge, int w, int i, int j)
    {
        if( exists( i, j) ) return ;
        Edge<Te>* e = new Edge<Te>( edge, w);
        e->nextEdge = E[i]->nextEdge;
        e->endVertex = j;
        E[i]->nextEdge = e;

        Graph<Tv, Te>::e++;

        V[i].outDegree++;
        V[j].inDegree++;
    }

    // exit(i,j) ?
    virtual Te remove( int i, int j) 
    {
        if( !exists(i, j)) return Te(0);
        Te eBak = edge( i, j); 
        Edge<Te>* e = E[i];
        while(e)
        {
            if( e->nextEdge && e->nextEdge->endVertex == j)
            {
                Edge<Te>* temp = e->nextEdge;
                e->nextEdge = temp->nextEdge;
                delete temp;
                V[i].outDegree--;
                V[j].inDegree--;
                Graph<Tv, Te>::e--;
                break;
            }
        }
        return eBak;
    }
};