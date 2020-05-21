#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

enum Pawn
{
    H,
    W,
    S,
    V,
    END
};

class CrossRiver
{
private:
    vector<bool> E;
    vector<bool> R;
    
    vector<string> name = {"H", "W", "S", "V"};
    vector<string> result;
public:
    CrossRiver() = default;

    bool isValid(const vector<bool>& status)
    {
        if(status[W] && status[S] || status[S]&&status[V]) return false; 
        return true;
    }

    void doCross(vector<bool> begin,vector<bool> target,vector<string> res)
    {
        if(!result.empty()) return ;
        if(count(target.cbegin(), target.cend(), false) == END && res.size()>0) 
        {
            result=res;
            return;
        }
        for(int i = 0; i < END; ++i)
        {
            if(!begin[i] ) continue;

            string currentStep = name[0] + (i != 0 ? name[i] : "");
            if(!res.empty() && res.back() == currentStep ) continue;

            vector<bool> b = begin;
            vector<bool> t = target;
            b[0] = false;
            b[i] = false;

            t[0]= true;
            t[i]=true;

            if(isValid(b))
            {
                vector<string> r(res);
                r.push_back(currentStep);
                doCross(t, b, r);
            }
        }
    }

    void cross()
    {
        E = vector<bool>(END, true);
        R = vector<bool>(END, false);

        vector<string> res;
        doCross(E, R,res);
    }

    friend ostream& operator<<(ostream&,const CrossRiver&);
};

ostream& operator<<(ostream& os,const CrossRiver& cr)
{
    for(int i = 0; i < cr.result.size(); ++i)
    {
        os << cr.result[i] << " ";
    } 
    return os;
}

int main()
{
    CrossRiver cr;
    cr.cross();

    cout << cr << std::endl;
}
