#include <iostream>
#include <queue>
#include <vector>
using namespace std;


int SortKahn(int numNodes, vector<vector<int>>& graph, vector<int>& result) {

    vector<int> inDegree(numNodes, 0);
    for (int i = 0; i < numNodes; i++) {
        for (int v : graph[i])
            inDegree[v]++;
    }

    queue<int> q;
    for (int i = 0; i < numNodes; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);

        for (int v : graph[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    return 0;
}


int main()
{
    vector <vector<int>> graph;

    int kolv, n;
    cin >> kolv>>n;
    vector <int> result;

    graph.resize(kolv);

    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        graph[a].push_back(b);
    }
    
    SortKahn(kolv, graph, result);

    for (int i = 0; i < kolv; i++) {
        cout << result[i];
    }
}
