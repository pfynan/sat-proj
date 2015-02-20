#pragma once

#include "cnf.h"


#include <boost/optional.hpp>
template<class T>
using Sat = boost::optional<T>;
template<class T>
using UnSat = boost::optional<T>;

Sat<Cnf> dpll(const Sat<Cnf> &in);

void print_dpll_stats();

