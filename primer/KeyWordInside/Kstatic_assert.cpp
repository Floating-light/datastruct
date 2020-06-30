// c++中想要在编译时期进行断言，在之前的标准中可以采用1/0来判断，如下：

#include <cstring>
using namespace std;

#define assert_static(e) do{ enum{ assert_static__ = 1 / (e) }; } while(0)

template<typename T, typename U>
int bit_copy_(T& a, U& b) {
 assert_static(sizeof(a) == sizeof(b));
 memcpy(&a, &b, sizeof(b));          
}

// 在c++11中，可以使用static_assert断言，且可以打印出具体的出错信息。static_assert接收两个参数，
// 一个是断言表达式，此表达式需要返回一个bool值；另一个则是警告信息，通常是字符串。以上代码可以修改如下：

 template<typename T, typename U>
  int bit_copy(T& a, U& b) {
   static_assert(sizeof(a) == sizeof(b), "the parameters of bit_copy must have same width");
   memcpy(&a, &b, sizeof(b));          
 }
// 编译会得到如下信息：

// error:static assertion failed: "the parameters of bit_copy must have same width."

// reference: https://www.cnblogs.com/sssblog/p/10178415.html

int main() {
  int a = 0x2468;
  double b;
  bit_copy(a, b);   
  //----------------------------------------------------------------------
  int a = 0x2468;
  int aa = 10;
  const int& cc = a;
  int& cccc = a; // 引用一旦被初始化就无法修改它指向的对象, 永远也不可能让它指向a之外的变量.
  cccc = aa;

  const int* p1 = &a;
  p1=&aa; 
  // *p1 = 3; // 表达式必须是可修改的左值

  int *const p2 = &aa;
  // p2 = &a; // 表达式必须是可修改的左值
  *p2 = aa;

  int const*p3 = &a; // <==> const int* 
  p3 = &aa;
  // *p3 = aa; // 表达式必须是可修改的左值
}
