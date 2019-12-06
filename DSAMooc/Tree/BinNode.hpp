#ifndef BINNODE_HPP
#define BINNODE_HPP

#define BinNodePosi(T) BinNode<T>*  // node position
typedef enum { RB_RED, RB_BLACK } RBColor;
template <typename T> struct BinNode
{
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lChild;
    BinNodePosi(T) rChild;
    int height;
    int npl; // null path length
    RBColor color; // red black tree

    // constructor
    BinNode() : parent(nullptr), lChild(nullptr), rChild(nullptr),
        height(0), npl(1), color( RB_RED) { }
    // construct from a data
    BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, 
        int h = 0, int l = 1, RBColor c = RB_RED) : 
        data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c)
        { }

    // the total number of descendant of the current node, 
    // also is the scale of the sub tree root by current node. 
    int size(); 

    //insert new data as the left child of current node
    BinNodePosi(T) insertAsLeftChild(const T& );

    //insert new data as the right child of current node
    BinNodePosi(T) insertAsRightChild(const T& );

    //get the direct succfix of current node
    BinNodePosi(T) succ();

    template <typename VST> void travLevel( VST& );
    template <typename VST> void travPre( VST& );
    template <typename VST> void travIn( VST& );
    template <typename VST> void travPost( VST& );

    bool operator< (BinNode cosnt& bn ) { return data < bn.data ; }
    bool operator==( BinNode const& bn ) { return data == bn.data; }
};

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLeftChild(T const & e ) // assum lChild == nullptr
{
    return lChild = new BinNode<T>(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRightChild(T const & e ) // assum rChild == nullptr
{
    return rChild = new BinNode<T>(e, this);
}

template <typename T>
int BinNode<T>::size()
{
    int s = 1;
    if ( lChild ) s += lChild->size();
    if ( rChild ) s += rChild->size();
    return s;
} // O(n) = |size|

#endif