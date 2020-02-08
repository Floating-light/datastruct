### 目录
* [前情提要](#前情提要)
* [15.9 Text Queries Revisited](#159-Text-Queries-Revisited)
    * [15.9.1 An Object-Oriented Solution](#1591-an-object-oriented-solution)
        * [Abstract Base Class](#abstract-base-class)
        * [Hiding a Hierarchy in a Interface Class](#hiding-a-hierarchy-in-a-interface-class)
    * [15.9.2 The Query_base and Query Class](#1592-the-query_base-and-query-class)
    * [15.9.3 The Derived Class](#1593-the-derived-class)
        * [The WordQuery Class](#the-wordquery-class)
        * [The NotQuery Class and the ~ Operator](#The-NotQuery-Class-and-the-~-Operator)
        * [The BinaryQuery Class](#The-BinaryQuery-Class)
        * [The AndQuery and OrQuery Classes and Associated Operators](#The-AndQuery-and-OrQuery-Classes-and-Associated-Operators)
        * [15.9.4 The eval Functions](#1594-The-eval-Functions)
### 前情提要
在`第12章`利用标准和共享指针实现了一个简单的文本查询程序，其实现的主要功能是，读取一个文本，实现根据输入的单词，查询这个单词出现的次数以及出现的每一行的文本。

由`TextQuery`读入文件，并提供单个单词的查询。其中，文本内容按行保存在`std::shared_ptr<std::vector<std::string>>`数组中，`std::map<std::string, std::shared_ptr<std::set<line_no>>>`保存所有单词出现的行号，查询时可以通过map快速而又方便地查询。

查询结果由`QueryResult`类表示，它保存了表示一个完整的结果的所有信息，而不与特定`TextQuery`关联。其中`sought`表示是由这个单词查得的结果，其对应出现的所有行号`lines`是从`TextQuery`中的`word-->set`映射map中查得的`std::shared_ptr<std::set<line_no>>`。同时也保存了整个文本`std::shared_ptr<std::vector<std::string>> file`。使用`shared_ptr`，使得查询过程中没有新的数据的内存开销，仅仅是`shared_ptr`本身的`copy`，`construct`开销。同时为`QueryResult`类定义了一个friend函数，可以格式化打印出`QueryResult`表示的结果。

有了以上两个类，可以方便地实现单个单词的查询和显示。这与我之前通常的设计想法截然不同。通常可能会直接在一个类里面读入文本，然后调用查询函数，返回查询结果等等。但是这样将查询文本的数据主体和结果的表示分离，使得程序更为优美，后续的扩展中也更加方便。在多次查询中不会出现结果的错乱，在对多个文本的查询时，都能用同一个`QueryResult`来表示结果，易于扩展，并且多个结果可以直接保存在一些QueryResult对象中，且可以同时存在而没有额外开销。(瞎比比完了)

`TextQuery`的职责仅仅是保存基本的文本信息，方便查询（vector保存数据，map保存方便查询的数据）。而`QueryResult`是要保存查询的结果。所有与它们职责相关的都会成为它们的成员。其中用到了`shared_ptr`，方便内存的管理，也减少了数据的拷贝。成功践行了`单一职责原则`,即使后面需要将`QueryResult`打印出来，也仅仅是定义了一个`friend`函数来打印，保证了`QueryResult`的干净。本身打印这一功能就是和它无关的，将来`QueryResult`还可以会有打印之外的用途，所以没必要把它和打印绑定在一起。

# 15.9 Text Queries Revisited

扩展上述文本查询程序，使其能够支持与或非的查询，也就是：
```
fiery & bird | wind
```
这样的查询。其中包括各种各样的|、＆、～操作的组合。

## 15.9.1 An Object-Oriented Solution
通常我们会想直接使用`TextQuery`来表示单词查询，然后从它派生其他查询类别。

考虑一个`非`查询，一个单词查询是查询某一个词，如果要使`非`查询成为单词查询的一种，我们则必须找到`非`查询的那个单词，通常来说这个单词是不存在的，所以它无法成为单词查询的一种，相反它是对某个查询结果(单词查询或其他任意组合的查询)的取反。与查询和或查询是两个单词查询结果的组合。

由此我们可以知道，这四种类型的查询之间更像是组合关系，而不是某个查询的派生。我们可以定义出四种查询：
```
WordQuery // Daddy
NotQuery  // ~Alice
OrQuery   // hair | Alice
AndQuery  // hair & Alice
```
这些类只需要定义两种操作：
* `eval`, 有一个`TextQuery`对象作为参数，返回`QueryResult`。eval函数使用这个`TextQuery`对象找到这个查询匹配的行。

* `rep`,返回表示查询底层查询表达式的字符串。这个函数被eval用于创建`QueryResult`，以及用于输出操作符打印查询表达式。

### Abstract Base Class

如上所述，我们定义的四个不是继承关系，更像是概念上的兄弟关系。因此我们可以定义一个抽象基类来表示共同的接口：

```c++
class Query_base
{
    friend class Query;
protected:
    using line_no = TextQuery::line_no; // used in the eval function
    virtual ~Query_base() = default;
private:
    // eval return the QueryResult that matches this Query
    virtual  QueryResult eval(const TextQuery&) const = 0;

    // rep is a string representation of the query
    virtual std::string rep() const = 0;
};
```

由于在`eval`函数中要创建`QueryResult`，会使用到`TextQuery::line_no`,所以使用`using`使得这个`name`在`Query_base`的范围（包括子类）都可见。

### Hiding a Hierarchy in a Interface Class

我们的程序是计算查询结果，而不是建立它们。但是为了使程序跑起来又不得不创建各种复合查询。最简单的方式就是写c++表达式创建这些查询。例如：

```c++
Query q = Query("fiery") & Query("bird") | Query("wind");
```

这样一来，用户层的代码就没必要直接使用继承层次中的类。相反，我们要定义一个接口类`Query`,由它来操作具体的执行查询的对象的创建。它将会有一个指向`Query_base`类的指针，并提供和`Query_base`类一样的操作`eval()`和`rep()`,`Query`对象只是简单地调用它的`Query_base`类的对应方法，重点是根据不同的操作符创建不同的查询对象（Query_base的派生类）。

其中，直接通过一个代表要查询的单词的`string`创建`WordQuery`，其他三个类型的查询则通过作用于`Query`的操作符～，|，&的不同来创建。也就是说，不论什么类型的查询，都是从WordQuery开始的，然后通过不同的操作符再创建`NotQuery`， `OrQuery`， `AndQuery`。

## 15.9.2 The Query_base and Query Class

`Query`类提供对`Query_base`继承结构体系的使用接口，根据不同的操作，创建不同的`Query_base`对象。为从最开始的单词查询创建`WordQuery`对象，它需要一个以`string`为参数的构造器。此外为根据操作符创建复合查询对象，还需要一个以Query_base指针为参数的构造器，来构造新的对象。

```c++
class Query
{
    // these operators need access to the shared_ptr constructor
    friend Query operator~(const Query&);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);

public:
    Query(const std::string&); // build a new WordQuery

    // interface functions: call the corresponding Query_base operations
    QueryResult eval(const TextQuery& t) const { return q->eval(t); }

    std::string rep() const { return q->rep(); }
private:
    Query(std::shared_ptr<Query_base> query) : q(query) { }
    std::shared_ptr<Query_base> q;
};

inline std::ostream& operator<<(std::ostream& os, const Query& query)
{
    // Query::rep makes a virtual call through its Query_base pointer to rep()
    return os << query.rep();                
}
```

此外还定义了一个输出流操作符，用于输出Query的查询表达式。

## 15.9.3 The Derived Class

### The WordQuery Class

`WordQuery`类执行的是单个单词的查询，所以`eval`函数直接调用`TextQuery`的查询函数就可以了，此外`Query`类在定义了`WordQuery`类之后就可以定义它的构造函数了，在传入要查询的`string`单词后，创建一个`WordQuery`对象，并保存在`Query_base`类型指针的成员变量上。
```c++
class WordQuery: public Query_base
{
    friend class Query; // Query uses the WordQuery constructor
    WordQuery(const std::string& s): query_word(s) { }

    // concrete class: WordQUery defines all inherited pure virtual functions
    QueryResult eval(const TextQuery& t) const
    {
        return t.query(query_word);
    }

    std::string rep() const { return query_word; }

    std::string query_word; // word for which to search
};

// 直接创建WordQuery对象， 保存到成员变量中。
inline Query::Query(const std::string& s) : q(new WordQuery(s)) { }
```

### The NotQuery Class and the ~ Operator

`NotQuery`是在`～`作用于`Query`时创建的，所以它需要使得`Query`的`～`操作符为它的`friend`，以方便这个操作符调用`NotQuery`的构造函数。

```c++
class NotQuery: public Query_base
{
    friend Query operator~(const Query& );
    NotQuery(const Query& q) : query(q) { }

    // concrete class: NotQuery defines all inherited pure virtual functions
    std::string rep() const { return "~(" + query.rep() + ")"; }
    QueryResult eval( const TextQuery& ) const; // TODO:

    Query query; // 保存着它原来的Query,通常是WordQuery,也可能是别的复合查询
};

// 仅能通过～操作符构建NotQuery查询对象
inline Query operator~(const Query& operand)
{
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}
```

### The BinaryQuery Class

`BinaryQuery`也是一个抽象基类，因为`AndQUery`和`OrQuery`不同于`NotQuery`或`WordQuery`，前者需要有两个操作数，除此之外，还有一个具体的操作符，这由它的子类在构造的时候指定。

```c++
class BinaryQuery : public Query_base
{
protected:
    BinaryQuery(const Query&l , const Query& r, std::string s) : 
        lhs(l), rhs(r), opSym(s) { }

    // abstract class: BinaryQuery doesn't define eval
    std::string rep() const { return "(" + lhs.rep() + " "
                                         + opSym + " "
                                         + rhs.rep() + ")"; }
    
    // inherits a pure virtual funciton eval(),
    // so BinaryQuery is also an abstract base class.

    Query lhs, rhs;    // right- and left-hand operands
    std::string opSym; // name of the operator
};
```

### The AndQuery and OrQuery Classes and Associated Operators

`AndQuery`和`OrQuery`则分别在构造时调用父类构造器传入两个`Query`和对应的操作符`&` 和`|`。同时，还在此定义创建自己的`Query`的操作符。

```c++
class OrQuery : public BinaryQuery
{
    friend Query operator|(const Query&, const Query&);
    OrQuery(const Query&l, const Query& r): BinaryQuery(l, r, "|") { }

    QueryResult eval(const TextQuery& ) const;
};

// 必须inline, 否则会报错：
// multiple definition of `operator|(Query const&, Query const&)'; Query.o:Query.cpp:(.text+0xb6): first defined here
inline Query operator|(const Query& l, const Query&r)
{
    return std::shared_ptr<Query_base>(new OrQuery(l, r));
}

class AndQuery : public BinaryQuery
{
    friend Query operator&(const Query&, const Query&);
    AndQuery(const Query& left, const Query& right) :
        BinaryQuery(left, right, "&") { } 
    // concrete class: AndQuery inherits rep and defines the 
    // remaining pure virtual
    QueryResult eval(const TextQuery&) const;
};

inline Query operator&(const Query& l, const Query& r)
{
    return std::shared_ptr<Query_base>(new AndQuery(l,r));
}
```
# 15.9.4 The eval Functions

上一节定义的`QUery_base`的派生类除了`WordQuery`（这个很简单），都还没定义`eval()`函数，这才是关键所在，剩下这三个`eval()`主要就是根据已有的`Query`成员变量，读取它们的查询结果，再根据自身的查询逻辑，合成一个新的结果，其中这些Query均可能都是复合的。

当我们在进行(&|~)等复合查询操作时，正真的查询结果的操作并没有进行，只是调用了对应的`operator`构建的新的`Query`对象，直到我们调用了某个`Query`的`eval()`时，与其相关的一系列`eval()`才会调用。

```c++
QueryResult NotQuery::eval(const TextQuery& text ) const
{
    QueryResult result = query.eval( text);

    auto ret_lines = std::make_shared<std::set<line_no>>();

    auto beg = result.begin(), end = result.end();
    auto sz = result.get_file()->size();

    // set 中的元素有序
    for(size_t i = 0; i != sz; ++i)
    {
        if(beg == end || *beg != i)
            ret_lines->insert(i);
        else if( beg != end)
            ++beg;
    }
    return QueryResult(rep(), ret_lines, result.get_file());
}

// returns the union of its operands' result sets
QueryResult OrQuery::eval(const TextQuery& text) const
{
    // virtual calls through the Query members, lhs and rhs
    // the calls to eval return the QueryResult for each operand
    QueryResult right = rhs.eval(text), left = lhs.eval(text);

    // copy the line numbers from the left-hand operand into the result set
    auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());

    // insert lines from the right-hand operand
    ret_lines->insert(right.begin(), right.end());

    // return the new QueryResult representing the union of lhs and rhs
    return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text) const
{
    QueryResult left = lhs.eval(text), right = rhs.eval(text);
    auto ret_lines = std::make_shared<std::set<line_no>>();

    // 两集合求交集
    std::set_intersection(left.begin(), left.end(), 
                        right.begin(), right.end(), 
                        std::inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}
```