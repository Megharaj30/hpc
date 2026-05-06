#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

const int MAXN = 100000;

vector<int> adj[MAXN + 5];
bool visited[MAXN + 5];

// 🔹 Parallel DFS using OpenMP tasks
void dfs(int node) {

    bool alreadyVisited = false;

    // Critical section to safely check & update visited
    #pragma omp critical
    {
        if (visited[node]) {
            alreadyVisited = true;
        } else {
            visited[node] = true;
        }
    }

    // If already visited, return
    if (alreadyVisited) return;

    // Explore neighbors in parallel
    for (int i = 0; i < adj[node].size(); i++) {
        int next_node = adj[node][i];

        #pragma omp task
        dfs(next_node);
    }

    // Wait for all child tasks to complete
    #pragma omp taskwait
}

int main() {
    int n, m;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;

    // Input edges
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int start_node;
    cout << "Enter starting node: ";
    cin >> start_node;

    double start = omp_get_wtime();

    // Start parallel region
    #pragma omp parallel
    {
        #pragma omp single
        dfs(start_node);
    }

    double end = omp_get_wtime();

    // Print visited nodes (FIXED: include 0)
    cout << "Visited nodes: ";
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            cout << i << " ";
        }
    }
    cout << endl;

    cout << "Execution time: " << end - start << endl;

    return 0;
}