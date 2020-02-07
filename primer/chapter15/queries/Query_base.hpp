#ifndef QUERY_BASE_HPP
#define QUERY_BASE_HPP

#include <string>

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

class Q_base
{
  void test() { }  
};
#endif // QUERY_BASE_HPP             