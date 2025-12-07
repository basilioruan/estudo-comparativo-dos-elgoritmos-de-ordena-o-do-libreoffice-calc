#include "csv_reader.h"
#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>

using namespace std;

vector<double> CSVReader::lerArquivoCSV(const string& nomeArquivo, int coluna) {
    vector<double> dados;
    ifstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir arquivo: " << nomeArquivo << endl;
        return dados;
    }
    
    string linha;
    bool primeiraLinha = true;
    
    while (getline(arquivo, linha)) {
        // Pula cabeçalho se existir
        if (primeiraLinha && linha.find_first_not_of("0123456789.,-") != string::npos) {
            primeiraLinha = false;
            continue;
        }
        primeiraLinha = false;
        
        vector<string> campos = split(linha, ',');
        
        if (campos.size() > coluna) {
            try {
                double valor = stringParaDouble(campos[coluna]);
                dados.push_back(valor);
            } catch (const exception& e) {
                cerr << "Erro ao converter valor: " << campos[coluna] << endl;
            }
        }
    }
    
    arquivo.close();
    cout << "Lidos " << dados.size() << " valores do arquivo " << nomeArquivo << endl;
    return dados;
}

bool CSVReader::criarArquivoTeste(const string& nomeArquivo, int numLinhas) {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao criar arquivo: " << nomeArquivo << endl;
        return false;
    }
    
    // Cabeçalho
    arquivo << "ID,Valor,Categoria" << endl;
    
    // Gerador de números aleatórios
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(1.0, 100000.0);
    
    // Gera dados aleatórios
    for (int i = 0; i < numLinhas; i++) {
        double valor = dist(gen);
        arquivo << i + 1 << "," << fixed << setprecision(2) << valor << ",Cat" << (i % 10) << endl;
    }
    
    arquivo.close();
    cout << "Arquivo " << nomeArquivo << " criado com " << numLinhas << " linhas" << endl;
    return true;
}

vector<string> CSVReader::split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

double CSVReader::stringParaDouble(const string& str) {
    try {
        return stod(str);
    } catch (const invalid_argument& e) {
        throw runtime_error("Valor inválido: " + str);
    }
}