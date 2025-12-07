#!/bin/bash

# Script para executar benchmark oficial para o artigo
echo "=== BENCHMARK OFICIAL PARA ARTIGO CIENTÍFICO ==="
echo "Planilha: Relatorio_Geral_Inventario_2024_Dump.xlsx (94.011 registros)"
echo ""

# Verificar se arquivos existem
if [ ! -f "dados/Relatorio_Geral_Inventario_2024_Dump.xlsx" ]; then
    echo "ERRO: Planilha oficial não encontrada!"
    echo "Certifique-se que o arquivo está em: dados/Relatorio_Geral_Inventario_2024_Dump.xlsx"
    exit 1
fi

# Compilar se necessário
if [ ! -f "benchmark_oficial_simples" ]; then
    echo "Compilando benchmark oficial..."
    make clean && make
    if [ $? -ne 0 ]; then
        echo "Erro na compilação!"
        exit 1
    fi
fi

# Executar benchmark
echo "Executando benchmark oficial..."
echo "IMPORTANTE: Os resultados serão usados no artigo científico!"
echo ""
./benchmark_oficial_simples

echo ""
echo "=== BENCHMARK OFICIAL CONCLUÍDO ==="
echo ""
echo "Arquivos gerados:"
echo "  dados/resultados_artigo_oficial.csv"
echo "  dados/metadados_benchmark_oficial.txt"
echo ""
echo "Para gerar gráficos:"
echo "  cd codigo && python3 gerar_graficos.py ../dados/resultados_artigo_oficial.csv"
echo ""
echo "Para ver resultados:"
echo "  cat dados/metadados_benchmark_oficial.txt"