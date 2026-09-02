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

SERVIDOR = chat_servidor
CLIENTE  = chat_cliente

ALL = $(SERVIDOR) $(CLIENTE)

.PHONY: all clean help

all: $(ALL)
	@echo ""
	@echo "✅ Chat compilado com sucesso!"
	@echo ""
	@echo "Para testar:"
	@echo "  Terminal 1:  ./chat_servidor"
	@echo "  Terminal 2:  ./chat_cliente"
	@echo "  Terminal 3:  ./chat_cliente"

$(SERVIDOR): chat_servidor.c
	$(CC) $(CFLAGS) -o $@ $<

$(CLIENTE): chat_cliente.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(ALL)
	@echo "🧹 Binários removidos."

help:
	@echo "Alvos disponíveis:"
	@echo "  make          Compila servidor e cliente do chat"
	@echo "  make clean    Remove binários"
	@echo "  make help     Mostra esta ajuda"
