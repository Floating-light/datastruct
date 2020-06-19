# 可变参数模板
* `可变参数模板(variadic template)`为一个接受可变数目参数的模板函数或模板类。
* `参数包(parameter packet)`可变数目的参数。
* `模板参数包(template parameter packet)`表示零个或多个模板参数。
* `函数参数包(function parameter packet)`表示零个或多个函数参数。

用省略号指出一个`模板参数`或`函数参数`表示一个包。
* 用`class...`或`typename...`指出接下来的参数表示零个或多个类型的列表。
* 一个类型名后面跟一个省略号表示零个或多个给定类型的`非类型参数`的列表(可以是一个函数的实参列表).

```c++
// 用class 也一样
template <typename T, typename... Args> 
// 如果函数参数列表中一个参数的类型是一个模板参数包,
// 则此参数也是一个函数参数包
void func(const T& t, const Args&... rest);
```
编译器从函数实参推断模板参数类型.对可变参数模板,编译器还会推断包中的参数数目.用`sizeof...()`可以获取模板参数包的参数个数和函数参数包的参数个数.
```c++
template <typename T, typename... Args>
void foo(const T& t, const Args&... rest)
{
    std::cout << sizeof...(Args) << std::endl;
    std::cout << sizeof...(rest) << std::endl;
}
int main()
{
    int i = 0;
    double d = 3.14;
    string s = "haha";
    foo(i, s, 42, d); // 3 ,3 
    foo(s, 42, "hoi"); // 2, 2
    foo(d,s); // 1, 1
    foo("hihihi"); // 0, 0
    foo(d,"asdf", "asdfasdf"); // 2, 2
    // 这五种调用会实例化出五个不同版本的foo().
}
```
## 可变参数函数的递归调用
使用`initializer_list`可以定义一个接受可变数目实参的函数,但这些参数必须具有同一类型。

当我们既不知道实参数目也不知道类型时就可以用可变参数模板函数.
```c++
// 用于终止递归, 并打印最后一个元素
// 必须在可变参数版本之前声明
// 若在之后声明, 则无法通过编译, 
// note:   candidate expects at least 2 arguments, 1 provided
template<typename T>
ostream &print(ostream& os, const T &t)
{
    return os << t << std::endl;
}
// 可变参数函数通常是递归的,每一步处理最前面的一个实参, 然后用剩余参数调用自身.
// 除了最后一个元素之外的其他元素都会调用这个版本的print
template <typename T, typename ... Args>
ostream &print(ostream& os, const T& t,const Args&... rest)
// const Args&.. rest
// 表示将const Args& 应用到包中的每一个元素, 其扩展结果为一个逗号分隔的零个或多个类型的列表，
// 每个类型都形如const type&
// print(std::cout, 12, 13.3, "asfdae");
// 实例化为 print(ostream&, const int&, const double&, const string&)
{
    os << t << ", ";
    return print(os, rest...);
    // 模式是函数参数包的名字 rest.
    // 扩展为一个由由包中元素组成的、逗号分隔的列表.
    // print(os, 13.3, "asfdae");
}

int main()
{
    print(std::cout, 12, 13.3, "asfdae"); // 调用可变参版本
    print(std::cout, 13.3, "asfdae"); // 调用可变参版本
    print(std::cout, "asfdae"); // 两个print版本的参数都是匹配的，但是非可变参数模板比可变参数
                                // 模板更特例化, 因此编译器选择非可变参数版本
}
```
1. 非可变参版本的声明必须在可变参函数之前,否则,可变参版本的`print`将会一直调用自身,直到耗尽参数包,用一个`os`参数去调用`print`(无法通过编译).
2. 对一个没有`<<`运算符的类型的对象调用`print`，编译都无法通过,从模板实例化出的对应版本的函数终究会用这个参数调用`os << myclass`.
3. 对于`print(std::cout, "asfdae")`,两个版本的`print`都提供同样好的参数匹配, 但是非可变参数模板比可变参数模板更加特例化, 因此编译器选择非可变参数版本(16.3 P615).

## 包扩展
对于一个`参数包`(上面的rest),我们除了获取其大小外,唯一能做的就是扩展它(`expand`).扩展包就是对包中的每一个元素都应用一个指定的模式,并得到展开后的逗号分隔的列表, 这里的`模式`通常为一些类型限定修饰符.通过在模式右边放一个省略号(...)触发扩展操作。

* `const Args&... rest`扩展模板参数包,将`const type&`应用到包中的每一个元素,为`print`生成参数列表。

```c++
print(cout, 2, 3.14, "asd");// 包中有两个参数
```
此调用被实例化为
```c++
ostream& print(ostream&, const int&, const double&, const char[4]&);
```
在`print`函数的`return`语句中的递归调用也触发了扩展(有...出现),只是它直接将`rest`扩展为逗号分隔的参数列表, 而没有改变各个元素的类型。
考虑更清晰的一个例子,有这样一个函数: 
```c++
template <typename T>
string debug_rep(const T& t)
{
    ostringstream ss;
    ss << t;
    return ss.str();
}
```
它通过流操作符从传进来的参数获取一个字符串.
```c++
template <typename... Args>
ostream &errorMsg(ostream&os, const Args&... rest)
{
    return print(os, debug_rep(rest)...);
}
```
`errorMsg`将传进来的参数包`rest`用`debug_rep`扩展
```c++
errorMsg(cerr, fcnName, code.num(), otherData, "balabala", item);
```
扩展结果:
```c++
print(cerr, debug_rep(fcnName), debug_rep(code.num(),
            debug_rep(otherData), debug_rep("balabala"),
            debug_rep(item)));
```
相对的:
```c++
print(os, debug_rep(rest...));
```
展开为将包传递给了debug_rep:`print(os, debug_rep(a1,a2,a3...an))`，所以这个调用会失败,`debug_rep`没有匹配的参数列表。
## 转发参数包
可变参数模板一个更为常见的使用场景是用可变参数实例化一个类,通常一个类有多个不同参数列表的构造函数,这就要求我们必须将不同数量不同类型的参数原封不动地传递给这个类的构造函数。

标准库中的容器通常有个`emplace_back()`成员, 它是一个可变参数模板成员函数,它直接在容器管理的内存空间中用所给参数直接构造一个元素。

保持类型信息是一个两阶段的过程。

* 保持实参中的类型信息——将`emplace_back()`的参数定义为模板类型参数的右值引用。
* 将这些参数传递给`construct`时，用`forward`保持实参原始类型。

考虑标准库中的`vector`的`emplace_back()`实现:
```c++
    template <class... _Valty>
    decltype(auto) emplace_back(_Valty&&... _Val) {
        // insert by perfectly forwarding into element at end, provide strong guarantee
        auto& _My_data   = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;
        if (_Mylast != _My_data._Myend) {
            return _Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);
        }

        _Ty& _Result = *_Emplace_reallocate(_Mylast, _STD forward<_Valty>(_Val)...);
#if _HAS_CXX17
        return _Result;
#else // ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv
        (void) _Result;
#endif // _HAS_CXX17
    }
```
暂时不考虑那么多细枝末节,仅专注于对可变参数的处理, 首先,其参数`emplace_back(_Valty&&... _Val)`用右值引用扩展了参数包中的所有参数,
在传递参数包`_Val`时，用`forward<_Valty>(_Val)...`同时既扩展了模板参数包`_Valty`也扩展了函数参数包`_Val`,最终会通过`allocator::construct()`调用容器的元素类型的构造函数.