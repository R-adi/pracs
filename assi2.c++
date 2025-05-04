#include <iostream>
#include <omp.h>
#include <vector>
#include <ctime>
#include <chrono>

using namespace std;

// Sequential Bubble Sort
void bubbleSortSequential(vector<int> &arr) {
    int n = arr.size();
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    }
}

// Parallel Bubble Sort
void bubbleSortParallel(vector<int> &arr) {
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

// Sequential Merge Sort
void mergeSequential(vector<int> &array, int left, int middle, int right) {
    vector<int> merged;
    int i = left, j = middle + 1;

    while (i <= middle && j <= right) {
        if (array[i] < array[j])
            merged.push_back(array[i++]);
        else
            merged.push_back(array[j++]);
    }

    while (i <= middle) merged.push_back(array[i++]);
    while (j <= right) merged.push_back(array[j++]);

    for (int k = 0; k < merged.size(); k++) {
        array[left + k] = merged[k];
    }
}

void mergeSortSequential(vector<int> &arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSortSequential(arr, left, middle);
        mergeSortSequential(arr, middle + 1, right);
        mergeSequential(arr, left, middle, right);
    }
}

// Parallel Merge Sort
void mergeParallel(vector<int> &array, int left, int middle, int right) {
    mergeSequential(array, left, middle, right); // Reuse same merge logic
}

void mergeSortParallel(vector<int> &arr, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, left, middle);
            #pragma omp section
            mergeSortParallel(arr, middle + 1, right);
        }
        mergeParallel(arr, left, middle, right);
    }
}

// Utility to print vector
void printArray(const vector<int> &arr) {
    for (int num : arr) cout << num << " ";
    cout << endl;
}

// Main driver
int main() {
    vector<int> original = {14, 12, 8, 7, 10};

    vector<int> bubbleSeq = original;
    vector<int> bubblePar = original;
    vector<int> mergeSeq = original;
    vector<int> mergePar = original;

    cout << "Original array:\n";
    printArray(original);

    // Bubble Sort (Sequential)
    clock_t bseq_start = clock();
    bubbleSortSequential(bubbleSeq);
    clock_t bseq_end = clock();

auto start = chrono::high_resolution_clock::now();
    bubbleSortParallel(bubblePar);
    auto end = chrono::high_resolution_clock::now();
chrono::duration<double> duration = end - start;
cout << "Time: " << duration.count() << " seconds\n";
    // Bubble Sort (Parallel)
    clock_t bpar_start = clock();
    bubbleSortParallel(bubblePar);
    clock_t bpar_end = clock();

    // Merge Sort (Sequential)
    clock_t mseq_start = clock();
    mergeSortSequential(mergeSeq, 0, mergeSeq.size() - 1);
    clock_t mseq_end = clock();

    // Merge Sort (Parallel)
    clock_t mpar_start = clock();
    mergeSortParallel(mergePar, 0, mergePar.size() - 1);
    clock_t mpar_end = clock();

    cout << "\nSorted Arrays:\n";
    cout << "Bubble Sort (Sequential): ";
    printArray(bubbleSeq);
    cout << "Bubble Sort (Parallel):   ";
    printArray(bubblePar);
    cout << "Merge Sort (Sequential):  ";
    printArray(mergeSeq);
    cout << "Merge Sort (Parallel):    ";
    printArray(mergePar);

    // Time Results
    cout << "\nExecution Times:\n";
    double p=(bseq_end - bseq_start)  ;
    cout << "Bubble Sort Sequential: " <<p << " seconds\n";
    cout << "Bubble Sort Parallel:   " << double(bpar_end - bpar_start) / CLOCKS_PER_SEC << " seconds\n";
    cout << "Merge Sort Sequential:  " << double(mseq_end - mseq_start) / CLOCKS_PER_SEC << " seconds\n";
    cout << "Merge Sort Parallel:    " << double(mpar_end - mpar_start) / CLOCKS_PER_SEC << " seconds\n";

    return 0;
}
