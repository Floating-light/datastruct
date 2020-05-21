// 60. Permutation Sequence 
// tags: Math, Backtracking
#include <algorithm>
#include <iostream>
#include <sstream>
#include <list>
#include <vector>

using namespace std;

class Solution2 {
public:
    string getPermutation(int n, int k) {
        string s = "123456789";
        s = s.substr(0,n);
        for(int j = 1; j < k; ++j)
        {
            next_permutation(s.begin(), s.end());
        }      
        return s;
    }
};

class Solution {
    // int fac(int n)
    // {
    //     int res = 1;
    //     while(n >1)
    //     {
    //         res*=n;
    //         --n;
    //     }
    //     return res;
    // }
    static const vector<int> fac;
public:
    string getPermutation(int n, int k) {
        string res;
        string s = string("123456789").substr(0, n);
        --k;
        while(k > 0)
        {
            size_t i = k/fac[n - 1];
            res.push_back(s[i]);
            s.erase(s.begin() + i);
            k %= fac[n - 1];
            --n;
        }
        return res + s;
    }
};
const vector<int> Solution::fac = {0,1,2,6,24,120,720,5040,40320,362880,3628800};

int main()

{
    std::cout << Solution().getPermutation(5,97) << std::endl;
    //Solution().getPermutation(5,120);
    // int n = 10;
    // stringstream ss("{");
    // int fac = 1;
    // ss << "0,";
    // for(int i = 1; i <= n; ++i)
    // {
    //     fac *= i;
    //     ss << fac << "," ;
    // }
    // ss << "}";
    // std::cout << ss.str() << std::endl;

}
//12345
/*
1. 12345
2. 12354
3. 12435
4. 12453
5. 12534
6. 12543
7. 13245
8. 13254
9. 13425
10. 13452
11. 13524
12. 13542
13. 14235
14. 14253
15. 14325
16. 14352
17. 14523
18. 14532
19. 15234
20. 15243
21. 15324
22. 15342
23. 15423
24. 15432
25. 21345
26. 21354
27. 21435
28. 21453
29. 21534
30. 21543
31. 23145
32. 23154
33. 23415
34. 23451
35. 23514
36. 23541
37. 24135
38. 24153
39. 24315
40. 24351
41. 24513
42. 24531
43. 25134
44. 25143
45. 25314
46. 25341
47. 25413
48. 25431
49. 31245
50. 31254
51. 31425
52. 31452
53. 31524
54. 31542
55. 32145
56. 32154
57. 32415
58. 32451
59. 32514
60. 32541
61. 34125
62. 34152
63. 34215
64. 34251
65. 34512
66. 34521
67. 35124
68. 35142
69. 35214
70. 35241
71. 35412
72. 35421
73. 41235
74. 41253
75. 41325
76. 41352
77. 41523
78. 41532
79. 42135
80. 42153
81. 42315
82. 42351
83. 42513
84. 42531
85. 43125
86. 43152
87. 43215
88. 43251
89. 43512
90. 43521
91. 45123
92. 45132
93. 45213
94. 45231
95. 45312
96. 45321
97. 51234
98. 51243
99. 51324
100. 51342
101. 51423
102. 51432
103. 52134
104. 52143
105. 52314
106. 52341
107. 52413
108. 52431
109. 53124
110. 53142
111. 53214
112. 53241
113. 53412
114. 53421
115. 54123
116. 54132
117. 54213
118. 54231
119. 54312
120. 54321
*/