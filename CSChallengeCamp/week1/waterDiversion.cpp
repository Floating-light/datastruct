#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Cup
{
    Cup(const string& tag, int cap, int water = 0): name(tag),capacity(cap), curWater(water) {}

    Cup(const initializer_list<int>& list)
    {
        if(list.size() >=1 ) name = string(1, *list.begin());
        if(list.size() >= 2) capacity = *(list.begin()+1);
        if(list.size() >= 3) curWater = *(list.begin() + 2);
    }

    int capacity;
    int curWater;
    string name;
};

class WaterDiversion
{
    vector<Cup> cups;
    vector<string> steps;
public:
    WaterDiversion(const initializer_list<initializer_list<int>>& lists)
    {
        for(auto capa : lists)
        {
            cups.push_back(Cup(capa));
        }
    }

    bool isAchieved(const vector<Cup>& cps)
    {
        if(cps[0].curWater == cps[1].curWater&&cps[1].curWater == 40) return true;
        return false;
    }

    bool doDiv(int from, int to, vector<Cup> cps, vector<string> path, vector<string> status)
    {
        Cup& fromCup = cps[from];
        Cup& toCup = cps[to];

        if(fromCup.curWater >= toCup.capacity - toCup.curWater) 
        {
            fromCup.curWater -= (toCup.capacity - toCup.curWater);
            toCup.curWater = toCup.capacity;
        }
        else
        {
            toCup.curWater += fromCup.curWater ;
            fromCup.curWater = 0;
        }
        path.push_back(fromCup.name +" "+  toCup.name);
        if(isAchieved(cps)) 
        {
            steps = path;
            return true;
        }

        string sta ;
        stringstream ss;
        for(int i = 0; i < cps.size(); ++i)
        {
            ss << "-" << cps[i].curWater;
        }
        status.push_back(ss.str());
        if(find(status.begin(), status.end()-1, status.back()) != status.end()-1) 
            return false;
        status.push_back(sta);
        
        for(int f = 0; f < cps.size(); ++f)
        {
            for(int t = 0; t < cps.size(); ++t)
            {
                if(f == t ||
                   f == from && t == to ||
                   f == to&&t == from ||
                   cps[f].curWater == 0 ||
                   cps[t].curWater == cps[t].capacity) continue;

                if(doDiv(f, t,cps, path, status)) return true;
            }
        }
    }

    void Diversion()
    {
        const int& cupsNum = cups.size();

        vector<string> initStatus(1, "-80-0-0");
        int from = 0; // 只有第一个杯子有水
        for(int i = from +1; i < cupsNum; ++i)
        {
            if(doDiv(from, i,cups, vector<string>(),initStatus)) break;
        }
    }
    friend ostream& operator<<(ostream& os, const WaterDiversion& wd);
};

ostream& operator<<(ostream& os, const WaterDiversion& wd)
{
    /* os << "input : \n";
    for(int i = 0; i < wd.cups.size(); ++i)
    {
        os<< "Cup name: " << wd.cups[i].name << " " 
          << "Capacity: " << wd.cups[i].capacity << " "
          << "curWater: " << wd.cups[i].curWater << "."
          << "\n";
    }
    os << "After diversion: \n";*/
    os << wd.steps.size() << endl; 
    for(const string& s: wd.steps)
    {
        os << s << "\n";
    }
}

int main()
{
    WaterDiversion wd = {{'A', 80, 80},
                         {'B', 50, 0},
                         {'C', 30, 0}};
    wd.Diversion();
    cout << wd;
}