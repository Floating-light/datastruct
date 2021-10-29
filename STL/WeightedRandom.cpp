#include <iostream>
#include <random>

int main() 
{
    std::vector<double> weights{90,56,4,1000};
    std::discrete_distribution<int> dist(std::begin(weights), std::end(weights));
    std::mt19937 gen;
    gen.seed(time(0));//if you want different results from different runs
    int N = 100000;
    std::vector<int> samples(N);
    for(auto & i: samples)
    {
        i = dist(gen);
        std::cout << i << std::endl;

    }
    //do something with your samples...
    return 0;
}