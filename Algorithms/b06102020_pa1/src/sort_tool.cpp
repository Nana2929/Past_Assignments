// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2020/9/15 Mu-Ting Wu]
// **************************************************************************

#include "sort_tool.h"
#include <iostream>
#include <math.h>
// Nana; submission on 10/3
// Constructor
SortTool::SortTool() {}
/*
InsertionSort: Done
*/
// Insertsion sort method
void SortTool::InsertionSort(vector<int> &data)
{
    for (int j = 1; j < data.size(); j++)
    {
        int key = data[j];
        int i = j - 1;
        // 注意pseudo code寫i > 0，因為pseudo code的indexing是採1開始
        while (i >= 0 and data[i] > key)
        {
            // 第一輪的data[i+1]就是data[j]，代表key原位置的值會被覆寫
            // 但我們已用變數key記下so np
            data[i + 1] = data[i];
            i--;
        }
        data[i + 1] = key;
    }
}

/*
QuickSort: Done
*/
void SortTool::QuickSort(vector<int> &data)
{
    QuickSortSubVector(data, 0, data.size() - 1);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int> &data, int low, int high)
{
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    // Hint : recursively call itself
    //        Partition function is needed
    if (low < high)
    {
        int pivot_pos = Partition(data, low, high);
        QuickSortSubVector(data, low, pivot_pos - 1);
        QuickSortSubVector(data, pivot_pos + 1, high);
    }
}

int SortTool::Partition(vector<int> &data, int low, int high)
{
    // Function : Partition the vector
    // TODO : Please complete the function
    // Hint : Textbook page 171
    // partition uses fixed pivot selection
    int pivot = data[high];
    // i: the middle 'partition'中隔板 that separates the >(=) side and < side
    // the last pos of i also records the pos of pivot (i+1)
    int i = low - 1;
    // j 檢查進度
    for (int j = low; j < high; j++)
    {
        if (data[j] < pivot)
        {
            i++;
            int temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }
    // last step: exchange data[i+1] with data[high]
    data[high] = data[i + 1];
    data[i + 1] = pivot;
    // return pivot position
    return i + 1;
}

/*
 MergeSort: Done
*/
void SortTool::MergeSort(vector<int> &data)
{
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int> &data, int low, int high)
{
    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    // Hint : recursively call itself
    //        Merge function is needed
    if (high > low)
    {
        int middle; // calculate middle of vector
        middle = (low + high) / 2;
        MergeSortSubVector(data, low, middle);      // first half of vector
        MergeSortSubVector(data, middle + 1, high); // second half of vector
        // merge two sorted vectors after split calls return
        Merge(data, low, middle, high);
    }
}

// Merge
void SortTool::Merge(vector<int> &data, int low, int middle, int high)
{
    // Function : Merge two sorted subvector
    // TODO : Please complete the function
    int n1 = middle - low + 1;
    int n2 = high - middle;
    int Sentinel = 2147483647;
    // not in-place
    vector<int> LVec(n1 + 1, 0);
    vector<int> RVec(n2 + 1, 0);
    for (int i = 0; i < n1; i++)
    {
        LVec[i] = data[low + i];
    };
    for (int j = 0; j < n2; j++)
    {
        RVec[j] = data[middle + j + 1];
    }

    LVec[n1] = Sentinel;
    RVec[n2] = Sentinel;
    int x = 0;
    int y = 0;
    for (int k = low; k <= high; k++)
    {
        if (LVec[x] <= RVec[y])
        {
            // cout << "LVec[" << x << "]" << LVec[x] << endl;
            data[k] = LVec[x];
            x++;
        }
        else
        {
            //cout << "RVec[" << y << "]" << RVec[y] << endl;
            data[k] = RVec[y];
            y++;
        }
    }
}

/*
HeapSort: Done 
*/
// 連續的decrease_key()
void SortTool::HeapSort(vector<int> &data)
{
    // Build Max-Heap
    BuildMaxHeap(data);
    // 1. Swap data[0] which is max value and data[i] so that the max value will be in correct location
    // 2. Do max-heapify for data[0]
    for (int i = data.size() - 1; i >= 1; i--)
    {
        swap(data[0], data[i]);
        heapSize--;
        MaxHeapify(data, 0);
    }
}
//Max heapify
void SortTool::MaxHeapify(vector<int> &data, int root)
{
    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // 出於indexing的關係，孩子為root * 2 + 1與root * 2 + 2
    int left_index = root * 2 + 1;
    int right_index = root * 2 + 2;
    int Max = root;
    if (left_index < heapSize)
    {
        if (data[left_index] > data[root])
        {
            Max = left_index;
        }
    }
    if (right_index < heapSize)
    {
        if (data[right_index] > data[Max])
        {
            Max = right_index;
        }
    }
    if (Max != root)
    {
        int temp = data[Max];
        data[Max] = data[root];
        data[root] = temp;
        MaxHeapify(data, Max);
    }
}

//Build max heap
void SortTool::BuildMaxHeap(vector<int> &data)
{
    heapSize = data.size(); // initialize heap size
    // Function : Make input data become a max-heap
    for (int j = floor((heapSize) / 2) - 1; j >= 0; j--)
    {
        MaxHeapify(data, j);
        // take data[j] as root and heapify it
    }
}
