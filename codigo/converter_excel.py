#!/usr/bin/env python3
"""
Script para converter arquivo Excel para CSV para uso no benchmark
"""

import pandas as pd
import sys
import os

def converter_excel_para_csv(arquivo_excel, arquivo_csv=None, coluna_dados=None):
    """
    Converte arquivo Excel para CSV
    
    Args:
        arquivo_excel: Caminho para arquivo .xlsx
        arquivo_csv: Caminho de saída (opcional)
        coluna_dados: Nome ou índice da coluna com dados numéricos
    """
    try:
        # Ler arquivo Excel
        print(f"Lendo arquivo: {arquivo_excel}")
        df = pd.read_excel(arquivo_excel)
        
        print(f"Arquivo carregado: {len(df)} linhas, {len(df.columns)} colunas")
        print(f"Colunas disponíveis: {list(df.columns)}")
        
        # Se não especificou coluna, mostrar informações
        if coluna_dados is None:
            print("\n=== ANÁLISE DAS COLUNAS ===")
            for i, col in enumerate(df.columns):
                tipo_dados = df[col].dtype
                nao_nulos = df[col].notna().sum()
                print(f"{i}: '{col}' - Tipo: {tipo_dados} - Valores válidos: {nao_nulos}")
                
                # Mostrar amostra se for numérico
                if pd.api.types.is_numeric_dtype(df[col]):
                    amostra = df[col].dropna().head(3).tolist()
                    print(f"    Amostra: {amostra}")
            
            print(f"\nPara converter, execute:")
            print(f"python3 converter_excel.py {arquivo_excel} --coluna NOME_DA_COLUNA")
            return None
        
        # Verificar se coluna existe
        if isinstance(coluna_dados, str):
            if coluna_dados not in df.columns:
                print(f"Erro: Coluna '{coluna_dados}' não encontrada!")
                print(f"Colunas disponíveis: {list(df.columns)}")
                return None
        elif isinstance(coluna_dados, int):
            if coluna_dados >= len(df.columns):
                print(f"Erro: Índice {coluna_dados} inválido! Máximo: {len(df.columns)-1}")
                return None
            coluna_dados = df.columns[coluna_dados]
        
        # Filtrar apenas dados numéricos
        print(f"\nProcessando coluna: '{coluna_dados}'")
        dados_numericos = pd.to_numeric(df[coluna_dados], errors='coerce')
        dados_validos = dados_numericos.dropna()
        
        print(f"Dados válidos encontrados: {len(dados_validos)}")
        
        if len(dados_validos) == 0:
            print("Erro: Nenhum dado numérico válido encontrado!")
            return None
        
        # Criar DataFrame para CSV
        csv_data = pd.DataFrame({
            'ID': range(1, len(dados_validos) + 1),
            'Valor': dados_validos.values,
            'Categoria': ['Real'] * len(dados_validos)
        })
        
        # Nome do arquivo de saída
        if arquivo_csv is None:
            base_name = os.path.splitext(os.path.basename(arquivo_excel))[0]
            arquivo_csv = f"../dados/dados_reais_{base_name}.csv"
        
        # Salvar CSV
        csv_data.to_csv(arquivo_csv, index=False)
        
        print(f"Arquivo CSV criado: {arquivo_csv}")
        print(f"Linhas: {len(csv_data)}")
        print(f"Estatísticas:")
        print(f"  Mínimo: {dados_validos.min():.2f}")
        print(f"  Máximo: {dados_validos.max():.2f}")
        print(f"  Média: {dados_validos.mean():.2f}")
        
        return arquivo_csv
        
    except Exception as e:
        print(f"Erro ao processar arquivo: {e}")
        return None

def main():
    if len(sys.argv) < 2:
        print("Uso: python3 converter_excel.py <arquivo.xlsx> [--coluna NOME_OU_INDICE]")
        print("Exemplo: python3 converter_excel.py planilha.xlsx --coluna Vendas")
        print("Exemplo: python3 converter_excel.py planilha.xlsx --coluna 1")
        return
    
    arquivo_excel = sys.argv[1]
    coluna_dados = None
    
    # Verificar argumentos
    if len(sys.argv) > 3 and sys.argv[2] == '--coluna':
        coluna_dados = sys.argv[3]
        # Tentar converter para int se possível
        try:
            coluna_dados = int(coluna_dados)
        except ValueError:
            pass  # Manter como string
    
    if not os.path.exists(arquivo_excel):
        print(f"Erro: Arquivo '{arquivo_excel}' não encontrado!")
        return
    
    converter_excel_para_csv(arquivo_excel, coluna_dados=coluna_dados)

if __name__ == "__main__":
    main()