#include <iostream>
#include <string>
#include <chrono>

#ifndef UTILS_HPP
#define UTILS_HPP

using namespace std;

namespace utils
{
    void setIO(string in)
    {
        freopen(in.c_str(), "r", stdin);
        ios::sync_with_stdio(0);
        cin.tie(0);
    }

    int time(bool show=false, string label="")
    {
        static auto last = chrono::high_resolution_clock::now();
        auto curr = chrono::high_resolution_clock::now();
        int dtn = chrono::duration_cast<chrono::nanoseconds>(curr-last).count();
        int dtm = chrono::duration_cast<chrono::milliseconds>(curr-last).count();
        last = curr;
        if (!show) return dtn;
        cout << "------------------------\n";
        cout << label << "\n";
        cout << "nanoseconds:  " << dtn << "\n";
        cout << "milliseconds: " << dtm << "\n";
        return dtn;
    }
}

#endif
