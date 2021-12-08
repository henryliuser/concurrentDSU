#ifndef LOCK_FREE_DSU_HPP
#define LOCK_FREE_DSU_HPP

#include "DSU.hpp"
#include <vector>
#include <utility>
#include <atomic>

class LockFreeDSU : public DSU
{
public:
    LockFreeDSU(int n);               // construct a DSU of n components

    bool unite(int u, int v);         // unites the components that u and v belong to

    std::pair<int, int> find(int u);  // return the parent and rank of u's component

    bool sameSet(int u, int v);       // return whether u and v are in the same component

    int countSets();                  // return # of connected components remaining

private:
    std::vector<std::atomic<int>> A;
    // (A[u] <  0)  -A[u] is rank of subtree rooted at u
    // (A[u] >= 0)   A[u] is parent of u
};

#endif
