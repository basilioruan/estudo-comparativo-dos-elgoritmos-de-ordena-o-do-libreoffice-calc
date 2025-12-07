#!/usr/bin/env python3
"""
Script para gerar gráficos e análises dos resultados do benchmark
de algoritmos de ordenação.
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from pathlib import Path

def configurar_estilo():
    """Configura o estilo dos gráficos"""
    plt.style.use('default')
    sns.set_palette("husl")
    plt.rcParams['figure.figsize'] = (12, 8)
    plt.rcParams['font.size'] = 12

def carregar_dados(arquivo=None):
    """Carrega os dados do CSV"""
    # Lista de arquivos possíveis em ordem de preferência
    arquivos_possiveis = [
        '../dados/resultados_artigo_oficial.csv',
        'resultados_completos.csv', 
        '../dados/resultados_completos.csv',
        'dados/resultados_artigo_oficial.csv'
    ]
    
    # Se arquivo foi especificado, tentar ele primeiro
    if arquivo:
        arquivos_possiveis.insert(0, arquivo)
    
    for arquivo_teste in arquivos_possiveis:
        try:
            df = pd.read_csv(arquivo_teste)
            print(f"✓ Dados carregados de: {arquivo_teste}")
            print(f"✓ Total de registros: {len(df)}")
            return df
        except FileNotFoundError:
            continue
    
    print("❌ Nenhum arquivo de resultados encontrado!")
    print("Arquivos testados:", arquivos_possiveis)
    return None

def gerar_grafico_comparacao(df, salvar=True):
    """Gera gráfico de comparação de tempo por algoritmo"""
    plt.figure(figsize=(14, 8))
    
    # Filtrar apenas resultados bem-sucedidos
    df_sucesso = df[df['Sucesso'] == 'Sim'].copy()
    
    # Gráfico de linhas
    for algoritmo in df_sucesso['Algoritmo'].unique():
        dados_alg = df_sucesso[df_sucesso['Algoritmo'] == algoritmo]
        plt.plot(dados_alg['Tamanho_Array'], dados_alg['Tempo_ms'], 
                marker='o', linewidth=2, label=algoritmo)
    
    # Configurar eixo X para mostrar todos os tamanhos importantes
    tamanhos_unicos = sorted(df_sucesso['Tamanho_Array'].unique())
    plt.xticks(tamanhos_unicos, [f'{int(x):,}' for x in tamanhos_unicos], rotation=45)
    
    plt.xlabel('Tamanho do Array (elementos)')
    plt.ylabel('Tempo (ms)')
    plt.title('Comparação de Performance - Algoritmos de Ordenação\n(Dados Reais: 94.011 registros)')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.yscale('log')  # Escala logarítmica para melhor visualização
    
    if salvar:
        plt.savefig('grafico_comparacao_algoritmos.png', dpi=300, bbox_inches='tight')
        print("Gráfico salvo: grafico_comparacao_algoritmos.png")
    
    plt.show()

def gerar_grafico_barras(df, tamanho_especifico=None, salvar=True):
    """Gera gráfico de barras para um tamanho específico"""
    if tamanho_especifico is None:
        # Usa o maior tamanho disponível
        tamanho_especifico = df['Tamanho_Array'].max()
    
    df_filtrado = df[(df['Tamanho_Array'] == tamanho_especifico) & 
                     (df['Sucesso'] == 'Sim')]
    
    plt.figure(figsize=(12, 6))
    
    algoritmos = df_filtrado['Algoritmo']
    tempos = df_filtrado['Tempo_ms']
    
    bars = plt.bar(algoritmos, tempos, color=sns.color_palette("husl", len(algoritmos)))
    
    # Adicionar valores nas barras
    for bar, tempo in zip(bars, tempos):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{tempo:.2f}ms', ha='center', va='bottom')
    
    plt.xlabel('Algoritmo')
    plt.ylabel('Tempo (ms)')
    plt.title(f'Comparação de Tempo - Array com {tamanho_especifico:,} elementos')
    plt.xticks(rotation=45)
    plt.tight_layout()
    
    if salvar:
        plt.savefig(f'grafico_barras_{tamanho_especifico}.png', dpi=300, bbox_inches='tight')
        print(f"Gráfico salvo: grafico_barras_{tamanho_especifico}.png")
    
    plt.show()

def gerar_tabela_resumo(df, salvar=True):
    """Gera tabela resumo dos resultados"""
    print("\n=== TABELA RESUMO ===")
    
    # Filtrar apenas sucessos
    df_sucesso = df[df['Sucesso'] == 'Sim'].copy()
    
    # Criar tabela pivot
    tabela = df_sucesso.pivot(index='Tamanho_Array', 
                             columns='Algoritmo', 
                             values='Tempo_ms')
    
    print(tabela.round(2))
    
    if salvar:
        tabela.to_csv('tabela_resumo.csv')
        print("\nTabela salva: tabela_resumo.csv")
    
    # Análise de speedup comparado ao std::sort
    if 'std::sort' in tabela.columns:
        print("\n=== SPEEDUP RELATIVO AO STD::SORT ===")
        speedup_table = tabela.div(tabela['std::sort'], axis=0)
        print(speedup_table.round(2))
        
        if salvar:
            speedup_table.to_csv('speedup_stdSort.csv')
    
    return tabela

def gerar_analise_complexidade(df, salvar=True):
    """Analisa a complexidade dos algoritmos"""
    plt.figure(figsize=(15, 10))
    
    df_sucesso = df[df['Sucesso'] == 'Sim'].copy()
    
    # Subplot para cada algoritmo
    algoritmos = df_sucesso['Algoritmo'].unique()
    n_cols = 2
    n_rows = (len(algoritmos) + 1) // n_cols
    
    for i, algoritmo in enumerate(algoritmos, 1):
        plt.subplot(n_rows, n_cols, i)
        
        dados_alg = df_sucesso[df_sucesso['Algoritmo'] == algoritmo]
        tamanhos = dados_alg['Tamanho_Array']
        tempos = dados_alg['Tempo_ms']
        
        # Plot dos dados reais
        plt.scatter(tamanhos, tempos, alpha=0.7, s=50, label='Dados reais', color='blue')
        
        # Fit de diferentes curvas baseado no algoritmo
        if len(tamanhos) > 2:
            x_fit = np.linspace(tamanhos.min(), tamanhos.max(), 100)
            
            if 'Insertion Sort' in algoritmo:
                # O(n²) para Insertion Sort
                n2_fit = (x_fit ** 2) * (tempos.iloc[0] / (tamanhos.iloc[0] ** 2))
                plt.plot(x_fit, n2_fit, '--', alpha=0.8, color='red', linewidth=2, label='O(n²)')
                plt.title(f'{algoritmo}\n(Complexidade Quadrática)')
            else:
                # O(n log n) para os outros algoritmos
                nlogn_fit = x_fit * np.log2(x_fit) * (tempos.iloc[0] / (tamanhos.iloc[0] * np.log2(tamanhos.iloc[0])))
                plt.plot(x_fit, nlogn_fit, '--', alpha=0.8, color='green', linewidth=2, label='O(n log n)')
                if 'std::sort' in algoritmo:
                    plt.title(f'{algoritmo}\n(Complexidade Logarítmica - OTIMIZADO)')
                else:
                    plt.title(f'{algoritmo}\n(Complexidade Logarítmica)')
        
        plt.xlabel('Tamanho do Array')
        plt.ylabel('Tempo (ms)')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        # Usar escala log para Insertion Sort para visualizar melhor
        if 'Insertion Sort' in algoritmo:
            plt.yscale('log')
            plt.ylabel('Tempo (ms) - Escala Log')
    
    plt.tight_layout()
    
    if salvar:
        plt.savefig('analise_complexidade.png', dpi=300, bbox_inches='tight')
        print("Análise salva: analise_complexidade.png")
    
    plt.show()

def gerar_relatorio_completo(df):
    """Gera relatório completo em texto"""
    with open('relatorio_benchmark.txt', 'w', encoding='utf-8') as f:
        f.write("=== RELATÓRIO DE BENCHMARK - ALGORITMOS DE ORDENAÇÃO ===\n\n")
        
        # Informações gerais
        f.write(f"Total de testes executados: {len(df)}\n")
        f.write(f"Testes bem-sucedidos: {len(df[df['Sucesso'] == 'Sim'])}\n")
        f.write(f"Algoritmos testados: {', '.join(df['Algoritmo'].unique())}\n")
        f.write(f"Tamanhos testados: {sorted(df['Tamanho_Array'].unique())}\n\n")
        
        # Melhor algoritmo por tamanho
        f.write("=== MELHOR ALGORITMO POR TAMANHO ===\n")
        df_sucesso = df[df['Sucesso'] == 'Sim']
        for tamanho in sorted(df_sucesso['Tamanho_Array'].unique()):
            dados_tamanho = df_sucesso[df_sucesso['Tamanho_Array'] == tamanho]
            melhor = dados_tamanho.loc[dados_tamanho['Tempo_ms'].idxmin()]
            f.write(f"Tamanho {tamanho:6d}: {melhor['Algoritmo']:20s} ({melhor['Tempo_ms']:8.2f} ms)\n")
        
        # Ranking geral
        f.write("\n=== RANKING GERAL (média de todos os tamanhos) ===\n")
        ranking = df_sucesso.groupby('Algoritmo')['Tempo_ms'].mean().sort_values()
        for i, (algoritmo, tempo_medio) in enumerate(ranking.items(), 1):
            f.write(f"{i}. {algoritmo:20s}: {tempo_medio:8.2f} ms (média)\n")
        
        f.write("\n=== OBSERVAÇÕES ===\n")
        f.write("- std::sort utiliza Introsort (híbrido: quicksort + heapsort + insertion)\n")
        f.write("- Para arrays pequenos, insertion sort pode ser competitivo\n")
        f.write("- std::sort é otimizado e geralmente superior às implementações manuais\n")
    
    print("Relatório salvo: relatorio_benchmark.txt")

def main():
    """Função principal"""
    configurar_estilo()
    
    # Carregar dados
    df = carregar_dados()
    if df is None:
        return
    
    print("\nDados carregados:")
    print(df.head())
    print(f"\nAlgoritmos: {df['Algoritmo'].unique()}")
    print(f"Tamanhos: {sorted(df['Tamanho_Array'].unique())}")
    
    # Gerar análises
    print("\n1. Gerando gráfico de comparação...")
    gerar_grafico_comparacao(df)
    
    print("\n2. Gerando gráfico de barras...")
    gerar_grafico_barras(df)
    
    print("\n3. Gerando tabela resumo...")
    tabela = gerar_tabela_resumo(df)
    
    print("\n4. Gerando relatório completo...")
    gerar_relatorio_completo(df)
    
    print("\n=== ANÁLISE CONCLUÍDA ===")
    print("Arquivos gerados:")
    print("- grafico_comparacao_algoritmos.png")
    print("- grafico_barras_[tamanho].png")
    print("- tabela_resumo.csv")
    print("- speedup_stdSort.csv")
    print("- analise_complexidade.png")
    print("- relatorio_benchmark.txt")

if __name__ == "__main__":
    main()