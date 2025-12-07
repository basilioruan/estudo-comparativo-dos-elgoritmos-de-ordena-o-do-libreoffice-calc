#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <string>
#include <chrono>
#include <functional>

using namespace std;
using namespace chrono;

struct ResultadoBenchmark {
    string nomeAlgoritmo;
    double tempoMs;
    int tamanhoArray;
    bool sucesso;
};

class Benchmark {
public:
    static ResultadoBenchmark medirTempo(const string& nome, vector<double> dados, 
                                       function<void(vector<double>&)> algoritmo);
    
    static void executarBenchmarkCompleto(const vector<double>& dados, 
                                        vector<ResultadoBenchmark>& resultados);
    
    static void salvarResultados(const vector<ResultadoBenchmark>& resultados, 
                               const string& nomeArquivo);
    
    static void imprimirResultados(const vector<ResultadoBenchmark>& resultados);
    
    static bool verificarOrdenacao(const vector<double>& arr);

private:
    static string formatarTempo(double tempoMs);
};

#endif