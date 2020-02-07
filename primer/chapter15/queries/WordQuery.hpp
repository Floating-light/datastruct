#ifndef WORDQUERY_HPP
#define WORDQUERY_HPP

#include <string>

#include "Query_base.hpp"
#include "TextQuery.h"
#include "QueryResult.h"

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
#endif // WORDQUERY_HPP