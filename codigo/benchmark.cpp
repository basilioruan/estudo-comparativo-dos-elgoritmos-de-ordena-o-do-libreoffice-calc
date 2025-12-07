#include "benchmark.h"
#include "algoritmos_ordenacao.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

ResultadoBenchmark Benchmark::medirTempo(const string& nome, vector<double> dados, 
                                        function<void(vector<double>&)> algoritmo) {
    ResultadoBenchmark resultado;
    resultado.nomeAlgoritmo = nome;
    resultado.tamanhoArray = dados.size();
    resultado.sucesso = false;
    
    try {
        auto inicio = high_resolution_clock::now();
        algoritmo(dados);
        auto fim = high_resolution_clock::now();
        
        auto duracao = duration_cast<microseconds>(fim - inicio);
        resultado.tempoMs = duracao.count() / 1000.0; // Converte para milissegundos
        
        // Verifica se a ordenação foi bem-sucedida
        resultado.sucesso = verificarOrdenacao(dados);
        
        if (!resultado.sucesso) {
            cerr << "ERRO: " << nome << " não ordenou corretamente!" << endl;
        }
        
    } catch (const exception& e) {
        cerr << "Erro durante execução de " << nome << ": " << e.what() << endl;
        resultado.tempoMs = -1;
    }
    
    return resultado;
}

void Benchmark::executarBenchmarkCompleto(const vector<double>& dados, 
                                        vector<ResultadoBenchmark>& resultados) {
    cout << "\n=== INICIANDO BENCHMARK ===" << endl;
    cout << "Tamanho do array: " << dados.size() << " elementos" << endl;
    cout << "================================" << endl;
    
    // Teste Insertion Sort
    cout << "Testando Insertion Sort..." << endl;
    resultados.push_back(medirTempo("Insertion Sort", dados, 
        [](vector<double>& arr) { AlgoritmosOrdenacao::insertionSort(arr); }));
    
    // Teste Shell Sort
    cout << "Testando Shell Sort..." << endl;
    resultados.push_back(medirTempo("Shell Sort", dados,
        [](vector<double>& arr) { AlgoritmosOrdenacao::shellSort(arr); }));
    
    // Teste Quick Sort
    cout << "Testando Quick Sort..." << endl;
    resultados.push_back(medirTempo("Quick Sort", dados,
        [](vector<double>& arr) { AlgoritmosOrdenacao::quickSort(arr); }));
    
    // Teste Introsort (manual)
    cout << "Testando Introsort (manual)..." << endl;
    resultados.push_back(medirTempo("Introsort (manual)", dados,
        [](vector<double>& arr) { AlgoritmosOrdenacao::introsort(arr); }));
    
    // Teste std::sort
    cout << "Testando std::sort..." << endl;
    resultados.push_back(medirTempo("std::sort", dados,
        [](vector<double>& arr) { AlgoritmosOrdenacao::stdSort(arr); }));
    
    cout << "\n=== BENCHMARK CONCLUÍDO ===" << endl;
}

void Benchmark::salvarResultados(const vector<ResultadoBenchmark>& resultados, 
                                const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao criar arquivo de resultados: " << nomeArquivo << endl;
        return;
    }
    
    // Cabeçalho CSV
    arquivo << "Algoritmo,Tempo_ms,Tamanho_Array,Sucesso" << endl;
    
    for (const auto& resultado : resultados) {
        arquivo << resultado.nomeAlgoritmo << ","
                << fixed << setprecision(3) << resultado.tempoMs << ","
                << resultado.tamanhoArray << ","
                << (resultado.sucesso ? "Sim" : "Nao") << endl;
    }
    
    arquivo.close();
    cout << "Resultados salvos em: " << nomeArquivo << endl;
}

void Benchmark::imprimirResultados(const vector<ResultadoBenchmark>& resultados) {
    cout << "\n=== RESULTADOS DO BENCHMARK ===" << endl;
    cout << left << setw(20) << "Algoritmo" 
         << setw(15) << "Tempo (ms)" 
         << setw(15) << "Tamanho" 
         << setw(10) << "Status" << endl;
    cout << string(60, '-') << endl;
    
    for (const auto& resultado : resultados) {
        cout << left << setw(20) << resultado.nomeAlgoritmo
             << setw(15) << fixed << setprecision(3) << resultado.tempoMs
             << setw(15) << resultado.tamanhoArray
             << setw(10) << (resultado.sucesso ? "OK" : "ERRO") << endl;
    }
    
    // Encontra o mais rápido
    auto melhor = min_element(resultados.begin(), resultados.end(),
        [](const ResultadoBenchmark& a, const ResultadoBenchmark& b) {
            return a.sucesso && (!b.sucesso || a.tempoMs < b.tempoMs);
        });
    
    if (melhor != resultados.end() && melhor->sucesso) {
        cout << "\nMelhor algoritmo: " << melhor->nomeAlgoritmo 
             << " (" << melhor->tempoMs << " ms)" << endl;
    }
    
    cout << "================================" << endl;
}

bool Benchmark::verificarOrdenacao(const vector<double>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

string Benchmark::formatarTempo(double tempoMs) {
    if (tempoMs < 1.0) {
        return to_string(tempoMs * 1000) + " μs";
    } else if (tempoMs < 1000.0) {
        return to_string(tempoMs) + " ms";
    } else {
        return to_string(tempoMs / 1000.0) + " s";
    }
}