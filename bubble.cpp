#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;
using namespace std::chrono;

/* -------- Sequential Bubble Sort -------- */
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

/* -------- Parallel Bubble Sort (Odd-Even) -------- */
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        int start = i % 2;

        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

/* -------- Print -------- */
void printArray(vector<int>& arr) {
    for (int x : arr)
        cout << x << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), original(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        original[i] = arr[i];
    }

    cout << "\nOriginal Array:\n";
    printArray(arr);

    // Sequential
    auto start = high_resolution_clock::now();
    bubbleSort(arr);
    auto end = high_resolution_clock::now();

    cout << "\nSequential Bubble Sort:\n";
    printArray(arr);
    cout << "Time: " << duration<double, milli>(end - start).count() << " ms\n";

    // Restore original
    arr = original;

    // Parallel
    start = high_resolution_clock::now();
    parallelBubbleSort(arr);
    end = high_resolution_clock::now();

    cout << "\nParallel Bubble Sort:\n";
    printArray(arr);
    cout << "Time: " << duration<double, milli>(end - start).count() << " ms\n";

    return 0;
}