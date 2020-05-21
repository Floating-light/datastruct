#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>

using namespace std;

enum SentenceType
{
    Week,
    Plan,
    Order
};

class Sentence
{
public:
    Sentence() = default;
    Sentence(const string& s) : sen(s) {}

    SentenceType getType() {return type; }
    string getKeyWords() { return keyWord; }
    int number() { return n; }
    virtual ~Sentence() = default;
protected:
    string sen;

    SentenceType type;
    
    string keyWord;
    int n;
};

class HumanPattern : public Sentence
{
    const string weekPrefix =  "Today is ";
    const string planPrefix =  "I want to ";
    const string orderSuffix = ", please.";
public:
    HumanPattern(const string& s) : Sentence(s) 
    {
        initMorpheme();
    }

    HumanPattern(const Sentence& s): Sentence(s)
    {

    }

    void initMorpheme()
    {
        if(sen.substr(0, weekPrefix.size()) == weekPrefix)
        {
            type = Week;
            sen.pop_back(); // 去掉尾部标点
            keyWord = sen.substr(weekPrefix.size());
        }
        else if(sen.substr(0,planPrefix.size()) == planPrefix)
        {
            type = Plan;
            sen.pop_back();
            keyWord = sen.substr(planPrefix.size());
        }
        else
        {
            type = Order;

            size_t firstEmpty = sen.find_first_of(' ');
            size_t firstComma = sen.find_first_of(',');

            keyWord = sen.substr(firstEmpty+1, firstComma - firstEmpty-1);

            stringstream ss(sen.substr(0, firstEmpty));
            ss >> n;
        }
    }

    string getSentence()
    {
        if(type == Week)
        {
            return weekPrefix +  keyWord + "." ;
        }
        else if(type == Plan)
        {
            return planPrefix + keyWord + ".";
        }
        else 
        {
            stringstream ss;
            ss << n;
            return ss.str() + " " + keyWord + orderSuffix;
        }
    }
};

class ComPattern : public Sentence
{
    const string weekPrefix =  "Var day=";
    const string planPrefix =  "Var plan=";
    const string orderSuffix = ", please.";
    unordered_map<string, string> weekMap={
            {"Monday", "1"},
            {"Tuesday", "2"},
            {"Wednesday", "3"},
            {"Thursday", "4"},
            {"Friday", "5"},
            {"Saturday", "6"},
            {"Sunday", "7"}
        };
public:
    ComPattern(const string& s) : Sentence(s) 
    {
        initMorpheme();
    }

    ComPattern(const Sentence& s): Sentence(s)
    {

    }

    void initMorpheme()
    {
        if(sen.substr(0, weekPrefix.size()) == weekPrefix)
        {
            type = Week;
            sen.pop_back(); // 去掉尾部标点

            keyWord = sen.substr(weekPrefix.size()); // the week number

            auto itr = weekMap.cbegin();
            while(itr != weekMap.cend())
            {
                if(itr->second == keyWord) 
                {
                    keyWord = itr->first;
                    break;
                }
                ++itr;
            }
        }
        else if(sen.substr(0,planPrefix.size()) == planPrefix)
        {
            type = Plan;
            sen.pop_back(); // 去掉句末分号
            sen.pop_back(); // 去掉右引号
            keyWord = sen.substr(planPrefix.size() + 1); // 去掉左引号
        }
        else
        {
            type = Order;
            size_t leftQuo = sen.find_first_of('\"');
            size_t rightQuo = sen.find_last_of('\"');
            keyWord = sen.substr(leftQuo + 1, rightQuo - leftQuo - 1);

            size_t lastEqual = sen.find_last_of('=');
            stringstream ss;
            ss << sen.substr(lastEqual + 1, sen.size() - lastEqual - 2);
            ss >> n;
        }
    }

    string getSentence()
    {
        if(type == Week)
        {
            return weekPrefix + weekMap[keyWord] +";";
        }
        else if(type == Plan)
        {
            return planPrefix + "\"" +keyWord + "\";";
        }
        else 
        {
            stringstream ss;
            ss << n;
            return  "Var item=\""+keyWord+"\"; Var num="+ss.str()+";";
        }
    }
};

class LittleCPattern : public Sentence
{
    const string weekPrefix =  "Oh, my god. that's incredible. You know what? I just found that today is ";
    const string planPrefix =  "My god! What should I do today? Let me see. Well, I have an excellent idea! Let us go to ";
    const string orderPrefix = "I want one ";
    const string repeatStr = " and one more,";
    unordered_map<string, string> weekMap={
            {"Monday", "MMMonday"},
            {"Tuesday", "TTTuesday"},
            {"Wednesday", "WWWednesday"},
            {"Thursday", "TTThursday"},
            {"Friday", "FFFriday"},
            {"Saturday", "SSSaturday"},
            {"Sunday", "SSSunday"}
        };
public:
    LittleCPattern(const string& s) : Sentence(s) 
    {
        initMorpheme();
    }

    LittleCPattern(const Sentence& s): Sentence(s)
    {

    }

    void initMorpheme()
    {
        //"Oh, my god. that's incredible. You know what? I just found that today is MMMonday/TTTuesday/WWWednesday/TTThursday/FFFriday/SSSaturday/SSSunday!"
        if(sen.substr(0, weekPrefix.size()) == weekPrefix)
        {
            type = Week;
            sen.pop_back(); // 去掉尾部标点

            keyWord = sen.substr(weekPrefix.size()); // the week trible perfix

            auto itr = weekMap.cbegin();
            while(itr != weekMap.cend())
            {
                if(itr->second == keyWord) 
                {
                    keyWord = itr->first;
                    break;
                }
                ++itr;
            }
        }
        // "My god! What should I do today? Let me see. Well, I have an excellent idea! Let us go to XXX."
        else if(sen.substr(0,planPrefix.size()) == planPrefix)
        {
            type = Plan;
            sen.pop_back(); // 去掉句末逗号
            keyWord = sen.substr(planPrefix.size()); // 去掉左引号
        }
        // "I want one XXX, and one more, and one more, and one more …, and one more." (共N-1个"and one more")
        else
        {
            type = Order;
            size_t firstComma = sen.find_first_of(',');
            if(firstComma == string::npos) 
            {
                firstComma = sen.size() - 1;
                n = 1;
            }
            else
            {
                n = (sen.size() - firstComma - 1) / repeatStr.size() + 1;
            }
            keyWord = sen.substr(orderPrefix.size(), firstComma - orderPrefix.size());
        }
    }

    string getSentence()
    {
        if(type == Week)
        {
            sen = weekPrefix + weekMap[keyWord] +"!";
        }
        else if(type == Plan)
        {
            sen =  planPrefix+keyWord + ".";
        }
        else 
        {
            sen = orderPrefix + keyWord + ",";
            for(int i =1; i < n; ++i)
            {
                sen += repeatStr;
            }
            sen.back() = '.';
        }
        return sen;
    }
};

class Translator
{
protected:
    Sentence sen;
public:
    Translator(const Sentence& s) : sen(s) { }

    Sentence sentData()
    {
        return sen;
    }

    virtual string getSentence() = 0;
 
    virtual ~Translator(){}
};

class Human : public Translator
{
public: 
    Human(const string& s) : Translator(HumanPattern(s)){}

    Human(const Sentence& sen): Translator(HumanPattern(sen)) { }

    virtual string getSentence()
    {
        HumanPattern hp = static_cast<HumanPattern>(sen);
        return hp.getSentence();
    }
};

class Computer : public Translator
{
public:
    Computer(const string& s) : Translator(ComPattern(s)){}

    Computer(const Sentence& sen): Translator(ComPattern(sen)) { }

    virtual string getSentence()
    {
        ComPattern cp = static_cast<ComPattern>(sen);
        return cp.getSentence();
    }

};

class LittleC : public Translator
{
public:
    LittleC(const string& s) : Translator(LittleCPattern(s)){}

    LittleC(const Sentence& sen): Translator(LittleCPattern(sen)) { }

    virtual string getSentence()
    {
        LittleCPattern lp = static_cast<LittleCPattern>(sen);

        return lp.getSentence();
    }

};

int main()
{
    int totalSentence;
    cin >> totalSentence;
    string input ;
    getline(cin, input);
    for(int i = 0; i < totalSentence; ++i)
    {
        getline(cin, input);
        Sentence s;
        if(input[0] == 'A')
            s = Human(input.substr(4)).sentData();
        else if(input[0] == 'B')
            s = Computer(input.substr(4)).sentData();
        else
            s = LittleC(input.substr(4)).sentData();
        
        if(input[2] == 'A')
            cout << Human(s).getSentence() << std::endl;
        else if(input[2] == 'B')
            cout << Computer(s).getSentence() << std::endl;
        else
            cout << LittleC(s).getSentence() << std::endl;
    }
}

//int main()
//{
    //string input = "Today is Monday.";
    //string input = "I want to XX X.";
    //string input = "11 XX X, please.";

    
    /* Human human(input );
    Computer com(human.sentData());
    std::cout << com.getSentence() << std::endl; */

    //string input = "Var day=1;";
    // string input = "Var plan=\"XX X\";";
    /* string input = "Var item=\"XX X\"; Var num=24;";

    Computer com(input );
    Human human(com.sentData());
    std::cout << human.getSentence() << std::endl; */

    // string input = "Oh, my god. that's incredible. You know what? I just found that today is MMMonday!";

    // string input = "My god! What should I do today? Let me see. Well, I have an excellent idea! Let us go to XX X.";

    //string input = "I want one XX X, and one more, and one more, and one more, and one more.";
    //string input = "I want one XX X.";

    //string input = "Today is Monday.";
    //string input = "I want to XX X.";
//     string input = "1 XX X, please.";
//     Human com(input );
//     LittleC human(com.sentData());
//     std::cout << human.getSentence() << std::endl;
// }