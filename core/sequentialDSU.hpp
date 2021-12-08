#ifndef SEQUENTIAL_DSU_HPP
#define SEQUENTIAL_DSU_HPP

#include "DSU.hpp"
#include <vector>

class SequentialDSU : public DSU {
public:
    SequentialDSU(int n);        // construct a DSU of n components

    bool unite(int u, int v);    // unites the components that u and v belong to

    int find(int u);             // return the root of u's component

    bool sameSet(int a, int b);  // return whether u and v are in the same component

    int countSets();

private:
    int count;
    std::vector<int> A;
    // (A[u] <  0)  -A[u] is rank of subtree rooted at u
    // (A[u] >= 0)   A[u] is parent of u
};

#endif
