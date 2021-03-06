#include "dpll.h"

#include <exception>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;
using namespace boost;

// Clause = Satisfied | Unsatisfied Terms
// Cnf = Unsatisfied | Satisfied Clauses
// This way, both are monoids

struct {
    size_t splits,prop,smart_branch;
} stats;

UnSat<Clause> set_var_clause(int var,const Clause &clause) {
    Clause ret;
    for(auto j = begin(clause); j < end(clause); ++j) {
        if(*j == var) {
            return UnSat<Clause>();
        } else if(*j == -var) {
            continue;
        } else {
            ret.push_back(*j);
        }
    }
    return ret;
}

Sat<Cnf> set_var(int var,const Cnf &cnf) {
    Cnf ret;
    ret.variables = cnf.variables;
    ret.used_variables = cnf.used_variables;
    ret.used_variables.insert(var);
    for(auto i = begin(cnf.clauses); i < end(cnf.clauses); ++i) {
        UnSat<Clause> c = set_var_clause(var,*i);
        if(!c)
            continue;
        if(c->empty())
            return Sat<Cnf>();
        else
            ret.clauses.push_back(*c);
    }
    return Sat<Cnf>(ret);
}

optional<int> bcp_clause(const Clause &c) {
    if(c.size() == 1)
        return optional<int>(c[0]);
    else
        return optional<int>();
}

bool is_resolved(const Sat<Cnf> &in) {
    if(!in || in->clauses.empty())
        return true;
    else
        return false;
}

template<class U,class V>
Sat<Cnf> mplus(U a,V b) {
    Sat<Cnf> x = a();
    if(bool(x))
        return x;
    else
        return b();
}

Sat<Cnf> dpll(const Sat<Cnf> &in) {
    bool resolved;
    resolved = is_resolved(in);
    if(resolved)
        return in;

    for(auto c: in->clauses) {
        auto bvar = bcp_clause(c);
        if(bvar) {
            ++stats.prop;
            return dpll(set_var(*bvar,*in));
        }
    }

    //MOM
    
    unordered_map<int,size_t> counts;
    for(auto i = begin(in->clauses); i != end(in->clauses); ++i) {
        if(i->size() <= 5)
            continue;

        for(auto j = begin(*i); j != end(*i); ++j) {
            ++counts[*j];
        }
    }

    int var;
    if(!counts.empty()) {
        ++stats.smart_branch;
        var = max_element(begin(counts),end(counts),[](const pair<int,size_t> &x,const pair<int,size_t> &y){return x.second < y.second;})->first;
    } else
        for(var = 1; var < in->variables; ++var)
            if(!in->used_variables.count(var) && !in->used_variables.count(-var)) {
                break;
            }


    ++stats.splits;
    

   return mplus([&](){return dpll(set_var(var,*in));}, [&](){return dpll(set_var(-var,*in));});
}

void print_dpll_stats() {
    cout << "c Stats:" << endl
         << "c Splits: " << stats.splits << endl
         << "c Props: " << stats.prop << endl
         << "c Branch: " << stats.smart_branch << endl;
}


