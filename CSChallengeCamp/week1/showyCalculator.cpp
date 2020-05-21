#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>

using namespace std;
/* 字体a：
	1234567890
字体b：
	!@#$%^&*()
字体c：
	qwertyuiop
字体d：
	asdfghjkl;
字体e：
	zxcvbnm,./
字体f：
    ... .-. .-. ... .-. .-. .-. .-. .-. .-.
    ..| ..| ..| |.| |.. |.. ..| |.| |.| |.|
    ... .-. .-. .-. .-. .-. ... .-. .-. ...
    ..| |.. ..| ..| ..| |.| ..| |.| ..| |.|
    ... .-. .-. ... .-. .-. ... .-. .-. .-. */
const vector<vector<string>> fontA = {{"0...."},{"1...."},{"2...."},{"3...."},{"4...."},{"5...."},{"6...."},{"7...."},{"8...."},{"9...."},};
const vector<vector<string>> fontB = {{")...."},{"!...."},{"@...."},{"#...."},{"$...."},{"%...."},{"^...."},{"&...."},{"*...."},{"(...."},};
const vector<vector<string>> fontC = {{"p...."},{"q...."},{"w...."},{"e...."},{"r...."},{"t...."},{"y...."},{"u...."},{"i...."},{"o...."},};
const vector<vector<string>> fontD = {{";...."},{"a...."},{"s...."},{"d...."},{"f...."},{"g...."},{"h...."},{"j...."},{"k...."},{"l...."},};
const vector<vector<string>> fontE = {{"/...."},{"z...."},{"x...."},{"c...."},{"v...."},{"b...."},{"n...."},{"m...."},{",...."},{"....."},};
const vector<vector<string>> fontF = {
    {".|.|.", "-...-", ".|.|."},
    {".....", ".....", ".|.|."},
    {".|...", "-.-.-", "...|."},
    {".....", "-.-.-", ".|.|."},
    {"...|.", "..-..", ".|.|."},
    {"...|.", "-.-.-", ".|..."},
    {".|.|.", "-.-.-", ".|..."},
    {".....", "....-", ".|.|."},
    {".|.|.", "-.-.-", ".|.|."},
    {"...|.", "-.-.-", ".|.|."},
    };
class Solution
{
    string origin;
    bool hasf;
    vector<string> result;
    unordered_map<char, vector<vector<string>> > fonts = {
        {'a', fontA},
        {'b', fontB},
        {'c', fontC},
        {'d', fontD},
        {'e', fontE},
        {'f', fontF},
    };
public:
    Solution(): origin(""), hasf(false){}
    Solution& operator()(const string& input)
    {
        origin = input;
        hasf = (input.find('f') != string::npos);
        result.clear();
        convert();
        return *this;
    }
    friend ostream& operator<<(ostream& os, const Solution&);
private:
    void convert()
    {
        for(int i = 1; i < origin.size(); i+=2)
        {
            if(origin[i] != 'f')
            {
                vector<vector<string>> cur = fonts[origin[i]];
                //cout << cur.size() << "<<<<<<<<<<<<<<" <<endl;
                //cout << cur[i-1].size() << "================" << endl;
                int c = origin[i-1]-48;
                result.push_back(cur[origin[i-1] - 48][0]);
            }
                
            else
            {
                result.push_back(fonts[origin[i]][origin[i-1]-48][0]);
                result.push_back(fonts[origin[i]][origin[i-1]-48][1]);
                result.push_back(fonts[origin[i]][origin[i-1]-48][2]);
            }
        }
    }
};

ostream& operator<<(ostream& os, const Solution& solu)
{
    if(solu.hasf)
    {
        
        for(int i = 4; i >=0; --i )
        {
            for(int j = 0 ; j < solu.result.size(); ++j)
            {  
                os << solu.result[j][i];
            }
            if(i != 0)
                os << endl;
        }
    }
    else
    {
        for(auto item: solu.result)
        {
            os << item[0];
        }
    }
    return os;
}

int main()
{
    int n;
    cin >> n;
    string in;
    Solution solution;
    for ( int i =0; i < n; ++i)
    {
        cin >> in;
        cout << solution(in) << endl;
    }
}