#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

/* -------- MERGE FUNCTION -------- */
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }

    while (i <= m) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);

    for (int k = 0; k < temp.size(); k++) {
        arr[l + k] = temp[k];
    }
}

/* -------- SEQUENTIAL MERGE SORT -------- */
void merge_sort_seq(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        merge_sort_seq(arr, l, m);
        merge_sort_seq(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

/* -------- PARALLEL MERGE SORT -------- */
void merge_sort_parallel(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            merge_sort_parallel(arr, l, m);

            #pragma omp section
            merge_sort_parallel(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

/* -------- MAIN -------- */
int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    omp_set_num_threads(4);

    double start, end;

    // Sequential
    vector<int> a1 = arr;
    start = omp_get_wtime();
    merge_sort_seq(a1, 0, n - 1);
    end = omp_get_wtime();
    cout << "\nSequential Time: " << end - start;

    // Parallel
    vector<int> a2 = arr;
    start = omp_get_wtime();
    merge_sort_parallel(a2, 0, n - 1);
    end = omp_get_wtime();
    cout << "\nParallel Time: " << end - start;

    cout << "\nSorted Array:\n";
    for (int x : a2) cout << x << " ";

    return 0;
}
