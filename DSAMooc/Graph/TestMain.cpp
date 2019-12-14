#include <string>
#include <iostream>
#include <vector>

#include "GraphMatrix.hpp"

using namespace std;

int main() 
{
    GraphMatrix<std::string, int> gm = {"A", "B", "C", "D"};
    
    for( int i = 0; i < gm.n; ++i)
    {
        const int out = gm.outDegree(i);
        const int in = gm.inDegree(i);
        std::cout << "vertex "<< gm.vertex(i) << ": \n"
            << "outDegree : " << out << "\n"
            << "inDegree : " << in << std::endl; 
    }

    int edge = gm.remove(2, 3);
    std::cout << "remove edge : " << edge << std::endl;

        for( int i = 0; i < gm.n; ++i)
    {
        const int out = gm.outDegree(i);
        const int in = gm.inDegree(i);
        std::cout << "vertex "<< gm.vertex(i) << ": \n"
            << "outDegree : " << out << "\n"
            << "inDegree : " << in << std::endl; 
    }
}