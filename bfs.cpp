#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

int main() {
    int n, m, source;

    // 🔹 Input
    cout << "Enter number of vertices: ";
    cin >> n;

    cout << "Enter number of edges: ";
    cin >> m;

    vector<vector<int>> adj(n + 1);

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    cout << "Enter source vertex: ";
    cin >> source;

    vector<bool> visited(n + 1, false);
    queue<int> q;

    q.push(source);
    visited[source] = true;

    double start = omp_get_wtime();

    cout << "\nParallel BFS Traversal: ";

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        cout << curr << " ";

        // 🔹 Parallel processing of neighbors
        #pragma omp parallel for
        for (int i = 0; i < adj[curr].size(); i++) {
            int next = adj[curr][i];

            bool shouldAdd = false;

            // Protect visited array
            #pragma omp critical
            {
                if (!visited[next]) {
                    visited[next] = true;
                    shouldAdd = true;
                }
            }

            // Protect queue
            if (shouldAdd) {
                #pragma omp critical
                q.push(next);
            }
        }
    }

    double end = omp_get_wtime();

    cout << "\nExecution Time: " << end - start << " seconds\n";

    return 0;
}

/*  input
    vertices 6
    edge 8
    0 1
    0 5
    2 5
    0 2
    2 3
    1 4
    3 4
    4 3   */