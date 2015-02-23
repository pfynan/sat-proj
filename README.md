A SAT solver class project
==========================

### Phillip Fynan, Soumya Mittal

Just type `make` to build. It needs a fairly recent compiler, due to heavy use
of C++11 features, probably GCC 4.7 or Clang 3.3, but I haven't checked. Under
cluster machines it can be compiled with:

    scl enable devtoolset-2 make

Run with:

    ./dpll <benchmark file>

The output file is written to standard out.

Features
---------

The solver implements the basic DPLL algorithm, including BCP, and the MOM
branching heuristic.

