#include <string>

class TreeNode
{
public:
    TreeNode():
    value(""), count(new int(0)), left(nullptr), right(nullptr)
    {

    }

    TreeNode(const TreeNode& in):
    value(in.value), count(in.count), left(in.left), right(in.right)
    {
        ++*count;
    }
    
    ~TreeNode()
    {
        if(--*count == 0)
        {
            delete left;
            delete right;
            delete count;
        }
    }

    TreeNode& operator=(const TreeNode& rig)
    {
        ++*rig.count;
        if(--*count == 0)
        {
            delete left;
            delete right;
            delete count;
        }
        value = rig.value;
        count = rig.count;
        left = rig.left;
        right = rig.right;
        return *this;
    }

private:
    std::string value;
    int *count;
    TreeNode *left;
    TreeNode *right;
};

class BinStrTree
{
public:
    BinStrTree():root(nullptr)
    {}

    BinStrTree(const BinStrTree& rgh):root(new TreeNode(*rgh.root))
    {

    }

    ~BinStrTree()
    {
        delete root;
    }

    BinStrTree& operator=(const BinStrTree& rgh)
    {
        TreeNode* p = rgh.root;
        delete root;
        root = p;
        return *this;
    }

private:
    TreeNode* root;
};

int main() 
{
    
}