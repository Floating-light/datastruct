#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> result;
        if(s.empty() || words.empty()) return result;
        sort(words.begin(), words.end());
        
        do
        {
            string sub;
            for(const string& w : words)
            {
                sub += w;
            }
            size_t p = 0;
            while((p = s.find(sub, p)) != string::npos)
            {
                result.push_back(p);
                ++p;
            }
        }while(next_permutation(words.begin(), words.end()));
        return result;
    }

    vector<int> findSubstring2(string s, vector<string>& words) 
    {
        vector<int> result;
        if(s.empty() || words.empty()) return result;
        unordered_map<string, int> count;
        for(string word : words) count[word]++;

        const size_t& signalLength = words[0].size();
        const size_t& slength = words.size();
        const size_t& totalLength = signalLength*slength;

        for(int i = 0; totalLength + i <= s.size(); ++i)
        {
            // if(hasWord(s.substr(i, totalLength), words, signalLength, totalLength))
            // {
            //     result.push_back(i);
            // }
            unordered_map<string, int> see;
            int j = 0;
            while( j < slength)
            {
                const string& w = s.substr(i + j*signalLength, signalLength);
                if(count.find(w) != count.end())
                {
                    see[w]++;
                    if (see[w] > count[w]) break;
                }
                else break;
                ++j;
            }
            if( j == slength) result.push_back(i);
        }
        return result;
    }

    vector<int> findSubstring3(string s, vector<string>& words) {
        vector<int> result;
		if (words.empty())return result;
		unordered_map<string, int> counts, record;
		for (string word : words) {
			counts[word]++;
		}
		int len = words[0].size(), num = words.size(), sl = s.size();
		for (int i = 0; i < len; ++i) {
			int left = i, sum = 0;
            record.clear();
			for (int j = i; j <= sl - len; j+=len) {
				string word = s.substr(j, len);
				if (counts.count(word)) {
					record[word]++;
                    sum++;
                    while (record[word] > counts[word])
                    {
                        //remove the most left word
                        record[s.substr(left, len)]--;
                        left += len;
                        sum--;
                    }
					if (sum == num) 
						result.push_back(left);
				}
				else
				{
					record.clear();
					sum = 0;
					left = j + len;
				}
			}
		}
		return result;
    }

private:
    bool hasWord(const string&s, const vector<string>& words, const int& signalNum, const int& totalNum)
    {
        vector<bool> occurs(words.size(), false);
        int cur = 0;
        while(cur < totalNum)
        {
            bool find = false;
            for( int i = 0; i < words.size(); ++i)
            {
                if(!occurs[i] && s.substr(cur, signalNum) == words[i])
                {
                    cur += signalNum;
                    occurs[i] = true;
                    find = true;
                    break;
                }
            }
            if(!find) break;
        }
        if(cur == s.size()) return true;
        else return false;
    }
};

class Solution2 {
public:

vector<int> findSubstring(string s, vector<string>& words) 
{
    unordered_map<string, int> counts;
    for (string word : words) counts[word]++;
    int n = s.length(), num = words.size(), len = words[0].length();
    vector<int> indexes;
    for (int i = 0; i < n - num * len + 1; i++) 
    {
        unordered_map<string, int> seen;
        int j = 0;
        for (; j < num; j++) 
        {
            string word = s.substr(i + j * len, len);
            if (counts.find(word) != counts.end()) 
            {
                seen[word]++;
                if (seen[word] > counts[word]) break;
            }
            else break;
        }

        if (j == num) indexes.push_back(i);
    }
    return indexes;
}

};

int main() 
{
    // vector<string> ss{"afasf", "zsdfs", "bsferf", "aasdf"};
    // sort(ss.begin(), ss.end());
    // for(string item : ss)
    // {
    //     std::cout << item << std::endl;
    // }
    // while(next_permutation(ss.begin(), ss.end()))
    // {
    // for(string item : ss)
    // {
    //     std::cout << item << ", ";
    // }
    // std::cout << std::endl;
    // }

    string s = "pjzkrkevzztxductzzxmxsvwjkxpvukmfjywwetvfnujhweiybwvvsrfequzkhossmootkmyxgjgfordrpapjuunmqnxxdrqrfgkrsjqbszgiqlcfnrpjlcwdrvbumtotzylshdvccdmsqoadfrpsvnwpizlwszrtyclhgilklydbmfhuywotjmktnwrfvizvnmfvvqfiokkdprznnnjycttprkxpuykhmpchiksyucbmtabiqkisgbhxngmhezrrqvayfsxauampdpxtafniiwfvdufhtwajrbkxtjzqjnfocdhekumttuqwovfjrgulhekcpjszyynadxhnttgmnxkduqmmyhzfnjhducesctufqbumxbamalqudeibljgbspeotkgvddcwgxidaiqcvgwykhbysjzlzfbupkqunuqtraxrlptivshhbihtsigtpipguhbhctcvubnhqipncyxfjebdnjyetnlnvmuxhzsdahkrscewabejifmxombiamxvauuitoltyymsarqcuuoezcbqpdaprxmsrickwpgwpsoplhugbikbkotzrtqkscekkgwjycfnvwfgdzogjzjvpcvixnsqsxacfwndzvrwrycwxrcismdhqapoojegggkocyrdtkzmiekhxoppctytvphjynrhtcvxcobxbcjjivtfjiwmduhzjokkbctweqtigwfhzorjlkpuuliaipbtfldinyetoybvugevwvhhhweejogrghllsouipabfafcxnhukcbtmxzshoyyufjhzadhrelweszbfgwpkzlwxkogyogutscvuhcllphshivnoteztpxsaoaacgxyaztuixhunrowzljqfqrahosheukhahhbiaxqzfmmwcjxountkevsvpbzjnilwpoermxrtlfroqoclexxisrdhvfsindffslyekrzwzqkpeocilatftymodgztjgybtyheqgcpwogdcjlnlesefgvimwbxcbzvaibspdjnrpqtyeilkcspknyylbwndvkffmzuriilxagyerjptbgeqgebiaqnvdubrtxibhvakcyotkfonmseszhczapxdlauexehhaireihxsplgdgmxfvaevrbadbwjbdrkfbbjjkgcztkcbwagtcnrtqryuqixtzhaakjlurnumzyovawrcjiwabuwretmdamfkxrgqgcdgbrdbnugzecbgyxxdqmisaqcyjkqrntxqmdrczxbebemcblftxplafnyoxqimkhcykwamvdsxjezkpgdpvopddptdfbprjustquhlazkjfluxrzopqdstulybnqvyknrchbphcarknnhhovweaqawdyxsqsqahkepluypwrzjegqtdoxfgzdkydeoxvrfhxusrujnmjzqrrlxglcmkiykldbiasnhrjbjekystzilrwkzhontwmehrfsrzfaqrbbxncphbzuuxeteshyrveamjsfiaharkcqxefghgceeixkdgkuboupxnwhnfigpkwnqdvzlydpidcljmflbccarbiegsmweklwngvygbqpescpeichmfidgsjmkvkofvkuehsmkkbocgejoiqcnafvuokelwuqsgkyoekaroptuvekfvmtxtqshcwsztkrzwrpabqrrhnlerxjojemcxel";
    vector<string> words{"dhvf","sind","ffsl","yekr","zwzq","kpeo","cila","tfty","modg","ztjg","ybty","heqg","cpwo","gdcj","lnle","sefg","vimw","bxcb"};
    
    string s2 = "wordgoodgoodgoodbestword";
    vector<string> words2{"word","good","best","word"};
    
    string s3 = "barfoothefoobarman";
    vector<string> words3{"foo","bar"};

    string s4 = "wordgoodgoodgoodbestword";
    vector<string> words4{"word","good","best","good"};

    vector<int> res = Solution().findSubstring2(s3, words3);
    for (int i : res)
    {
        std::cout << i << std::endl;
    }
    std::cout << "total : " << res.size() << std::endl;

}