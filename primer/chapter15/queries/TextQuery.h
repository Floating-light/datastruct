#ifndef TEXTQUERY_H
#define TEXTQUERY_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include "QueryResult.h"

class QueryResult; // declaration needed for return type in the query function
class TextQuery {
public:
#ifdef TYPE_ALIAS_DECLS
	using line_no = std::vector<std::string>::size_type;
#else
	typedef std::vector<std::string>::size_type line_no;
#endif
	TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const; 
    void display_map();        // debugging aid: print the map
private:
    std::shared_ptr<std::vector<std::string>> file; // input file
    // maps each word to the set of the lines in which that word appears
    std::map<std::string, 
	         std::shared_ptr<std::set<line_no>>> wm;  

	// canonicalizes text: removes punctuation and makes everything lower case
    static std::string cleanup_str(const std::string&);
};

inline
std::string make_plural(size_t ctr, const std::string &word, 
                               const std::string &ending)
{
	return (ctr > 1) ? word + ending : word;
}

#endif
