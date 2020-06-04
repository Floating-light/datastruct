# c++ reference

## Algorithm
`algorithm`定义了一系列的作用于一定范围的元素的函数.
### Merge
对排过序的两个`ranges`的操作。
#### set_intersection

求两个集合的交集。即`result`中仅包含在两个集合中都出现过的元素。在默认`Compare`下，仅用`<`来判断两元素是否相等。这里的`Compare`操作应该和集合中排序的方式相同。
```c++
 OutputIterator set_intersection (InputIterator1 first1, InputIterator1 last1,
                                   InputIterator2 first2, InputIterator2 last2,
                                   OutputIterator result, Compare comp);
```
测试例子:
```c++
int main()
{
    std::vector<int> set1 = { 4,9,5,9 };
    std::vector<int> set2 = { 9,4,9,8,4,3,2,1 };
    
    sort(set1.begin(), set1.end(), std::greater<int>()); // 9, 9, 5, 4
    sort(set2.begin(), set2.end(), std::greater<int>()); // 9, 9, 8, 4, 4, 3, 2, 1

    auto itr = std::set_intersection(set1.begin(), set1.end(),
        set2.begin(), set2.end(),
        set1.begin(), std::greater<int>());

    //set1.erase(itr, set1.end());
    set1.resize(itr - set1.begin());
    auto itr2 = begin(set1);
    while (itr2 != end(set1))
    {
        std::cout << *itr2 << ", ";
        ++itr2;
    }
    std::cout << std::endl;
    // cout： 9， 9， 4
}
```
因为在找到相交的元素时，`result`的位置一定是小于等于当前迭代的位置, 所以`Outputiterator`可以是输入迭代器中的一个,然后根据返回的下一个迭代器的位置，用`resize`或`erase`去掉多余的元素。

#### set_difference
求集合`[first1, last1)`相对于集合`[first2, last2)`的补集，即出现在第一个集合中而不存在于第二个集合的元素`CuA`，相当于第一个集合是全集.对于第二个集合中超出第一个集合（全集）的元素不作处理,直接忽视。
```c++
int main()
{
    std::vector<int> set1 = { 4,9,5,9 };
    std::vector<int> set2 = { 9,4,9,8,4,3,2,1 };
    
    sort(set1.begin(), set1.end(), std::greater<int>()); // 9, 9, 5, 4
    sort(set2.begin(), set2.end(), std::greater<int>()); // 9, 9, 8, 4, 4, 3, 2, 1

    std::vector<int> res;
    auto itr = std::set_difference(set2.begin(), set2.end(),
        set1.begin(), set1.end(),
        std::back_inserter(res), std::greater<int>());
    // 8, 4, 3, 2, 1
}
```
其中`set1`中的元素`5`超出了全集`set2`的范围, 直接忽略它。

这里由于`set_difference`实现的原因，`Outputiterator`不能是输入的迭代器。

#### set_symmetric_difference
对等差分,即存在于两个集合的并集但不存在于交集的所有元素。
```c++
int main()
{
    int set1[] = { 4,9,5,9 };
    int set2[] = { 9,4,9,8,4,3,2,1 };
    
    sort(set1, set1+4, std::greater<int>()); // 9, 9, 5, 4
    sort(set2, set2 + 8, std::greater<int>()); // 9, 9, 8, 4, 4, 3, 2, 1

    std::vector<int> res;

    auto itr = std::set_symmetric_difference(set2, set2 + 8,
        set1, set1 + 4,
        std::back_inserter(res), std::greater<int>());
    // 8, 5, 4, 3, 2, 1,
}
```

#### set_union
求两集合的并集,同时存在于两个集合的元素只会出现一次。
```c++
int main()
{
    int set1[] = { 4,9,5,9 };
    int set2[] = { 9,4,9,8,4,3,2,1 };
    
    sort(set1, set1+4, std::greater<int>()); // 9, 9, 5, 4
    sort(set2, set2 + 8, std::greater<int>()); // 9, 9, 8, 4, 4, 3, 2, 1

    std::vector<int> res;
    auto itr = std::set_union(set2, set2 + 8,
        set1, set1 + 4,
        std::back_inserter(res), std::greater<int>());
    // 9, 9, 8, 5, 4, 4, 3, 2, 1,
}
```
#### includes
先对两个范围内的元素排序，`includes`检测是否第一个范围内的元素包含了所有第二个范围内的元素，没有`Outputiterator`。

#### inplace_merge

#### merge
