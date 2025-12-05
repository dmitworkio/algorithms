#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct process {
    char type;
    int time;
    vector<int> daughter;
    vector<int> parents;
    int status;
    int cntParents = 0;
    int cntDaugher = 0;
};

struct procQueue {
    queue<int> VecFirst, VecSec, CubFirst, CubSec;
};


int SortKahn(int numNodes, vector<vector<int>>& graph, vector<int>& result);

int Input(int size_proces, int sizeProcV, int sizeProcC, int *sizeNodes, vector<process> &arr, vector<vector<int>> &graph);

int Start(procQueue &data, vector<vector<int>> &graph, vector<process> &arr, vector<int> &topsort, int size_proces); 

int AddElQueue(procQueue &data, vector<process> &arr, int ind);

int main()
{
    vector <process> arr;
    vector <vector<int>> graph;
    vector<int> topsort;
    procQueue dataProcQueue;


    int size_proces, sizeProcV, sizeProcC, sizeEdge;
    cin >> size_proces >> sizeProcV >> sizeProcC;  //количество процессов, кол-во ядер v и c

    Input(size_proces, sizeProcV, sizeProcC, &sizeEdge, arr, graph);
    
    SortKahn(size_proces, graph, topsort);

    Start(dataProcQueue, graph, arr, topsort, size_proces);

}

int Input(int size_proces, int sizeProcV, int sizeProcC, int *sizeNodes, vector<process> &arr, vector<vector<int>> &graph) {
    arr.resize(size_proces);
    graph.resize(size_proces);

    for (int i = 0; i < size_proces; i++) {  //массив характеристик процессов
        cin >> arr[i].type >> arr[i].time;
    }

    cin >> *sizeNodes;

    for (int i = 0; i < *sizeNodes; i++) {  //граф
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        graph[a].push_back(b);
        arr[a].daughter.push_back(b);
        arr[b].parents.push_back(a);
        arr[b].cntParents++;
        arr[a].cntDaugher++;
    }

    return 0;
}

int SortKahn(int numNodes, vector<vector<int>>& graph, vector<int> &result) {
    
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

int AddElQueue(procQueue &data, vector<process> &arr, int ind) {
    cout << ind;
    if (arr[ind].cntDaugher == 0) {
        if (arr[ind].type == 'V') {
            data.VecSec.push(ind);
        } else {
            data.CubSec.push(ind);
        }
    } else {
        if (arr[ind].type == 'V') {
            data.VecFirst.push(ind);
        } else {
            data.CubFirst.push(ind);
        }
    }
    
    return 0;
}

int Start(procQueue &data, vector<vector<int>> &graph, vector<process> &arr, vector<int> &topsort, int size_proces) {
    for (int i = 0; i < size_proces; i++) {
        int ind = topsort[i];
        if (arr[ind].cntParents != 0) {
            break;
        }
        AddElQueue(data, arr, ind);
    }
    return 0;
}

