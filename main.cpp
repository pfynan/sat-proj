#include "parser.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include "dpll.h"

using namespace std;


int main(int argc, char *argv[]) {
    if(argc != 2)
        return 1;
    char *fn = argv[1];


    Cnf clauses;
    clauses = parse_DIMACS_CNF(fn);
    Sat<Cnf> result = dpll(Sat<Cnf>(clauses));
    print_dpll_stats();
    cout << "s " << (result ? "SATISFIABLE" : "UNSATISFIABLE")  << endl;
    if(result) {
        cout << "v ";
        ostream_iterator<int> osi(cout, " ");
        copy(begin(result->used_variables),end(result->used_variables),osi);
        cout << "0" << endl;
    }
    return 0;
}
