#include "Query.hpp"
#include "TextQuery.h"

#include <set>
#include <algorithm>

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