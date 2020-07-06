# 内存对齐

### why?
CPU需要花费一个时钟周期读取一个WORD的数据到寄存器中,一个WORD的大小取决于体系结构.
假设在32位体系结构下,一个WORD为4Byte,我们有如下数据:
```c++
data1: "ab"
data2: "cdef"
```
如果没有内存对齐,它将以如下方式组成:
```c++
|a b c d|    |e f 0 0|
```
即data1占据一个单元的一半，data2占据另一半和第二单元的前一半.

在读取data2时，CPU需要两个时钟周期来访问它，先访问第一单元，再访问第二单元的剩下部分。

如果作内存对齐:
```c++
|a b 0 0|     |c d e f|
     ---
   padding
```
仅需要一个时钟周期就能访问data2.
作用:
1. 内存对齐可以提高CPU访问内存中数据的速度。
2. 不对齐则可以节省存储空间。
某些架构上必须作内存对齐。
具体对齐的字节数与体系结构有关, c 中可以指定对齐方式：

```c++
#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

struct MyPackedData
{
    char Data1;
    long Data2;
    char Data3;
};
#pragma pack(pop)   /* restore original alignment from stack */
```

reference：
* https://stackoverflow.com/questions/38875369/what-is-data-alignment-why-and-when-should-i-be-worried-when-typecasting-pointe
* https://en.wikipedia.org/wiki/Data_structure_alignment