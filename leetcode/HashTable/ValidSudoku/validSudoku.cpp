// 36. Valid Sudoku
// tags: Hash Table
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <bitset>

using namespace std;

class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        unordered_map<char, int> mp_hori;
        unordered_map<char, int> mp_vert;

        unordered_map<char, int> mp_left;
        unordered_map<char, int> mp_mid;
        unordered_map<char, int> mp_right;
        for(int i = 0; i < board.size(); ++ i)
        {
            mp_hori.clear();
            mp_vert.clear();
            if(i%3 == 0)
            {
                mp_left.clear();
                mp_mid.clear();
                mp_right.clear();
            }
            for(int j = 0; j < board[i].size(); ++j)
            {
                if (board[i][j] !='.')
                {
                    if(++mp_hori[board[i][j]] > 1)
                    {
                        return false;
                    }  
                    if(j < 3 && ++mp_left[board[i][j]] > 1 ||
                        j > 5 && ++mp_right[board[i][j]] > 1 ||
                        3<=j&&j<=5&&++mp_mid[board[i][j]] > 1)
                    {
                        return false;
                    } 
                }  
                if (board[j][i] !='.')
                {
                    if(++mp_vert[board[j][i]] > 1)
                    {
                        return false;
                    }   
                }   
            }
        }
        return true;
    }

    bool isValidSudoku2(vector<vector<char>>& board) {
        int column[9] = {0};
        int matrix[9] = {0};
        for(int i = 0; i < board.size(); ++ i)
        {
            int row = 0;

            for(int j = 0; j < board[i].size(); ++j)
            {
                if(board[i][j] =='.') continue;
                const int& horiz = 1 << (board[i][j] - '0'); 
                const int& mat = (i/3)*3 + j%3;
                char s[256];
            
                //itoa(column[j], s, 2);
                printf("%s\n", s);
                if(row& horiz ||
                    column[j]& horiz ||
                    matrix[mat]& horiz)
                {
                    cout<<bitset<sizeof(int)*8>(row)<<endl; 
                    cout<<bitset<sizeof(int)*8>(column[j])<<endl; 
                    cout<<bitset<sizeof(int)*8>(matrix[mat])<<endl; 
                    return false;
                }  
                row |= horiz;
                column[j] |= horiz;
                matrix[mat] |= horiz;
            }
        }
        return true;
    }
};

int main()
{
    vector<vector<char>> input = 
    {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };

    vector<vector<char>> in = 
    {
        {'8','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };

    vector<vector<char>> in3 = // true
    {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };

    std::cout << Solution().isValidSudoku2(in3) << std::endl;

}