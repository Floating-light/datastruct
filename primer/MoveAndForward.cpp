#include <iostream>
#include <type_traits>
#include <string>
#include <vector>

using namespace std;

// 移除引用

// template <class _Ty>
// struct remove_reference {
//     using type                 = _Ty;
//     using _Const_thru_ref_type = const _Ty;
// };

// template <class _Ty>
// struct remove_reference<_Ty&> {
//     using type                 = _Ty;
//     using _Const_thru_ref_type = const _Ty&;
// };

// template <class _Ty>
// struct remove_reference<_Ty&&> {
//     using type                 = _Ty;
//     using _Const_thru_ref_type = const _Ty&&;
// };

template<typename T>
std::remove_reference_t<T>&& Mymove(T&& param)
{
    using ReturnType = std::remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}

int main()
{
    int i = 0;
    int& ri = i;
    std::move(ri);
    std::vector<int&> v;
    std::vector<const int &> vv;
    std::string s;
    std::forward<string>(s);
}