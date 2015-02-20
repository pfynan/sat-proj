#pragma once
#include <vector>
#include <unordered_set>

typedef std::vector<int> Clause;
struct Cnf {
    std::vector<Clause> clauses;
    std::unordered_set<int> used_variables;
    int variables;
};
