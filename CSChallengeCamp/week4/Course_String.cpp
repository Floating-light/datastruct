#include <algorithm>
#include <iostream>
#include <string>
#include <functional>
#include <vector>

using namespace std;

class PatternMatching
{
public:
	// Brute-Force O(n) ~ O(nm)
	// 在大字符集的情况下，BP和KMP性能差不多
	int matchV1(const string& P,const string& T)
	{
		size_t n = T.size(), i = 0;
		size_t m = P.size(), j = 0;
		while (j < m && i < n)
		{
			if (P[j] == T[i]) { i++; j++; }
			else { i = i - j + 1; j = 0; }
		}
		return i - j; // T 和 P的对齐位置
					  // i-j < n-m+1 is valid
	}
	// Brute-Force
	// 更加易于理解
	int matchV2(const string& P,const string& T)
	{
		size_t n = T.size(), i = 0;
		size_t m = P.size(), j = 0;
		for (i = 0; i < n - m + 1; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				if (T[i + j] != P[j]) break;
			}
			if (j == m) break;
		}
		return i; // 失败时i == n - m + 1
	}

	void buildNext(const string& P,vector<int>& next)
	{
		size_t m = P.size(), j = 0;
		next = vector<int>(P.size(), -1);
		int t = -1;
		while (j < m - 1) // 每次循环计算的next[j+1]的值, t总是j的next[j]的值
		{
			// 增量计算
			// (t < 0 || P[j] == P[t]) ? next[++j] = ++t : t = next[t];
			if (t < 0 || P[j] == P[t])
			{
				++j;
				++t;
				// 不重复同样的失败
				// 是从前往后构造的，next也可能经过同样的
				// P: 00001
				// T: 000100001
				// next: -1 0 1 2 3
				// next: -1 -1 -1 -1 3
				next[t] = (P[j] != P[t]) ? t : next[t];
			}
			else
				t = next[t];
		}
	}

	// KMP也指明了算法设计中, 要善于利用执行过程中出现的各种信息（成功的or失败的）
	// KMP O(n)， 小字符集优势更明显
	int matchKMP(const string& P, const string& T)
	{
		vector<int> next;
		buildNext(P, next);

		// size_t 不能和－int比较，int(-1) < size_t(3) == false ?  
		int n = T.size(), i = 0;
		int m = P.size(),j = 0;
		while (i < n && j < m) // watch 2*i - j, 必递增
		{
			if (j < 0 || T[i] == P[j])
			{
				++i; // k +1
				++j;
			}
			else
			{
				// 将已经匹配过的信息（成功）利用起来
				j = next[j]; // next[j] < j
			}
		}
		return i - j;
	}

	vector<int> buildBC(const string& P)
	{
		vector<int> bc(256, -1);
		for (int i = 0; i < P.size(); ++i)
		{
			bc[P[i]] = i;
		}
		return bc;
	}

	vector<int> buildSS(const string& P)
	{
		int m = P.size();
		vector<int> ss(m, -1);
		ss[m - 1] = m;
		for (int lo = m - 1, hi = m - 1, j = lo - 1; j >= 0; j--)
		{
			if ((lo < j) && ss[m - hi + j - 1] <= j - lo)
				ss[j] = ss[m - hi + j - 1];
			else
			{
				hi = j;
				lo = min(lo, hi);
				while ((0 <= lo) && (P[lo] == P[m - hi + lo - 1])) lo--;
				ss[j] = hi - lo;
			}
		}
		return ss;
	}

	vector<int> buildGS(const string& P)
	{
		vector<int>ss = buildSS(P);
		size_t m = P.size();
		vector<int>gs(m, m);
		for (size_t i = 0, j = m - 1; j < UINT_MAX; j--)
		{
			if (j + 1 == ss[j])
				while (i < m - j - 1)
					gs[i++] = m - j - 1;
		}
		for (size_t j = 0; j < m - 1; j++)
		{
			gs[m - ss[j] - 1] = m - j - 1;
		}
		return gs;
	}

	// BC O(n/m) ~ O(n*m) bad character
	// 着重于失配带来的‘教训’
	int matchBC(const string& P, const string& T)
	{
		vector<int> bc = buildBC(P);

		int n = T.size(), i = 0;
		int m = P.size();
		while (n >= i + m)
		{
			int j = m - 1;
			while (P[j] == T[i + j])
				if (0 > --j) break;
			if (0 > j) break; // 成功
			else
			{
				// 不可能等
				if (bc[T[i + j]] > j) i++;
				else i += j - bc[T[i + j]];
			}
		}
		return i;
	}

	// BM = BC + GS(Good suffix) O(m+n) ~ O(n/m)
	int matchBM(const string& P, const string& T)
	{
		vector<int> bc = buildBC(P);
		vector<int> gs = buildGS(P);

		int m = T.size();
		int n = P.size();
		size_t i = 0;
		while(m >= i + n)
		{
			int j = n - 1;
			while (P[j] == T[i + j])
				if (0 > --j) break;
			if (0 > j) break;
			else i += max(gs[j], j - bc[T[i + j]]);
		}
		return i;
	}

};

int main()
{
	PatternMatching pm;
	function<int(PatternMatching, const string&, const string&)> func = &PatternMatching::matchBM;
	int res = func(pm, "345", "1234567890");
	cout << "P = 345, T = 1234567890 , restul: " << res << endl;
	// int neg = -1;
	// size_t st = 3;
	// cout << (st > neg) << endl; // false
}