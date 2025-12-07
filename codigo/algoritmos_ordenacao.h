#ifndef ALGORITMOS_ORDENACAO_H
#define ALGORITMOS_ORDENACAO_H

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class AlgoritmosOrdenacao {
public:
    // Insertion Sort
    static void insertionSort(vector<double>& arr);
    
    // Shell Sort
    static void shellSort(vector<double>& arr);
    
    // Quick Sort
    static void quickSort(vector<double>& arr, int low, int high);
    static void quickSort(vector<double>& arr);
    
    // Introsort (implementação manual)
    static void introsort(vector<double>& arr);
    
    // std::sort (para comparação)
    static void stdSort(vector<double>& arr);

private:
    // Funções auxiliares para Quick Sort
    static int partition(vector<double>& arr, int low, int high);
    
    // Funções auxiliares para Introsort
    static void introsortUtil(vector<double>& arr, int begin, int end, int depthLimit);
    static void heapSort(vector<double>& arr, int begin, int end);
    static void heapify(vector<double>& arr, int n, int i, int begin);
    static int calculateDepthLimit(int n);
};

#endif