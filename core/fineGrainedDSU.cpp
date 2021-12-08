#include <utility>
#include "fineGrainedDSU.hpp"

FineGrainedDSU::FineGrainedDSU(int n)
    : locks(n), A(n)
{
    N = n;
    for (int i = 0; i < N; ++i)
        A[i] = -1;
}

bool FineGrainedDSU::unite(int u, int v)
{
    int x, y;  // keep original u,v for compression later
    do {
        x = find(u);
        y = find(v);
        if (x == y) return false;

        lockRoots(x, y);
        if (A[x] < 0 && A[y] < 0)
            break;
        else
            unlockRoots(x, y);

    } while(true);

    if (A[x] > A[y]) std::swap(x, y);
    if (A[x] == A[y]) --A[x];
    A[y] = x;

    unlockRoots(x, y);
    return true;
}

int FineGrainedDSU::find(int u) {  // wait-free
    if (A[u] < 0) return u;
    while (A[u] >= 0)
    {
        int p = A[u];
        int q = (A[p] < 0) ? p : 1*A[p];
        A[u].compare_exchange_strong(p, q);
        u = q;
    }
    return u;
}

bool FineGrainedDSU::sameSet(int u, int v)
{
    return find(u) == find(v);
}

int FineGrainedDSU::countSets()
{
    int res = 0;
    for (int i = 0; i < N; ++i)
        res += (A[i] < 0);
    return res;
}

void FineGrainedDSU::lockRoots(int u, int v)
{
    if (&locks[u] > &locks[v]) std::swap(u, v);
    // lock in ascending order of memory address
    locks[u].lock();
    locks[v].lock();
}
void FineGrainedDSU::unlockRoots(int u, int v)
{
    if (&locks[u] > &locks[v]) std::swap(u, v);
    // unlock in descending order of memory address
    locks[v].unlock();
    locks[u].unlock();
}
