#include <string>
#include <thread>

#include "testing/count.hpp"
#include "testing/queries.hpp"
#include "testing/wormsort.hpp"
// #include "testing/mst.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    int T = thread::hardware_concurrency();
    if (argc > 1) T = stoi(argv[1]);

    count::test(T);
    queries::test(T);
    wormsort::test(T);
//    mst::test(T);

}
