# ==============================================================================
#  Makefile - Servidor HTTP em C
# ==============================================================================
#
#  Comandos disponíveis:
#    make          → Compila o servidor
#    make run      → Compila e executa na porta padrão (8080)
#    make run PORT=3000 → Compila e executa na porta 3000
#    make clean    → Remove os arquivos compilados
#    make help     → Exibe ajuda
#
# ==============================================================================

# Nome do executável gerado
TARGET = server

# Compilador C a ser usado
CC = gcc

# Flags de compilação:
#   -Wall     → Ativa todos os warnings comuns
#   -Wextra   → Ativa warnings extras
#   -pedantic → Segue estritamente o padrão C
#   -std=c99  → Usa o padrão C99
#   -g        → Inclui informações de debug (útil para depuração com gdb)
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g

# Porta padrão (pode ser sobrescrita: make run PORT=3000)
PORT = 8080

# Arquivos fonte
SRC = server.c

# ==============================================================================
#  Regras
# ==============================================================================

# Regra padrão: compila o servidor
all: $(TARGET)
	@echo ""
	@echo "✅ Compilação concluída! Executável: ./$(TARGET)"
	@echo "   Execute com: ./$(TARGET) [porta]"
	@echo "   Ou use: make run"
	@echo ""

# Regra de compilação
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Compila e executa o servidor
run: $(TARGET)
	@echo ""
	@echo "🚀 Iniciando servidor na porta $(PORT)..."
	@echo ""
	./$(TARGET) $(PORT)

# Remove arquivos compilados
clean:
	rm -f $(TARGET)
	@echo "🧹 Arquivos compilados removidos."

# Exibe ajuda
help:
	@echo ""
	@echo "===== Servidor HTTP em C - Comandos ====="
	@echo ""
	@echo "  make            Compila o servidor"
	@echo "  make run        Compila e executa (porta 8080)"
	@echo "  make run PORT=N Compila e executa na porta N"
	@echo "  make clean      Remove executável"
	@echo "  make help       Exibe esta ajuda"
	@echo ""

.PHONY: all run clean help
