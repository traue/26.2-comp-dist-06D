# ============================================================
# Makefile — Chat Multicliente (Parte 2)
#
# Uso:
#   make          → compila servidor e cliente
#   make clean    → remove binários
#   make help     → mostra esta ajuda
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11
LDLIBS  = -pthread

BIN_DIR = bin
SERVIDOR = $(BIN_DIR)/chat_servidor
CLIENTE  = $(BIN_DIR)/chat_cliente

ALL = $(SERVIDOR) $(CLIENTE)

.PHONY: all clean help

all: $(ALL)
	@echo ""
	@echo "✅ Chat compilado com sucesso!"
	@echo ""
	@echo "Para testar:"
	@echo "  Terminal 1:  ./bin/chat_servidor"
	@echo "  Terminal 2:  ./bin/chat_cliente"
	@echo "  Terminal 3:  ./bin/chat_cliente"

$(SERVIDOR): chat_servidor.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(CLIENTE): chat_cliente.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)
	@echo "🧹 Binários removidos."

help:
	@echo "Alvos disponíveis:"
	@echo "  make          Compila servidor e cliente do chat em bin/"
	@echo "  make clean    Remove binários"
	@echo "  make help     Mostra esta ajuda"
