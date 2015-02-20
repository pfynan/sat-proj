#pragma once

#include "cnf.h"


#include <boost/optional.hpp>
template<class T>
using Sat = boost::optional<T>;
template<class T>
using UnSat = boost::optional<T>;

bool dpll(const UnSat<Cnf> &in);

void print_dpll_stats();

