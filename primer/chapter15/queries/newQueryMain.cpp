#include "Query.hpp"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void runQueries(ifstream& infile)
{
    TextQuery tq(infile);
}

int main(int argc, char **argv)
{
    ifstream infile;
    if(argc < 2 || !(infile.open(argv[1]), infile))
    {
        cerr << "No input file!" << endl;
        if(argc > 1)
            cerr << "Get : " << argv[1] << endl;
        return EXIT_FAILURE;
    }
    TextQuery tq(infile);
    Query qHer("her");
    QueryResult qr = qHer.eval(tq);

    print(cout, qHer.eval(tq));
    
    Query qDady("Daddy");

    Query andQ = (qDady&qHer);
    
    print(cout, (qDady&qHer).eval(tq));

    print(cout, (qDady|qHer).eval(tq));

    print(cout, (~qHer).eval(tq));

    Query she("she");
    print(cout, (she&(~qHer)).eval(tq));
}