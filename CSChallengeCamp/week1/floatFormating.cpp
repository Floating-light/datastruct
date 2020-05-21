#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace std;

enum RepairDir
{
    Left,
    Right,
};

class Solution 
{
    public:

    // 关键是理清需要满足的条件的优先次序
    // 小数点后保留位数 > 打印宽度(补位) > 补位方向
    bool isValid(const string& format, const string& f)
    {
        auto itrLeft = find_if(f.cbegin(), f.cend(), [](const char & c){
            return 48<=c&&c<=57;
        });
        auto itrRight = find_if(itrLeft, f.cend(), [](const char & c){
            return c == '*';
        });
        string realll(itrLeft, itrRight); // 实际浮点数

        // 获取格式化串中要求保留的位数.
        int floatNum;
        int pPoint = format.find_last_of('.');
        int pf = format.find_last_of('f');
        stringstream ss;
        ss << format.substr(pPoint+1, pf - pPoint - 1);
        ss >> floatNum;
        ss.clear();
        
        string floatPart; //小数部分
        int point = realll.find('.');
        if(point == string::npos) floatPart = "";
        else floatPart = realll.substr(point+1);

        // 保留的小数位数是否正确
        if(floatPart.size() != floatNum) return false;

        int totalWise; //要求的打印宽度
        int i = 0;
        while(format[i] != '.' &&i < format.size() )
        {
            if(48<=format[i]&&format[i]<=57)
                ss << format[i];
            ++i;
        }
        ss >> totalWise;

        // 实际浮点数更长 或无需补位
        if(realll.size() >=  totalWise) return true;

        // 总的格式化输出位数是否正确
        if(f.size() != totalWise) return false;

        RepairDir formatRepairDir = Left;
        if(format[1] == '-') formatRepairDir = Right;

        if((f[0] == '*' && formatRepairDir==Left) ||
            (f.back() == '*' && formatRepairDir == Right))
            return true;//补位方向是否正确
        return false;
    }
};

int main()
{

    string inputFormat = "%10.2f";
    string inputFloat = "1.14";
    cin >> inputFormat;
    cin >> inputFloat;
    if(Solution().isValid(inputFormat, inputFloat))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;

}