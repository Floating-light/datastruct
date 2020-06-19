# 散列表

## hash(key)

* 设计原则

#### 除余法(divisioin method)

#### multiply and divide method

#### 数字分析法(selecting digits)

从key的特定进制的展开中抽取出特定的若干位，构成一个整型地址。如，取十进制展开中的奇数位：

    hash(123456789) = 13579

#### 平方取中(mid-square)
从key的平方的十进制或二进制展开中取居中的若干位构成一个整型地址如，取平方后十进制展开居中的三位:

    hash(123) = 15129 = 512
    hash(1234567) = 1524155677489

#### 折叠(folding)

将key的十进制或二进制展开分割成等宽的若干段，取其总和作为散列地址。如，以三个数位为分割单位：

    hash(123456789) = 123 + 456 + 789 = 1368

或

    hash(123456789) = 123 + 654 + 789 = 1566

#### 位异或(xor)

将key的二进制展开分割成等宽的若干段，经异或运算得到散列地址。如，仍以三个数位为分割单位：

    hash(411) = hash(110011011) = 110 ^ 011 ^ 011 = 110 = 6
或

    hash(411) = hash(110011011) = 110 ^ 110 ^ 011 = 110 = 3

同时，为保证hash()函数取值合法性，通常上述运算结果还需要对散列表长度M再做一次取余运算。

#### (伪)随机数法

上述各具特点的散列函数，验证了其核心设计思想：越是随机、越是没有规律，就越是好的散列函数。

如此一来，任何一个伪随机数发生器就是一个好的散列函数：

    rand(key) / M

但是，由于不同的计算环境所提供的伪随机数发生器不尽相同，故将某一系统中生成的散列表移植到另一系统时，必须格外小心。