#ifndef FINE_GRAINED_DSU_HPP
#define FINE_GRAINED_DSU_HPP

#include "DSU.hpp"
#include <vector>
#include <atomic>
#include <pthread.h>
#include <mutex>

class FineGrainedDSU : public DSU {
public:
    FineGrainedDSU(int n);       // construct a DSU of n components

    bool unite(int u, int v);    // unites the components that u and v belong to

    int find(int u);             // return the root of u's component

    bool sameSet(int u, int v);

    int countSets();

private:
    void lockRoots(int u, int v);
    void unlockRoots(int u, int v);
    std::vector<std::mutex> locks;
    std::vector<std::atomic<int>> A;
    // (A[u] <  0)  -A[u] is rank of subtree rooted at u
    // (A[u] >= 0)   A[u] is parent of u
};

#endif
