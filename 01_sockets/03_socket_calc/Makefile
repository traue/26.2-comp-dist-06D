# ============================================================
# Makefile — Calculadora Socket (Parte 3)
#
# Uso:
#   make          → compila servidor e cliente
#   make clean    → remove binários
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11

BIN_DIR = bin
SERVIDOR = $(BIN_DIR)/calc_servidor
CLIENTE  = $(BIN_DIR)/calc_cliente

ALL = $(SERVIDOR) $(CLIENTE)

.PHONY: all clean help

all: $(ALL)
	@echo ""
	@echo "✅ Calculadora compilada!"
	@echo ""
	@echo "Para testar:"
	@echo "  Terminal 1:  ./bin/calc_servidor"
	@echo "  Terminal 2:  ./bin/calc_cliente"

$(SERVIDOR): calc_servidor.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< -lm

$(CLIENTE): calc_cliente.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)
	@echo "🧹 Binários removidos."

help:
	@echo "Alvos disponíveis:"
	@echo "  make          Compila servidor e cliente em bin/"
	@echo "  make clean    Remove binários"
