#include "algoritmos_ordenacao.h"
#include <iostream>

using namespace std;

// Insertion Sort
void AlgoritmosOrdenacao::insertionSort(vector<double>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        double key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Shell Sort
void AlgoritmosOrdenacao::shellSort(vector<double>& arr) {
    int n = arr.size();
    
    // Sequência de gaps (Knuth)
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            double temp = arr[i];
            int j;
            
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

// Quick Sort - função principal
void AlgoritmosOrdenacao::quickSort(vector<double>& arr) {
    if (arr.size() <= 1) return;
    quickSort(arr, 0, arr.size() - 1);
}

// Quick Sort - recursivo
void AlgoritmosOrdenacao::quickSort(vector<double>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Partição para Quick Sort
int AlgoritmosOrdenacao::partition(vector<double>& arr, int low, int high) {
    double pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Introsort - implementação manual
void AlgoritmosOrdenacao::introsort(vector<double>& arr) {
    if (arr.size() <= 1) return;
    
    int depthLimit = calculateDepthLimit(arr.size());
    introsortUtil(arr, 0, arr.size() - 1, depthLimit);
}

void AlgoritmosOrdenacao::introsortUtil(vector<double>& arr, int begin, int end, int depthLimit) {
    int size = end - begin + 1;
    
    // Se o tamanho for pequeno, usa insertion sort
    if (size <= 16) {
        vector<double> subArr(arr.begin() + begin, arr.begin() + end + 1);
        insertionSort(subArr);
        for (int i = 0; i < subArr.size(); i++) {
            arr[begin + i] = subArr[i];
        }
        return;
    }
    
    // Se a profundidade limite foi atingida, usa heap sort
    if (depthLimit == 0) {
        heapSort(arr, begin, end);
        return;
    }
    
    // Caso contrário, usa quick sort
    int pivot = partition(arr, begin, end);
    introsortUtil(arr, begin, pivot - 1, depthLimit - 1);
    introsortUtil(arr, pivot + 1, end, depthLimit - 1);
}

void AlgoritmosOrdenacao::heapSort(vector<double>& arr, int begin, int end) {
    int n = end - begin + 1;
    
    // Constrói o heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, begin);
    }
    
    // Extrai elementos do heap
    for (int i = n - 1; i > 0; i--) {
        swap(arr[begin], arr[begin + i]);
        heapify(arr, i, 0, begin);
    }
}

void AlgoritmosOrdenacao::heapify(vector<double>& arr, int n, int i, int begin) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[begin + left] > arr[begin + largest])
        largest = left;
    
    if (right < n && arr[begin + right] > arr[begin + largest])
        largest = right;
    
    if (largest != i) {
        swap(arr[begin + i], arr[begin + largest]);
        heapify(arr, n, largest, begin);
    }
}

int AlgoritmosOrdenacao::calculateDepthLimit(int n) {
    return 2 * static_cast<int>(log2(n));
}

// std::sort para comparação
void AlgoritmosOrdenacao::stdSort(vector<double>& arr) {
    sort(arr.begin(), arr.end());
}