/*//////////////////////////////
Cycle-Breaking
Algorithm pa3
/////////////////////////////*/

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <queue>
#include <set>
#include <algorithm>
#include <iterator>
#include <stack>
#include <limits>
#include <tuple>
#include "../lib/tm_usage.h"
#include "Structs.cpp" //typedef: pint, tint and structs
using namespace std; 

// timer: https://cboard.cprogramming.com/c-programming/139486-exit-program-using-timer.html
class Graph
{
public:
    vector<vector<pint>> adjList;
    set<tint> &Edges;
    int total_weight = 0;
    int n; // vertices number 
    int m; // edges number
    vector<vector<tint>> cycles;
    // constructor 
    Graph(set<tint> &edges, int n, int m, bool Direction):Edges(edges){
        // input: edges: pairs of 
        adjList.resize(n);
        int temp_w = 0;
        this->m = m;
        this->n = n;
        set<tint>::iterator iter_edges = Edges.begin();
        while (iter_edges != Edges.end())
        {
            int src = get<0>(*iter_edges); // 編號 
            int dest = get<1>(*iter_edges); // 編號
            int weight = get<2>(*iter_edges);
            temp_w += weight;// sort function
            adjList[src].push_back(make_pair(dest, weight));
            for (auto &row: adjList){
                sort(row.begin(), row.end(),
                    [](const pint& a, const pint& b) {
                    return a.second < b.second;
                });}
            if (!Direction){adjList[dest].push_back(make_pair(src, weight));}
            iter_edges++;
        }
        this->total_weight = temp_w;
    };
    void PrintGraph(int n)
    {
        printf("--Graph--\n");
        for (int src = 0; src < n; src++)
        {
            cout << src << "--->";
            for (auto neighbor: adjList[src]){
                // neighbor, 該edge的weight
                cout << neighbor.first << "," << neighbor.second << " ";
            }
            cout << endl;
        }
    }
};
class DFS{

    public:
        void Dfs(Graph G);
        bool HasCycle = false;
        int time = 0;
        int cost = 0;
        void Dfs_visit(Graph G, int v, int root, stack<int> &Stack, stack<int> &WeightStack);
        tint FindCycle(stack<int> &Stack, stack<int> &WeightStack, int v);
        tint LightestEdge(vector<pint>&cycle_in_path, int g_vertex);
        // void MarkBlack(stack<int> &Stack);
        // store all elements in one cycle
        vector<char> colors;
        vector<int> times_d;
        vector<int> times_f;
        vector<int> preds;
        vector<tint> to_be_removed;
};
void DFS::Dfs(Graph G)
{
    // init
    int n = G.n; // number of vertices
    colors.resize(n);
    times_d.resize(n);
    times_f.resize(n);
    preds.resize(n);
    int sentinel = std::numeric_limits<int>::max();
    for (int i = 0; i < n; i++)
    {
        colors[i] = 'W';
        preds[i] = -1; // Null
    }
    for (int u = 0; u < n; u++){
        if (colors[u] == 'W')
        // starting a new path 
        {
            // A stack for vertices and a stack for weights for each path  
            stack<int> Stack;
            stack<int> WeightStack; 
            Stack.push(u); 
            WeightStack.push(sentinel); 
            Dfs_visit(G, u, u, Stack, WeightStack); //recursion 
            //MarkBlack(Stack);
        }
    }
};
// https://www.baeldung.com/cs/detecting-cycles-in-directed-graph
void DFS::Dfs_visit(Graph G, int u, int root, stack<int> &Stack, stack<int>&WeightStack) 
{
    time += 1;
    int g_vertex;
    colors[u] = 'G';
    cout << "\ndfs order:" << u;
    for (const auto &neighbor : G.adjList[u])
    {
        // find u's neighbors
        int v = neighbor.first;
        int w = neighbor.second;
        if (colors[v] == 'W')
        {
            preds[v] = u;
            Stack.push(v);
            WeightStack.push(w); // u --w--> v
            cout << " "<< v;
            Dfs_visit(G, v, root, Stack, WeightStack);
        }
        else if (colors[v] == 'G')
        {
            //every dfs_visit yields a cycle -> simple cycle
            g_vertex = v;
            //cout << "g_vertex:" << g_vertex << endl;
            // u -w-> v
            // Stacks will have 2 same G vertices at top and bottom
            Stack.push(v);
            WeightStack.push(w);
            tint min_edge = FindCycle(Stack, WeightStack, g_vertex); // gray vertex
            if (find(to_be_removed.begin(), to_be_removed.end(), min_edge) == to_be_removed.end()) {
                to_be_removed.push_back(min_edge);
                cost += get<2>(min_edge);
            }
            HasCycle = true;
        }
    }
    int to_be_marked = Stack.top();
    colors[to_be_marked] = 'B';
    time += 1;
    times_f[u] = time;
    Stack.pop();
    WeightStack.pop();
};
tint DFS::LightestEdge(vector<pint> &cycle_in_path, int g_vertex){
    // https://stackoverflow.com/questions/57281739/finding-the-maximum-tuple-in-a-vector
    // reconstruct edges and weights
    size_t edges_n = cycle_in_path.size();
    tint edge;
    vector<tint> edges;
    int s = 0;
    int t = 0;
    int w = 0;
    for (int i = edges_n - 2; i >= 0; i--)
    {
        s = cycle_in_path[i + 1].first;
        t = cycle_in_path[i].first;
        w = cycle_in_path[i].second;
        edge = make_tuple(cycle_in_path[i + 1].first, cycle_in_path[i].first, cycle_in_path[i].second);
        edges.push_back(edge);
    }
    auto min_edge = min_element(edges.begin(), edges.end(),
                               [](const tint &x,
                                  const tint &y) {
                                return get<2>(x) < get<2>(y);
                               });                    
    //printf("min: %d-%d->%d\n", get<0>(*min_edge), get<2>(*min_edge), get<1>(*min_edge));
    return *min_edge; // in cycle
}
tint DFS::FindCycle(stack<int> &Stack, stack<int> &WeightStack, int g_vertex)
{
    // g_vertex has been colored gray 
    // before u reaches g_vertex 
    // two stacks
    //// 1/3 TODO: save the weights within the cycle and find the min to cut 
    stack<pint> temp; // display cycle
    vector<pint> cycle_in_path; // stores cycle for lightest edge finding 
    int endp;
    int weight;
    int top;
    int top_w;
    //printf("--Cycle--\n");
    //printf("path size: %lu\n", Stack.size());
    if (Stack.top() == g_vertex){
        cycle_in_path.push_back(make_pair(g_vertex, WeightStack.top())); 
        Stack.pop();
        WeightStack.pop();
    }
    while (Stack.top() != g_vertex)
    {
        endp = Stack.top();
        weight = WeightStack.top();
        cycle_in_path.push_back(make_pair(endp, weight));
        temp.push(make_pair(endp, weight));
        Stack.pop();
        WeightStack.pop();
    }
    cycle_in_path.push_back(make_pair(g_vertex, WeightStack.top()));
    temp.push(make_pair(g_vertex, WeightStack.top()));
    Stack.pop(); // empty 
    WeightStack.pop(); //empty 
    //printf("cycle size: %lu\n", temp.size());
    while (!temp.empty())
    {
        top = temp.top().first;// the first output is the cycle-occurred vertex
        top_w = temp.top().second;
        if (temp.size() != 1)
        {
            //printf("-->%d", top);
        }
        else{
            //printf("-->%d\n", top); 
        }
        Stack.push(temp.top().first);
        WeightStack.push(temp.top().second);
        // stack is called by reference and will be popped in Dfs_visit()
        temp.pop();}
    tint min_edge = LightestEdge(cycle_in_path, g_vertex); // the one to be removed 
    return min_edge;
};
pair<int, set<tint>> UpdateEdges(set<tint> &edges, vector<tint> &v){
    // set a counter for edge number 
    // return a new set of edges
    set<tint> new_edges;
    set<tint>::iterator siter = edges.begin();
    vector<tint>::iterator viter;
    int count_edges = 0;
    while (siter != edges.end()) // traverse every edge
    {
        viter = find(v.begin(), v.end(), (*siter));
        // check if it was removed
        if (viter == v.end()){
            // if not found in removed: save the edge in new edges
            int src = get<0>(*siter); 
            int dest = get<1>(*siter); 
            int weight = get<2>(*siter);
            new_edges.insert(make_tuple(src, dest, weight));
            count_edges++;
        }
        siter++;
    }
    return make_pair(count_edges, new_edges); // pair returns reference by default
}
// MST: Prim's algorithm(for undirected graph)
pair<int, set<tint>> MST_Prim(Graph G, int r)
{
    int n = G.n; // m: number of vertices, indexing from 0
    int costs = 0;
    vector<int> key(n);
    vector<int> preds(n);
    vector<bool> visited(n, false);
    set<tint> visited_edges;
    for (int i = 0; i < n; i++)
    {
        key[i] = numeric_limits<int>::min();
        preds[i] = -1; // Null 
    }
    key[r] = 0;
    // 12/20 priority queue are not designed for changes after insertion
    priority_queue<pair<int, int>, vector<pair<int, int>>, BySecond> Q; //Extract-max key
    Q.push(make_pair(r, key[r]));
    while (!Q.empty())
    {
        int u = Q.top().first; // .top().second is its key
        int ukey = Q.top().second;
        //cout << "Queue top:" << u << ";"<< ukey << endl;
        Q.pop();
        visited[u] = true;
        for (pint neighbor: G.adjList[u]){
            // neighbor.first: int, v
            // neighbor.second: int, w(u,v)
            int neigh = neighbor.first;
            int w = neighbor.second;
            if ((visited[neigh] == false) && (w > key[neigh]))
            {
                preds[neigh] = u;
                key[neigh] = w;
                Q.push(make_pair(neigh, key[neigh]));
            }
        }
    }
    int cost = 0;
    // paths visited (by tracing preds)
    for (int i = 0; i < n; i++)
    {
        int pred = preds[i];
        int w = key[i];
        if (i == 0){} // no preddecessor
        //small, big
        else if (pred < i)
        {
            visited_edges.insert(make_tuple(pred, i, w));
            cost += w;
        }
        else{
            visited_edges.insert(make_tuple(i, pred, w));
            cost += w;
        }
    }
    set<tint> unvisited;
    int cnt = 0;

    set_difference(G.Edges.begin(), G.Edges.end(), visited_edges.begin(), visited_edges.end(),
                   inserter(unvisited, unvisited.end()));
    cout << "original edges:" << G.Edges.size() << endl;
    cout << "unvivisted edges: " << unvisited.size() << endl;
    int weight_to_cut = G.total_weight - cost; // 要拔掉的weights
    cout <<  weight_to_cut << endl;
    pair <int, set<tint>> res(weight_to_cut, unvisited);
    return res;
}


///////////////main/////////////////////////
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Wrong command numbers." << endl;
        return 0;
    }
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    //////////// reading the input file /////////////
    // ./bin/cb <input file name> <output file name>
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    char dir; // 'u' or 'd'
    int n = 0; // total number of vertices
    int m = 0; // total number of edges  
    fin >> dir;
    fin >> n;
    fin >> m;
    int src, dest, weight;
    set<tint> edges;
    for (int i = 0; i < m; i++)
    {
        fin >> src >> dest >> weight;
        edges.insert(make_tuple(src, dest, weight));

    }
    bool direction;
    if (dir == 'u')
    {
       direction = false;
    }
    else
    {
       direction = true;
    }
    Graph G(edges, n, m, direction);
    G.PrintGraph(n);
    ///
    //////////// cycle breaking /////////////
    tmusg.periodStart();
    if (dir == 'u'){
         //weights are passed along with G
        pair<int, set<tint>> outs = MST_Prim(G, 0);
        if (outs.first != 0){
            fout << outs.first << endl;
            set<tint> to_remove = outs.second;
            set<tint>::iterator iter = to_remove.begin();
            while (iter != to_remove.end()){
                fout << get<0>(*iter) << " " << get<1>(*iter) << " " << get<2>(*iter) << endl;
                iter++;
            }
        }
        else{
            fout << 0 << endl;
        } 
    }
    else if (dir == 'd'){
        //void Dfs(Graph G);
        //void Dfs_visit(Graph G, int v);
        //void FindCycle(Graph G, int u, int v);
        vector<tint> v;
        bool Cycled = false;
        int total_cost = 0;
        // 執行第一次
        DFS DFS_helper; //0: src node
        DFS_helper.Dfs(G);
        total_cost += DFS_helper.cost;
        if (DFS_helper.HasCycle)
        {
            vector<tint> v_ = DFS_helper.to_be_removed;
            v.insert(v.end(), v_.begin(), v_.end());
            Cycled = true;
        }
        // 執行之後 
        while (Cycled)
        {
            pair<int, set<tint>> res = UpdateEdges(edges, v); 
            int new_m = res.first;
            set<tint> new_edges = res.second;
            Graph new_G(new_edges, n, new_m, direction);
            DFS DFS_helper; // redefine a new dfs
            // renew graph G
            DFS_helper.Dfs(new_G);
            total_cost += DFS_helper.cost;
            Cycled = DFS_helper.HasCycle;
            vector<tint> v_ = DFS_helper.to_be_removed;
            v.insert(v.end(), v_.begin(), v_.end());
        }
        // FindCycle is executed
        if (!v.empty()){
            fout << total_cost << endl;
            for (auto edge : v)
            {
                fout << get<0>(edge) << " " << get<1>(edge) << " " << get<2>(edge) << endl;
            };
        }
        else{
            fout << 0 << endl;
        }
        
    }
    else{
        cout << "Please specify graph type with 'd' or 'u'." << endl;
    }
    tmusg.getPeriodUsage(stat);
    //////////// output /////////////
    cout << "The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;
    cout << "memory: " << stat.vmPeak << "KB" << endl; // print peak memory
    fin.close();
    fout.close();
    return 0;
}

