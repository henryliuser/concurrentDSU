#include <vector>
#include "sequentialDSU.hpp"

SequentialDSU::SequentialDSU(int n) : A(n, -1)
{
    N = n;
    count = n;
}

bool SequentialDSU::unite(int u, int v)
{
    u = find(u);
    v = find(v);
    if (u == v) return false;      // same component already
    if (A[u] > A[v])               // union by rank
        std::swap(u, v);
    if (A[u] == A[v]) --A[u];      // increment rank
    A[v] = u;                      // re-parent
    --this->count;
    return true;
}

int SequentialDSU::find(int u)
{
    if (A[u] < 0) return u;        // A[u] < 0 means u is a root
    A[u] = find(A[u]);             // path compression
    return A[u];
}

bool SequentialDSU::sameSet(int a, int b)
{
    return find(a) == find(b);     // same set if same root
}

int SequentialDSU::countSets()
{
    return this->count;
}
