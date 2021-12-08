#include <thread>
#include <vector>

#include "utils.hpp"
#include "../core/sequentialDSU.hpp"
#include "../core/coarseGrainedDSU.hpp"
#include "../core/fineGrainedDSU.hpp"
#include "../core/lockFreeDSU.hpp"
using namespace std;

namespace queries
{

int N, M, Q;
vector<vector<int>> edges;
vector<vector<int>> queries;

bool* seqQueries()
{
    bool* ans = new bool[Q];
    SequentialDSU uf(N);
    for (auto &e : edges)
        uf.unite(e[0], e[1]);
    for (int i = 0; i < Q; ++i)
    {
        int a = queries[i][0];
        int b = queries[i][1];
        ans[i] = uf.sameSet(a, b);
    }
    return ans;
}

void workUnite(int id, int TC, DSU *uf)
{
    for (int i = id; i < M; i += TC)
    {
        int a = edges[i][0];
        int b = edges[i][1];
        uf->unite(a, b);
    }
}

void workQuery(int id, int TC, DSU *uf, bool *ans)
{
    for (int i = id; i < Q; i += TC)
    {
        int a = queries[i][0];
        int b = queries[i][1];
        ans[i] = uf->sameSet(a, b);
    }
}

bool* launch(int TC, DSU *uf)
{
    bool* ans = new bool[Q];
    vector<thread> t;
    for (int i = 0; i < TC; ++i)
        t.push_back( thread(workUnite, i, TC, uf) );
    for (int i = 0; i < TC; ++i)
        t[i].join();

    t.clear();
    for (int i = 0; i < TC; ++i)
        t.push_back( thread(workQuery, i, TC, uf, ans) );
    for (int i = 0; i < TC; ++i)
        t[i].join();

    return ans;
}

bool* cgQueries(int TC)
{
    CoarseGrainedDSU uf(N);
    return launch(TC, &uf);
}

bool* fgQueries(int TC)
{
    FineGrainedDSU uf(N);
    return launch(TC, &uf);
}

bool* lfQueries(int TC)
{
    LockFreeDSU uf(N);
    return launch(TC, &uf);
}

void test(int TC)
{
    int TIME[4];
    bool* ANS[4];
    string NAME[4] =
    {
        "Sequential    ",
        "Coarse-Grained",
        "Fine-Grained  ",
        "Lock-Free     "
    };
    string nt = to_string(TC);
    string base = "testing/inputs/queries/";
    string fout = "queries_" + nt + ".out";
    freopen(fout.c_str(), "w", stdout);

    cout << "Task: QUERIES\n";
    cout << "Description: answer connectivity queries in a graph\n";
    cout << "Number of Threads: " << TC << "\n";

    for (int t = 0; t < 10; ++t)
    {
        string fin = base + to_string(t) + ".in";
        utils::setIO(fin);

        cin >> N >> M >> Q;
        edges.assign(M, vector<int>(2));
        queries.assign(Q, vector<int>(2));
        for (auto &e : edges)
            cin >> e[0] >> e[1];
        for (auto &q : queries)
            cin >> q[0] >> q[1];

        cout << "--------------------------------------------------------------\n";
        cout << "Test Case: " << t << "\n";
        cout << "Vertices:  " << N << "\n";
        cout << "Edges:     " << M << "\n";
        cout << "Queries:   " << Q << "\n\n";

        TIME[0] = utils::time();
        ANS [0] = seqQueries();
        TIME[0] = utils::time();

        TIME[1] = utils::time();
        ANS [1] = cgQueries(TC);
        TIME[1] = utils::time();

        TIME[2] = utils::time();
        ANS [2] = fgQueries(TC);
        TIME[2] = utils::time();

        TIME[3] = utils::time();
        ANS [3] = lfQueries(TC);
        TIME[3] = utils::time();

        for (int i = 0; i < 4; ++i)
        {
            cout << NAME[i] << "\n";
            cout << TIME[i] << " ns\n\n";
            for (int j = 0; j < Q; ++j)
            {
                if (i == 0) continue;
                if (ANS[i][j] != ANS[0][j])
                {
                    cout << "MISMATCH!\n";
                    return;
                }
            }
        }
        cout << "\nSpeed-Up:\n";
        cout << "Coarse-Grained vs. Sequential:     " << 1.0*TIME[0]/TIME[1] << "x\n";
        cout << "  Fine-Grained vs. Sequential:     " << 1.0*TIME[0]/TIME[2] << "x\n";
        cout << "     Lock-Free vs. Sequential:     " << 1.0*TIME[0]/TIME[3] << "x\n\n";
        cout << "  Fine-Grained vs. Coarse-Grained: " << 1.0*TIME[1]/TIME[2] << "x\n";
        cout << "     Lock-Free vs. Coarse-Grained: " << 1.0*TIME[1]/TIME[3] << "x\n\n";
        cout << "     Lock-Free vs. Fine-Grained:   " << 1.0*TIME[2]/TIME[3] << "x\n";
        for (int i = 0; i < 4; ++i)
            delete[] ANS[i];
    }

}

}
