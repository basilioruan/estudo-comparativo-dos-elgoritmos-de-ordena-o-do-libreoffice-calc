#ifndef CSV_READER_H
#define CSV_READER_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class CSVReader {
public:
    static vector<double> lerArquivoCSV(const string& nomeArquivo, int coluna = 0);
    static bool criarArquivoTeste(const string& nomeArquivo, int numLinhas);
    
private:
    static vector<string> split(const string& str, char delimiter);
    static double stringParaDouble(const string& str);
};

#endif