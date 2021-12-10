#include <thread>
#include <vector>
#include <tuple>
#include <algorithm>

#include "utils.hpp"
#include "../core/sequentialDSU.hpp"
#include "../core/coarseGrainedDSU.hpp"
#include "../core/fineGrainedDSU.hpp"
#include "../core/lockFreeDSU.hpp"
using namespace std;

namespace wormsort
{

int N, M;
vector<int> cows;
vector<vector<int>> wormholes;

int seq()
{
    auto check = [](int g)
    {
        SequentialDSU uf(N+1);
        for (int i = g; i < M; ++i)
        {
            int a = wormholes[i][1];
            int b = wormholes[i][2];
            uf.unite(a, b);
        }
        for (int i = 0; i < N; ++i)
        {
            if (cows[i] == i+1) continue;
            if (!uf.sameSet(cows[i], i+1))
                return false;
        }
        return true;
    };

    int lo = 0, hi = M-1;
    while (lo < hi)
    {
        int mid = lo + (hi-lo+1) / 2;
        if (check(mid))
            lo = mid;
        else
            hi = mid-1;
    }
    return wormholes[lo][0];
}

void work(int g, int id, int TC, DSU *uf)
{
    for (int i = g+id; i < M; i += TC)
    {
        int a = wormholes[i][1];
        int b = wormholes[i][2];
        uf->unite(a, b);
    }
}

template<typename dsu>
bool launch(int g, int TC)
{
    dsu uf(N+1);
    vector<thread> t;
    for (int i = 0; i < TC; ++i)
        t.push_back( thread(work, g, i, TC, &uf) );
    for (int i = 0; i < TC; ++i)
        t[i].join();

    for (int i = 0; i < N; ++i)
    {
        if (cows[i] == i+1) continue;
        if (!uf.sameSet(cows[i], i+1))
            return false;
    }
    return true;

}
template <typename dsu>
int parSolve(int TC)
{
    int lo = 0, hi = M-1;
    while (lo < hi)
    {
        int mid = lo + (hi-lo+1) / 2;
        if (launch<dsu>(mid, TC))
            lo = mid;
        else
            hi = mid-1;
    }
    return wormholes[lo][0];
}

int coarse(int TC)
{
    return parSolve<CoarseGrainedDSU>(TC);
}

int fine(int TC)
{
    return parSolve<FineGrainedDSU>(TC);
}

int lockFree(int TC)
{
    return parSolve<LockFreeDSU>(TC);
}

bool isSorted()
{
    for (int i = 0; i < N-1; ++i)
        if (cows[i] > cows[i+1])
            return false;
    return true;
}

void test(int TC)
{
    int ANS[4];
    int TIME[4];
    string NAME[4] =
    {
        "Sequential    ",
        "Coarse-Grained",
        "Fine-Grained  ",
        "Lock-Free     "
    };
    string nt = to_string(TC);
    string base = "testing/inputs/wormsort/";
    string fout = "wormsort_" + nt + ".out";
    freopen(fout.c_str(), "w", stdout);

    cout << "Task: WORMSORT\n";
    cout << "Description: maximize width of smallest wormhole cows must use\n";
    cout << "in order to sort themselves. Full problem statement available:\n";
    cout << "http://www.usaco.org/index.php?page=viewproblem2&cpid=992\n";
    cout << "Number of Threads: " << TC << "\n";

    for (int t = 1; t <= 10; ++t)
    {
        string fin = base + to_string(t) + ".in";
        utils::setIO(fin);

        cin >> N >> M;
        cows.resize(N);
        wormholes.assign(M, vector<int>(3));
        for (int &c : cows)
            cin >> c;
        for (auto &e : wormholes)
            cin >> e[1] >> e[2] >> e[0];

        bool s = isSorted();
        sort(begin(wormholes), end(wormholes));  // not timed.

        cout << "--------------------------------------------------------------\n";
        cout << "Test Case: " << t << "\n";
        cout << "Cows:      " << N << "\n";
        cout << "Wormholes: " << M << "\n\n";

        TIME[0] = utils::time();
        ANS [0] = (s) ? -1 : seq();
        TIME[0] = utils::time();

        TIME[1] = utils::time();
        ANS [1] = (s) ? -1 : coarse(TC);
        TIME[1] = utils::time();

        TIME[2] = utils::time();
        ANS [2] = (s) ? -1 : fine(TC);
        TIME[2] = utils::time();

        TIME[3] = utils::time();
        ANS [3] = (s) ? -1 : lockFree(TC);
        TIME[3] = utils::time();

        cout << "[variant | result | time]\n";
        for (int i = 0; i < 4; ++i)
        {
            cout << NAME[i] << " | ";
            cout <<  ANS[i] << " | ";
            cout << TIME[i] << " ns\n";
        }
        cout << "\nSpeed-Up:\n";
        cout << "Coarse-Grained vs. Sequential:     " << 1.0*TIME[0]/TIME[1] << "x\n";
        cout << "  Fine-Grained vs. Sequential:     " << 1.0*TIME[0]/TIME[2] << "x\n";
        cout << "     Lock-Free vs. Sequential:     " << 1.0*TIME[0]/TIME[3] << "x\n\n";
        cout << "  Fine-Grained vs. Coarse-Grained: " << 1.0*TIME[1]/TIME[2] << "x\n";
        cout << "     Lock-Free vs. Coarse-Grained: " << 1.0*TIME[1]/TIME[3] << "x\n\n";
        cout << "     Lock-Free vs. Fine-Grained:   " << 1.0*TIME[2]/TIME[3] << "x\n";
    }

}

}
