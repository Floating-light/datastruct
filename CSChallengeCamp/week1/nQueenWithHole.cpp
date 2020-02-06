#include <vector>
#include <algorithm>
#include <stack>
#include <iostream>
#include <string>
using namespace std;

struct Queen
{
    int x, y;
    Queen(int xx, int yy) : x(xx), y(yy) {}

    bool operator==(const Queen& r) const
    {
        return (r.x ==x) ||
               (r.y == y) ||
               (r.x + r.y == x + y) ||
               (r.x - r.y == x - y);

    }
    bool operator!=(const Queen& r) const
    {
        return !(*this == r);
    }

};

class Solution
{
public:
    int nQueenWithHole(vector<vector<bool>>& hole)
    {
        int N = hole.size();
        int nums = 0;
        vector<Queen> res;
        Queen q(0,0);
        while(q.x > 0 || q.y < N)
        {
            if( q.y >= N || res.size() >= N) 
            {
                if(res.size() == N) ++nums;
                q = res.back();
                res.pop_back();
                ++q.y;
            } // backtracking
            else
            {
                while((q.y < N && 
                  (find(res.cbegin(), res.cend(), q) != res.cend())) || 
                   hole[q.x][q.y] )
                {
                    ++q.y;
                }
                if(q.y != N)
                {
                    res.push_back(q);
                    ++q.x;
                    q.y = 0;
                }
            }
             
            
        }
        return nums;
    }
};

int main()
{
    int n ;

    cin >> n;
    vector<vector<bool>> input(n, vector<bool>(n, false));
    string s;
    for( int i =0; i < n; ++i )
    {
        for(int j =0; j < n;++j )
        {
            cin >> s;
            if(s == "1") input[i][j] = true;
        }
    }
    /* vector<vector<bool>> input = {
        {1, 1, 0, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }; */

    std::cout << Solution().nQueenWithHole(input) << std::endl;
}