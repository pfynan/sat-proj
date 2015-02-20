#include "parser.h"

#include <iostream>
#include "dpll.h"

using namespace std;


int main(int argc, char *argv[]) {
    if(argc != 2)
        return 1;
    char *fn = argv[1];


    Cnf clauses;
    clauses = parse_DIMACS_CNF(fn);
    bool result = dpll(UnSat<Cnf>(clauses));
    cout << (result ? "SAT" : "UNSAT")  << endl;
    print_dpll_stats();
    return 0;
}
