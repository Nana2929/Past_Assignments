#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include "max_planar_subset.h"
#include "../lib/tm_usage.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "handed in, revised ver." << endl;
    if (argc != 3)
    {
        cout << "Commands not in the right format." << endl;
        return 0;
    }
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;

    //////////// reading the input file /////////////
    // ./bin/mps <input file name> <output file name>
    fstream fin(argv[1]);//已確定是12.in
    fstream fout;
    fout.open(argv[2], ios::out);
    int endpoint1, endpoint2;
    int n = 0;
    // 2*n is at most 180000
    fin >> n;
    n = n / 2;
    vector<unsigned short> query_data(2 * n, 0);
    for (int i = 0; i < n; i++)
    {
        fin >> endpoint1 >> endpoint2;
        // update order
        query_data[endpoint2] = endpoint1;
        query_data[endpoint1] = endpoint2;
        // keep endpoint1 < endpoint2 
        if (endpoint1 > endpoint2){
            int temp = endpoint1;
            endpoint1 = endpoint2;
            endpoint2 = temp;
        }
        // use indexing to record another endpoint //j -> k
    };
    // //////////// dp program running + timing ////////////////
    tmusg.periodStart();

    Solution Sol;
    pair<int, vector<vector<unsigned short>>> res = Sol.Max_Planar_Subset(n, query_data);
    unsigned short sol;
    sol = res.first;
    cout << "Sol: " << sol << endl;
    auto Cases= res.second;
    vector<vector<unsigned short>> records(sol, vector<unsigned short>(2, 0));
    Sol.RecordedChords(0, 2 * n - 1, Cases, query_data, records);
    tmusg.getPeriodUsage(stat);
    // outputting
    cout << "The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;
    cout << "memory: " << stat.vmPeak << "KB" << endl; // print peak memory
    fout << sol << endl;
    for (int i = sol-1; i >= 0 ; i--)
    {
        fout << records[i][0] << "," << records[i][1] << endl;
    }
    fin.close();
    fout.close();
    return 0;
}
