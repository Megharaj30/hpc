#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

// ---------------- BFS ----------------
void parallelBFS(vector<vector<int>> &tree, int root, int n) {
    vector<bool> visited(n, false);
    queue<int> q;

    visited[root] = true;
    q.push(root);

    cout << "\nParallel BFS: ";

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            int node;

            #pragma omp critical
            {
                node = q.front();
                q.pop();
                cout << node << " ";
            }

            // index-based loop
            for (int j = 0; j < tree[node].size(); j++) {
                int child = tree[node][j];

                if (!visited[child]) {
                    #pragma omp critical
                    {
                        if (!visited[child]) {
                            visited[child] = true;
                            q.push(child);
                        }
                    }
                }
            }
        }
    }
    cout << endl;
}

// ---------------- DFS ----------------
void dfsUtil(int node, vector<vector<int>> &tree, vector<bool> &visited) {

    #pragma omp critical
    cout << node << " ";

    visited[node] = true;

    #pragma omp parallel for
    for (int i = 0; i < tree[node].size(); i++) {
        int child = tree[node][i];

        if (!visited[child]) {
            dfsUtil(child, tree, visited);
        }
    }
}

void parallelDFS(vector<vector<int>> &tree, int root, int n) {
    vector<bool> visited(n, false);

    cout << "\nParallel DFS: ";

    #pragma omp parallel
    {
        #pragma omp single
        dfsUtil(root, tree, visited);
    }

    cout << endl;
}

// ---------------- MAIN ----------------
int main() {
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    vector<vector<int>> tree(n);

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    int root;
    cout << "Enter root: ";
    cin >> root;

    parallelBFS(tree, root, n);
    parallelDFS(tree, root, n);

    return 0;
}