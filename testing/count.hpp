#include <thread>
#include <vector>

#include "utils.hpp"
#include "../core/sequentialDSU.hpp"
#include "../core/coarseGrainedDSU.hpp"
#include "../core/fineGrainedDSU.hpp"
#include "../core/lockFreeDSU.hpp"
using namespace std;

namespace count
{

int N, M;
vector<vector<int>> edges;

int seqCOUNT()
{
    SequentialDSU uf(N);
    for (auto &e : edges)
        uf.unite( e[0], e[1] );
    return uf.countSets();
}

void work(int id, int TC, DSU *uf)
{
    for (int i = id; i < M; i += TC)
    {
        int a = edges[i][0];
        int b = edges[i][1];
        uf->unite(a, b);
    }
}

int launch(int TC, DSU *uf)
{
    vector<thread> t;
    for (int i = 0; i < TC; ++i)
        t.push_back( thread(work, i, TC, uf) );

    for (int i = 0; i < TC; ++i)
        t[i].join();

    return uf->countSets();
}

int cgCOUNT(int TC)
{
    CoarseGrainedDSU uf(N);
    return launch(TC, &uf);
}

int fgCOUNT(int TC)
{
    FineGrainedDSU uf(N);
    return launch(TC, &uf);
}

int lfCOUNT(int TC)
{
    LockFreeDSU uf(N);
    return launch(TC, &uf);
}

void test(int TC)  // TC = thread count
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
    string base = "testing/inputs/count/";
    string fout = "count_" + nt + ".out";
    freopen(fout.c_str(), "w", stdout);

    cout << "Task: COUNT\n";
    cout << "Description: find # of connected components in graph\n";
    cout << "Number of Threads: " << TC << "\n";

    for (int t = 0; t < 10; ++t)
    {
        string fin = base + to_string(t) + ".in";
        utils::setIO(fin);

        cin >> N >> M;
        edges.assign(M, vector<int>(2));
        for (auto &e : edges)
            cin >> e[0] >> e[1];

        cout << "--------------------------------------------------------------\n";
        cout << "Test Case: " << t << "\n";
        cout << "Vertices:  " << N << "\n";
        cout << "Edges:     " << M << "\n\n";

        TIME[0] = utils::time();
        ANS [0] = seqCOUNT();
        TIME[0] = utils::time();

        TIME[1] = utils::time();
        ANS [1] = cgCOUNT(TC);
        TIME[1] = utils::time();

        TIME[2] = utils::time();
        ANS [2] = fgCOUNT(TC);
        TIME[2] = utils::time();

        TIME[3] = utils::time();
        ANS [3] = lfCOUNT(TC);
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
