#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>

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


struct indAndDaughter {
    int ind;
    int cntDaughter;
};

struct CompareTimeAndInd {
    bool operator()(const indAndDaughter& a, const indAndDaughter& b) {
        return a.cntDaughter < b.cntDaughter; 
    }
};

struct structProcPriorQueue {
    priority_queue<
        indAndDaughter, 
        vector<indAndDaughter>, 
        CompareTimeAndInd
    > VecQueue;
    
    priority_queue<
        indAndDaughter, 
        vector<indAndDaughter>, 
        CompareTimeAndInd
    > CubQueue;
};

struct timeAndInd {
    int time = 0;
    int ind = -1;
};

struct structResultData {
    vector<vector<int>> resVec;
    vector<vector<int>> resCub;
};

struct structDataProc {
    vector<timeAndInd> vecProcVec;
    vector<timeAndInd> vecProcCub;
    stack<int> stProcVec;
    stack<int> stProcCub;
};


int SortKahn(int numNodes, vector<vector<int>>& graph, vector<int>& result); // сортировка процессоров (вершин) для 
//выявления всех процессов которые можно запустить в нулевой точке, то есть которые будут запускатся первыми

int Input(int size_proces, int sizeProcV, int sizeProcC, int &sizeNodes, vector<process> &arr, vector<vector<int>> &graph); // ввод значений

int Start(structProcPriorQueue &procPriorQueue, vector<vector<int>> &graph, vector<process> &arr, vector<int> &topsort, int size_proces); // заполнение 
//очереди в нулевой точке. является логикой выбора точек, само добавление процессов вынесенно в отдельную функцию

int AddElQueue(structProcPriorQueue &procPriorQueue, vector<process> &arr, int ind); // добавление процесса в очередь

int idleProc(structDataProc &dataProc, structProcPriorQueue &procPriorQueue, vector<process> &arr, int size_proces, int sizeProcV, int sizeProcC, int delta, int &cntProcessComplite); // функция нахождения свободных процессоров и добавления их в стек свободных процессоров

int startProc(structDataProc &dataProc, structProcPriorQueue &procPriorQueue, vector<process> &arr, structResultData &resultData); // функция добавления процессов на свободные процессоры 

int minTime(structDataProc &dataProc, int &numMinTime, int sizeProcC, int sizeProcV);

int clearStack(structDataProc &dataProc);

int output(structResultData &resultData);

int main()
{
    vector <process> arr;
    vector <vector<int>> graph;
    vector<int> topsort;
    structDataProc dataProc;
    vector<int> result;
    structResultData resultData;

    structProcPriorQueue ProcPriorQueue;


    int size_proces, sizeProcV, sizeProcC, sizeEdge, cntProcessComplite = 0, numMinTime = 0;
    cin >> size_proces >> sizeProcV >> sizeProcC;  //количество процессов, кол-во ядер v и c

    dataProc.vecProcVec.resize(sizeProcV);
    dataProc.vecProcCub.resize(sizeProcC);
    resultData.resVec.resize(sizeProcV);
    resultData.resCub.resize(sizeProcC);

    Input(size_proces, sizeProcV, sizeProcC, sizeEdge, arr, graph);
    
    SortKahn(size_proces, graph, topsort);

    Start(ProcPriorQueue, graph, arr, topsort, size_proces);

    while (cntProcessComplite != size_proces)
    {
        idleProc(dataProc, ProcPriorQueue, arr, size_proces, sizeProcV, sizeProcC, numMinTime, cntProcessComplite);

        startProc(dataProc, ProcPriorQueue, arr, resultData);

        minTime(dataProc, numMinTime, sizeProcC, sizeProcV);

        
    }

    output(resultData);
}

int Input(int size_proces, int sizeProcV, int sizeProcC, int &sizeNodes, vector<process> &arr, vector<vector<int>> &graph) {
    arr.resize(size_proces);
    graph.resize(size_proces);

    for (int i = 0; i < size_proces; i++) {  //массив характеристик процессов
        cin >> arr[i].type >> arr[i].time;
    }

    cin >> sizeNodes;

    for (int i = 0; i < sizeNodes; i++) {  //граф
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

int AddElQueue(structProcPriorQueue &procPriorQueue, vector<process> &arr, int ind) {
    indAndDaughter el;
    el.ind = ind;
    el.cntDaughter = arr[ind].cntDaugher;

    if (arr[ind].type == 'V') {
        procPriorQueue.VecQueue.push(el);
    } else {
        procPriorQueue.CubQueue.push(el);
    }
    
    return 0;
}

int Start(structProcPriorQueue &procPriorQueue, vector<vector<int>> &graph, vector<process> &arr, vector<int> &topsort, int size_proces) {
    for (int i = 0; i < size_proces; i++) {
        int ind = topsort[i];
        if (arr[ind].cntParents != 0) {
            break;
        }
        AddElQueue(procPriorQueue, arr, ind);
    }
    return 0;
}

int idleProc(structDataProc &dataProc, structProcPriorQueue &procPriorQueue, vector<process> &arr, int size_proces, int sizeProcV, int sizeProcC, int delta, int &cntProcessComplite) {
    int ind;
    clearStack(dataProc);
    for (int i = 0; i < sizeProcV; i++) {
        dataProc.vecProcVec[i].time = max(0, dataProc.vecProcVec[i].time - delta);
        if (dataProc.vecProcVec[i].time == 0) {
            dataProc.stProcVec.push(i);
            ind = dataProc.vecProcVec[i].ind;
            if (ind != -1) {
                cntProcessComplite++;
                for (int j : arr[ind].daughter) {
                    arr[j].cntParents--;
                    if (arr[j].cntParents == 0) {
                        AddElQueue(procPriorQueue, arr, j);
                    }
                }
                dataProc.vecProcVec[i].ind = -1;
            }
        }
    }

    for (int i = 0; i < sizeProcC; i++) {
        dataProc.vecProcCub[i].time = max(0, dataProc.vecProcCub[i].time - delta);
        if (dataProc.vecProcCub[i].time == 0) {
            dataProc.stProcCub.push(i);
            ind = dataProc.vecProcCub[i].ind;
            if (ind != -1) {
                cntProcessComplite++;
                for (int j : arr[ind].daughter) {
                    arr[j].cntParents--;
                    if (arr[j].cntParents == 0) {
                        AddElQueue(procPriorQueue, arr, j);
                    }
                }
                dataProc.vecProcCub[i].ind = -1;
            }
        }
    }
    
    return 0;
}


int startProc(structDataProc &dataProc, structProcPriorQueue &procPriorQueue, vector<process> &arr, structResultData &resultData) {
    int flag1 = 1;
    int flag2 = 1;
    int indProcess, indProcessor;
    while (flag1 && flag2)
    {
        flag1 = 0;
        flag2 = 0;
        if (!dataProc.stProcCub.empty()) {
            flag1 = 1;
            indProcessor = dataProc.stProcCub.top();
            if (!procPriorQueue.CubQueue.empty()) {
                flag2 = 1;
                indProcess = procPriorQueue.CubQueue.top().ind;
                
                procPriorQueue.CubQueue.pop();
                resultData.resCub[indProcessor].push_back(indProcess);
                dataProc.vecProcCub[indProcessor].ind = indProcess;
                dataProc.vecProcCub[indProcessor].time = arr[indProcess].time;

                dataProc.stProcCub.pop();
            }
        }
        if (!dataProc.stProcVec.empty()) {
            flag1 = 1;
            indProcessor = dataProc.stProcVec.top();
            if (!procPriorQueue.VecQueue.empty()) {
                flag2 = 1;
                indProcess = procPriorQueue.VecQueue.top().ind;
                
                procPriorQueue.VecQueue.pop();
                resultData.resVec[indProcessor].push_back(indProcess);
                dataProc.vecProcVec[indProcessor].ind = indProcess;
                dataProc.vecProcVec[indProcessor].time = arr[indProcess].time;

                dataProc.stProcVec.pop();
            }
        }
    }
    
    return 0;
}

int minTime(structDataProc &dataProc, int &numMinTime, int sizeProcC, int sizeProcV) {
    numMinTime = 4 * pow(10, 3);
    for (int i = 0; i < sizeProcV; i++) {
        if (dataProc.vecProcVec[i].ind != -1) {
            numMinTime = min(numMinTime, dataProc.vecProcVec[i].time);
        }
    }
    for (int i = 0; i < sizeProcC; i++) {
        if (dataProc.vecProcCub[i].ind != -1) {
            numMinTime = min(numMinTime, dataProc.vecProcCub[i].time);
        }
    }

    return 0;
}

int clearStack(structDataProc &dataProc) {
    dataProc.stProcCub = stack<int>();
    dataProc.stProcVec = stack<int>();
    return 0;
}

int output(structResultData &resultData) {
    for (vector<int> j : resultData.resVec) {
        cout << j.size() << " ";
        for (int r : j) {
            cout << r + 1 << " ";
        }
        cout << "\n";
    }
    for (vector<int> j : resultData.resCub) {
        cout << j.size() << " ";
        for (int r : j) {
            cout << r + 1 << " ";
        }
        cout << "\n";
    }

    return 0;
}
