#ifndef COARSE_GRAINED_DSU_HPP
#define COARSE_GRAINED_DSU_HPP

#include "DSU.hpp"
#include <vector>
#include <mutex>

class CoarseGrainedDSU : public DSU {
public:
    CoarseGrainedDSU(int n);     // construct a DSU of n components

    bool unite(int u, int v);    // unites the components that u and v belong to

    int find(int u);             // return the root of u's component

    bool sameSet(int u, int v);  // return whether u and v are in the same component

    int countSets();

private:
    int count;

    int _find(int u);            // get root of u, apply path compression

    std::mutex lock;             // to enforce thread-safety of data structure

    std::vector<int> A;
    // (A[u] <  0)  -A[u] is rank of subtree rooted at u
    // (A[u] >= 0)   A[u] is parent of u
};

#endif
