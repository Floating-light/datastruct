#include <iostream>
#include <string>
#include <functional>
#include <vector>

using namespace std;

class PatternMatching
{
public:
	// Brute-Force O(n) ~ O(nm)
	// �ڴ��ַ���������£�BP��KMP���ܲ��
	int matchV1(const string& P,const string& T)
	{
		size_t n = T.size(), i = 0;
		size_t m = P.size(), j = 0;
		while (j < m && i < n)
		{
			if (P[j] == T[i]) { i++; j++; }
			else { i = i - j + 1; j = 0; }
		}
		return i - j; // T �� P�Ķ���λ��
					  // i-j < n-m+1 is valid
	}
	// Brute-Force
	// �����������
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
		return i; // ʧ��ʱi == n - m + 1
	}

	void buildNext(const string& P,vector<int>& next)
	{
		size_t m = P.size(), j = 0;
		next = vector<int>(P.size(), -1);
		int t = -1;
		while (j < m - 1) // ÿ��ѭ�������next[j+1]��ֵ, t����j��next[j]��ֵ
		{
			// ��������
			// (t < 0 || P[j] == P[t]) ? next[++j] = ++t : t = next[t];
			if (t < 0 || P[j] == P[t])
			{
				++j;
				++t;
				// ���ظ�ͬ����ʧ��
				// �Ǵ�ǰ������ģ�nextҲ���ܾ���ͬ����
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

	// KMPҲָ�����㷨�����, Ҫ��������ִ�й����г��ֵĸ�����Ϣ���ɹ���orʧ�ܵģ�
	// KMP O(n)�� С�ַ������Ƹ�����
	int matchKMP(const string& P, const string& T)
	{
		vector<int> next;
		buildNext(P, next);

		// size_t ���ܺͣ�int�Ƚϣ�int(-1) < size_t(3) == false ?  
		int n = T.size(), i = 0;
		int m = P.size(),j = 0;
		while (i < n && j < m) // watch 2*i - j, �ص���
		{
			if (j < 0 || T[i] == P[j])
			{
				++i; // k +1
				++j;
			}
			else
			{
				// ���Ѿ�ƥ�������Ϣ���ɹ�����������
				j = next[j]; // next[j] < j
			}
		}
		return i - j;
	}

	// BC O(n/m) ~ O(n*m)
	// ������ʧ������ġ���ѵ��
	int matchBC(const string& P, const string& T)
	{

	}
};

int main()
{
	PatternMatching pm;
	function<int(PatternMatching, const string&, const string&)> func = &PatternMatching::matchKMP;
	int res = func(pm, "345", "1234567890");
	cout << "P = 345, T = 1234567890 , restul: " << res << endl;
	// int neg = -1;
	// size_t st = 3;
	// cout << (st > neg) << endl; // false
}