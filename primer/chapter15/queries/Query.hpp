#ifndef QUERY_HPP
#define QUERY_HPP

#include <string>
#include <memory>
#include <iostream>

#include "TextQuery.h"

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

inline Query::Query(const std::string& s) : q(new WordQuery(s)) { }

class NotQuery: public Query_base
{
    friend Query operator~(const Query& );
    NotQuery(const Query& q) : query(q) { }

    // concrete class: NotQuery defines all inherited pure virtual functions
    std::string rep() const { return "~(" + query.rep() + ")"; }
    QueryResult eval( const TextQuery& ) const; // TODO:

    Query query;
};

// 仅能通过～操作符构建NotQuery查询对象
inline Query operator~(const Query& operand)
{
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}

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

#endif // QUERY_HPP
