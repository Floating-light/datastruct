#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

int slove(vector<vector<int>>& in)
{
    if(in.size() < 5 || in[0].size() < 5) return 0;
    vector<bool> row(in.size());
    vector<bool> col(in[0].size());
    for (size_t i = 0; i < in.size(); i++)
    {
        // unordered_map<int, bool> m;
        vector<int> v(6);
        
        for(size_t j = 0; j < in[i].size(); ++j)
        {
            if(in[i][j] == 0) continue;
            if(!v[in[i][j]]) // no occurs
            {
                v[in[i][j]] = true;
            }
            else
            {
                row[i] = false;
                break;
            }
            
        }
        if(v[1] && v[2] &&v[3] &&v[4] &&v[5])
        {
            row[i] = true;
        }

    }
    
    
}



int main()
{

}