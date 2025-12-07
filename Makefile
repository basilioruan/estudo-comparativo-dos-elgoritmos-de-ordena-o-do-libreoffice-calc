# Makefile para o projeto de comparação de algoritmos de ordenação

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET = benchmark_ordenacao
TARGET_REAL = benchmark_planilha_real
TARGET_OFICIAL = benchmark_oficial
TARGET_SIMPLES = benchmark_oficial_simples
SOURCES = codigo/main.cpp codigo/algoritmos_ordenacao.cpp codigo/csv_reader.cpp codigo/benchmark.cpp
SOURCES_REAL = codigo/benchmark_planilha_real.cpp codigo/algoritmos_ordenacao.cpp codigo/csv_reader.cpp codigo/benchmark.cpp
SOURCES_OFICIAL = codigo/main_oficial.cpp codigo/algoritmos_ordenacao.cpp codigo/csv_reader.cpp codigo/benchmark.cpp codigo/config.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Regra principal
all: $(TARGET) $(TARGET_REAL) $(TARGET_SIMPLES)

# Compilação do executável principal
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compilação do benchmark para planilha real
$(TARGET_REAL): codigo/benchmark_planilha_real.o codigo/algoritmos_ordenacao.o codigo/csv_reader.o codigo/benchmark.o
	$(CXX) $(CXXFLAGS) -o $(TARGET_REAL) codigo/benchmark_planilha_real.o codigo/algoritmos_ordenacao.o codigo/csv_reader.o codigo/benchmark.o

# Compilação do benchmark oficial simples
$(TARGET_SIMPLES): codigo/main_oficial_simples.o codigo/algoritmos_ordenacao.o codigo/csv_reader.o codigo/benchmark.o
	$(CXX) $(CXXFLAGS) -o $(TARGET_SIMPLES) codigo/main_oficial_simples.o codigo/algoritmos_ordenacao.o codigo/csv_reader.o codigo/benchmark.o

# Compilação dos arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f codigo/*.o $(TARGET) $(TARGET_REAL) dados/*.csv tmp_rovodev_*

# Execução com dados de teste
test: $(TARGET)
	./$(TARGET)

# Execução completa automatizada
auto-test: $(TARGET)
	@echo "Criando arquivo de teste com 10000 elementos..."
	@echo -e "1\n10000\ndados_teste.csv" | ./$(TARGET)
	@echo "Executando benchmark..."
	@echo -e "2\ndados_teste.csv\n1" | ./$(TARGET)

# Geração de gráficos (requer Python)
graficos: dados/resultados_completos.csv
	cd codigo && python3 gerar_graficos.py && cd ..

# Instalação de dependências Python
install-deps:
	pip3 install matplotlib pandas numpy seaborn

# Teste com planilha real
test-real: $(TARGET_REAL)
	./$(TARGET_REAL)

# Teste oficial para artigo
test-oficial: $(TARGET_OFICIAL) preparar-dados-oficiais
	./$(TARGET_OFICIAL)

# Preparar dados da planilha oficial
preparar-dados-oficiais:
	@echo "Preparando dados da planilha oficial..."
	cd codigo && python3 converter_excel.py ../dados/Relatorio_Geral_Inventario_2024_Dump.xlsx --coluna 1

# Converter Excel para CSV
convert-excel:
	@echo "Para converter Excel para CSV:"
	@echo "cd codigo && python3 converter_excel.py sua_planilha.xlsx"

# Ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make               - Compila ambos os programas"
	@echo "  make test          - Executa benchmark com dados sintéticos"
	@echo "  make test-real     - Executa benchmark com planilha real"
	@echo "  make auto-test     - Executa teste automatizado completo"
	@echo "  make convert-excel - Mostra como converter Excel para CSV"
	@echo "  make graficos      - Gera gráficos (requer Python)"
	@echo "  make install-deps  - Instala dependências Python"
	@echo "  make clean         - Remove arquivos compilados"

.PHONY: all clean test auto-test graficos install-deps help