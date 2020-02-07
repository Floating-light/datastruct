#ifndef NOTQUERY_HPP
#define NOTQUERY_HPP

#include <string>

#include "Query_base.hpp"
#include "TextQuery.h"
#include "QueryResult.h"
#include "Query.hpp"

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

#endif // NOTQUERY_HPP