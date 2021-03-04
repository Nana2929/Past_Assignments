#ifndef _MAX_PLANAR_SUBSET_H
#define _MAX_PLANAR_SUBSET_H

#include <vector>
using namespace std;
class Solution
{
    public:
    Solution(); // constructor
    pair<int, vector<vector<unsigned short>>> Max_Planar_Subset(int, vector<unsigned short>&);
    void RecordedChords(int i, int j, vector<vector<unsigned short>>&, vector<unsigned short>&, vector<vector<unsigned short>>&);
};
#endif
