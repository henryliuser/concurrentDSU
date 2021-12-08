#include <utility>
#include <tuple>
#include "lockFreeDSU.hpp"

LockFreeDSU::LockFreeDSU(int n) : A(n)
{
    N = n;
    for (auto &x : A)
        x = -1;
}

bool LockFreeDSU::unite(int u, int v)
{
    int ru, rv;
    while (true)
    {
        std::tie(u, ru) = find(u);  // obtain parent and rank values simultaneously
        std::tie(v, rv) = find(v);  // use tie for convenient unpacking
        if (u == v) return false;
        if (ru > rv)
        {
            if (A[u].compare_exchange_strong(ru, v))
                return true;
        }
        else if (ru < rv)
        {
            if (A[v].compare_exchange_strong(rv, u))
                return true;
        }
        else
        {
            int x = ru, y = rv;
            if (u < v && A[u].compare_exchange_strong(x, v))
            {
                A[v].compare_exchange_strong(y, rv-1);  // minus 1; ranks are negative
                return true;
            }
            x = ru, y = rv;
            if (u > v && A[v].compare_exchange_strong(y, u))
            {
                A[u].compare_exchange_strong(x, ru-1);
                return true;
            }
        }
    }
    return true;
}

std::pair<int, int> LockFreeDSU::find(int u)
{
    if (A[u] < 0) return std::make_pair(u, 1*A[u]);
    int p = A[u];

    if (A[p] < 0) return std::make_pair(p, 1*A[p]);
    //  ^^ IPC - immediate parent check optimization

    int root, rank;
    std::tie(root, rank) = find(p);

    if (p != root)           // path compression
        while (!A[u].compare_exchange_strong(p, root));

    return std::make_pair(root, rank);  // return them simult. so they are in sync
}

bool LockFreeDSU::sameSet(int u, int v)
{
    int ru, rv;
    while (true)  // until the value stops changing halfway
    {
        std::tie(u, ru) = find(u);
        std::tie(v, rv) = find(v);
        if (u == v) return true;
        if (A[u] < 0) return false;
    }
}

int LockFreeDSU::countSets()
{
    int res = 0;
    for (auto &x : A)
        res += (x < 0);
    return res;
}
