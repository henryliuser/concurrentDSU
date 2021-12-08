#include <utility>
#include "coarseGrainedDSU.hpp"

CoarseGrainedDSU::CoarseGrainedDSU(int n)
{
    A.assign(n, -1);
    count = n;
}

bool CoarseGrainedDSU::unite(int u, int v)
{
    std::lock_guard<std::mutex> lg(lock);  // lock before mutating
    u = _find(u);
    v = _find(v);
    if (u == v) return false;
    if (A[u] > A[v]) std::swap(u, v);  // union by rank
    if (A[u] == A[v]) --A[u];
    A[v] = u;
    --count;
    return true;
}

int CoarseGrainedDSU::find(int u)
{
    std::lock_guard<std::mutex> lg(lock);
    int p = _find(u);
    return p;
}

int CoarseGrainedDSU::_find(int u)  // will mutate data structure! lock before calling.
{
    if (A[u] < 0) return u;
    A[u] = _find(A[u]);      // path compression
    return A[u];
}

bool CoarseGrainedDSU::sameSet(int u, int v)
{
    std::lock_guard<std::mutex> lg(lock);
    return _find(u) == _find(v);
}

int CoarseGrainedDSU::countSets()
{
    int res = 0;
    for (int &x : A)
        res += (x < 0);
    return res;

}
