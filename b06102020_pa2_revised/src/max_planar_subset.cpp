#include <iostream>
#include <vector>
#include <fstream>
#include "max_planar_subset.h"
// 11/20 
// based on the original btm up version
// use vector<vector<unsigned short>> to avoid requesting contiguous memory

using namespace std;
// specialized return struct
Solution::Solution(){}
void Solution::RecordedChords(int i, int j, vector<vector<unsigned short>>&Cases, vector<unsigned short> &query_data, vector<vector<unsigned short>>&records)
{
    static int counter = 0;
    while (j > i + 1)
    {
        int k = query_data[j];
        if (Cases[i][j] == 1)
        {
            records[counter][0] = j;
            records[counter][1] = k;
            counter++;
            RecordedChords(i + 1, j - 1, Cases, query_data, records);
            j = i - 1;
           
        }
        else if (Cases[i][j] == 2)
        {
            records[counter][0] = j;
            records[counter][1] = k;
            counter++;
            // k/k+1?
            RecordedChords(k, j - 1, Cases, query_data, records);
            // tail recursion 
            j = k - 1;
        }
        else{ // take care of Case3: records[i][j-1]
            j--;
        }
    }
}
pair<int, vector<vector<unsigned short>>> Solution::Max_Planar_Subset(int n, vector<unsigned short>&query_data)
{
    // declare a matrix
    // declare a table storing maximunm subset size (chords number)
    // revision 
    vector<vector<unsigned short>> Size(2 * n, vector<unsigned short>(2 * n, 0)); 
    vector<vector<unsigned short>> Cases(2 * n, vector<unsigned short>(2 * n, 0));
    for (int j = 1; j <= 2 * n - 1; j++)
    {
        // find j's corresponding q
        unsigned short k = query_data[j];
        // finished finding pair
        for (int i = 0; i <= j - 1; i++)
        {
            // assign Size[i][j] to the one with greater number
            // Case 0: k is not within the range of i~j
            if (k > j || k < i)
            {
                Size[i][j] = Size[i][j-1];
            }
            // Case 1: Special Case, k == i
            else if (k == i)
            {
                if (i - j == 1){
                    Size[i][j] = 1;
                }
                else{
                    Size[i][j] = Size[i + 1][j - 1] + 1;
                }
                // recorded chord: (i, j) itself
                Cases[i][j] = 1;
            }
            // Case 2
            else if ((i < k < j) && (Size[i][k - 1] + 1 + Size[k + 1][j - 1]) >= Size[i][j - 1])
            {
                Size[i][j] = Size[i][k - 1] + 1 + Size[k + 1][j - 1];
                // recorded chords: (i, j), and backtrack the other two cells [i][k - 1] and [k + 1][j - 1] for their chords
                // (recursive)
                Cases[i][j] = 2;
            }
            // Case 3(Case2的不include(k,j版本))
            else if ((i < k < j) && (Size[i][k - 1] + 1 + Size[k + 1][j - 1]) < Size[i][j - 1])
            {
                Size[i][j] = Size[i][j - 1]; 
            }
            //printf("%d, %d, %d, %d\n", i, k, j, Size[i][j]);
        }
    }
    // return
    unsigned short size = Size[0][2 * n - 1];
    //cout << "Cases:" << Cases.size() << " " << Cases[0].size() << endl;
    //pair passes l value
    pair<unsigned short, vector<vector<unsigned short>>> res(size, Cases);
    return res;
}

