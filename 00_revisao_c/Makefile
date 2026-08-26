# Makefile para Revisão de C
# ===========================

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Diretórios
SRC_DIR = .
BIN_DIR = bin

# Lista de todos os arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Gera nomes dos executáveis (remove .c e adiciona prefixo bin/)
EXECUTABLES = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SOURCES))

# ===========================
# Regras principais
# ===========================

# Compilar todos os exemplos
all: $(BIN_DIR) $(EXECUTABLES)
	@echo ""
	@echo "✓ Todos os exemplos compilados em ./bin/"
	@echo ""

# Criar diretório bin se não existir
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regra genérica para compilar qualquer .c para bin/
$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -o $@ $<

# ===========================
# Comandos úteis
# ===========================

# Listar todos os exemplos disponíveis
list:
	@echo ""
	@echo "Exemplos disponíveis:"
	@echo "---------------------"
	@for src in $(SOURCES); do \
		name=$$(basename $$src .c); \
		echo "  make run-$$name"; \
	done
	@echo ""
	@echo "Ou compile todos com: make all"
	@echo ""

# Limpar executáveis
clean:
	@echo "Removendo executáveis..."
	rm -rf $(BIN_DIR)
	@echo "✓ Limpo!"

# Recompilar tudo
rebuild: clean all

# ===========================
# Regras para executar exemplos individuais
# ===========================

run-01_vetores_basico: $(BIN_DIR)/01_vetores_basico
	@./$(BIN_DIR)/01_vetores_basico

run-02_vetores_matrizes: $(BIN_DIR)/02_vetores_matrizes
	@./$(BIN_DIR)/02_vetores_matrizes

run-03_loop_for: $(BIN_DIR)/03_loop_for
	@./$(BIN_DIR)/03_loop_for

run-04_loop_while: $(BIN_DIR)/04_loop_while
	@./$(BIN_DIR)/04_loop_while

run-05_loop_do_while: $(BIN_DIR)/05_loop_do_while
	@./$(BIN_DIR)/05_loop_do_while

run-06_loops_aninhados: $(BIN_DIR)/06_loops_aninhados
	@./$(BIN_DIR)/06_loops_aninhados

run-07_break_continue: $(BIN_DIR)/07_break_continue
	@./$(BIN_DIR)/07_break_continue

run-08_ponteiros_basico: $(BIN_DIR)/08_ponteiros_basico
	@./$(BIN_DIR)/08_ponteiros_basico

run-09_aritmetica_ponteiros: $(BIN_DIR)/09_aritmetica_ponteiros
	@./$(BIN_DIR)/09_aritmetica_ponteiros

run-10_passagem_referencia: $(BIN_DIR)/10_passagem_referencia
	@./$(BIN_DIR)/10_passagem_referencia

run-11_vetores_ponteiros: $(BIN_DIR)/11_vetores_ponteiros
	@./$(BIN_DIR)/11_vetores_ponteiros

run-12_alocacao_dinamica: $(BIN_DIR)/12_alocacao_dinamica
	@./$(BIN_DIR)/12_alocacao_dinamica

run-13_ponteiros_de_ponteiros: $(BIN_DIR)/13_ponteiros_de_ponteiros
	@./$(BIN_DIR)/13_ponteiros_de_ponteiros

run-14_matriz_dinamica: $(BIN_DIR)/14_matriz_dinamica
	@./$(BIN_DIR)/14_matriz_dinamica

run-15_vetor_strings: $(BIN_DIR)/15_vetor_strings
	@./$(BIN_DIR)/15_vetor_strings

# ===========================
# Declarar alvos que não são arquivos
# ===========================
.PHONY: all clean rebuild list \
	run-01_vetores_basico run-02_vetores_matrizes run-03_loop_for \
	run-04_loop_while run-05_loop_do_while run-06_loops_aninhados \
	run-07_break_continue run-08_ponteiros_basico run-09_aritmetica_ponteiros \
	run-10_passagem_referencia run-11_vetores_ponteiros run-12_alocacao_dinamica \
	run-13_ponteiros_de_ponteiros run-14_matriz_dinamica run-15_vetor_strings
