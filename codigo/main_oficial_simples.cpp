#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "algoritmos_ordenacao.h"
#include "benchmark.h"
#include "csv_reader.h"

using namespace std;

// Configurações para a planilha oficial
const string PLANILHA_OFICIAL =
    "../dados/Relatorio_Geral_Inventario_2024_Dump.xlsx";
const string CSV_OFICIAL =
    "../dados/dados_reais_Relatorio_Geral_Inventario_2024_Dump.csv";

// Colunas disponíveis (baseado na análise)
struct ColunaInfo {
  int indice;
  string nome;
  string descricao;
};

const ColunaInfo COLUNAS_NUMERICAS[] = {
    {1, "CÓDIGO", "Códigos de inventário (94011 valores)"},
    {11, "CÓD. CENTRO DE CUSTO", "Códigos de centro de custo (94011 valores)"},
    {15, "AUX. 1", "Campo auxiliar 1 (702 valores)"},
    {17, "LATITUDE", "Coordenadas de latitude (1348 valores)"},
    {18, "LONGITUDE", "Coordenadas de longitude (1348 valores)"}};

const int NUM_COLUNAS = 5;

void mostrarColunas() {
  cout << "\n=== COLUNAS NUMÉRICAS DISPONÍVEIS ===" << endl;
  cout << "Planilha: Relatorio_Geral_Inventario_2024_Dump.xlsx" << endl;
  cout << "Total de registros: 94.011" << endl;
  cout << "\nColunas recomendadas para ordenação:" << endl;

  for (int i = 0; i < NUM_COLUNAS; i++) {
    cout << i + 1 << ". " << COLUNAS_NUMERICAS[i].nome << " - "
         << COLUNAS_NUMERICAS[i].descricao << endl;
  }

  cout << "\nRecomendação: Use CÓDIGO (opção 1) para melhor representatividade"
       << endl;
  cout << "Escolha uma opção (1-" << NUM_COLUNAS << "): ";
}

void executarBenchmarkOficial(int opcaoColuna) {
  if (opcaoColuna < 1 || opcaoColuna > NUM_COLUNAS) {
    cout << "Opção inválida!" << endl;
    return;
  }

  ColunaInfo coluna = COLUNAS_NUMERICAS[opcaoColuna - 1];

  cout << "\n=== CONFIGURAÇÃO DO BENCHMARK OFICIAL ===" << endl;
  cout << "Coluna selecionada: " << coluna.nome << endl;
  cout << "Descrição: " << coluna.descricao << endl;

  // Verificar se CSV já existe
  ifstream teste(CSV_OFICIAL);
  if (!teste.good()) {
    cout << "\nConvertendo planilha Excel para CSV..." << endl;
    string comando = "cd codigo && python3 converter_excel.py " +
                     PLANILHA_OFICIAL + " --coluna " + to_string(coluna.indice);
    int resultado = system(comando.c_str());

    if (resultado != 0) {
      cout << "Erro na conversão! Execute manualmente:" << endl;
      cout << comando << endl;
      return;
    }
  }

  // Carregar dados
  cout << "\nCarregando dados oficiais..." << endl;

  // Tentar diferentes caminhos
  vector<string> caminhosPossiveis = {
      CSV_OFICIAL, "dados/dados_reais_Relatorio_Geral_Inventario_2024_Dump.csv",
      "../dados/dados_reais_Relatorio_Geral_Inventario_2024_Dump.csv"};

  vector<double> dados;
  string arquivoEncontrado = "";

  for (const string& caminho : caminhosPossiveis) {
    dados = CSVReader::lerArquivoCSV(caminho, 1);  // Coluna "Valor" no CSV
    if (!dados.empty()) {
      arquivoEncontrado = caminho;
      cout << "Arquivo encontrado: " << caminho << endl;
      break;
    }
  }

  if (dados.empty()) {
    cout << "Erro: Não foi possível carregar dados!" << endl;
    return;
  }

  cout << "Dados carregados: " << dados.size() << " registros" << endl;

  // Análise dos dados
  cout << "\n=== ANÁLISE DOS DADOS OFICIAIS ===" << endl;
  double minimo = *min_element(dados.begin(), dados.end());
  double maximo = *max_element(dados.begin(), dados.end());
  double soma = 0;
  for (double valor : dados) soma += valor;
  double media = soma / dados.size();

  cout << "Coluna: " << coluna.nome << endl;
  cout << "Registros: " << dados.size() << endl;
  cout << "Mínimo: " << minimo << endl;
  cout << "Máximo: " << maximo << endl;
  cout << "Média: " << media << endl;

  // Tamanhos de teste - usar máximo da planilha real
  vector<int> tamanhos = {1000,  5000,  10000, 25000,
                          50000, 75000, 90000, dados.size()};

  cout << "\n=== EXECUTANDO BENCHMARK OFICIAL ===" << endl;

  vector<ResultadoBenchmark> todosResultados;

  for (int tamanho : tamanhos) {
    if (tamanho > static_cast<int>(dados.size())) continue;

    cout << "\n--- Testando " << tamanho << " elementos ---" << endl;
    vector<double> subconjunto(dados.begin(), dados.begin() + tamanho);

    vector<ResultadoBenchmark> resultados;
    Benchmark::executarBenchmarkCompleto(subconjunto, resultados);

    for (auto& resultado : resultados) {
      todosResultados.push_back(resultado);
    }
  }

  // Salvar resultados oficiais
  cout << "\n=== SALVANDO RESULTADOS OFICIAIS ===" << endl;
  Benchmark::imprimirResultados(todosResultados);

  string arquivoResultados = "dados/resultados_artigo_oficial.csv";
  Benchmark::salvarResultados(todosResultados, arquivoResultados);

  // Criar metadados
  ofstream meta("dados/metadados_benchmark_oficial.txt");
  meta << "BENCHMARK OFICIAL PARA ARTIGO CIENTÍFICO\n";
  meta << "=======================================\n\n";
  meta << "Planilha: Relatorio_Geral_Inventario_2024_Dump.xlsx\n";
  meta << "Coluna utilizada: " << coluna.nome << " (índice " << coluna.indice
       << ")\n";
  meta << "Descrição: " << coluna.descricao << "\n";
  meta << "Total de registros: " << dados.size() << "\n";
  meta << "Valor mínimo: " << minimo << "\n";
  meta << "Valor máximo: " << maximo << "\n";
  meta << "Valor médio: " << media << "\n";
  meta << "Tamanhos testados: ";
  for (size_t i = 0; i < tamanhos.size(); i++) {
    meta << tamanhos[i];
    if (i < tamanhos.size() - 1) meta << ", ";
  }
  meta << "\n";
  meta << "Data/Hora: " << __DATE__ << " às " << __TIME__ << "\n";
  meta.close();

  cout << "\n=== ARQUIVOS GERADOS ===" << endl;
  cout << "✓ Resultados: " << arquivoResultados << endl;
  cout << "✓ Metadados: ../dados/metadados_benchmark_oficial.txt" << endl;
  cout << "\nPara gerar gráficos oficiais:" << endl;
  cout << "cd codigo && python3 gerar_graficos.py " << arquivoResultados
       << endl;

  cout << "\n🎯 BENCHMARK OFICIAL CONCLUÍDO!" << endl;
  cout << "Os resultados estão prontos para o artigo científico." << endl;
}

int main() {
  cout << "=== BENCHMARK OFICIAL PARA ARTIGO CIENTÍFICO ===" << endl;
  cout << "Planilha: Relatorio_Geral_Inventario_2024_Dump.xlsx" << endl;
  cout << "Registros: 94.011 linhas de dados reais de inventário" << endl;

  mostrarColunas();

  int opcao;
  cin >> opcao;

  executarBenchmarkOficial(opcao);

  return 0;
}