# ============================================================
# Makefile — Calculadora Socket (Parte 3)
#
# Uso:
#   make          → compila servidor e cliente
#   make clean    → remove binários
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11

SERVIDOR = calc_servidor
CLIENTE  = calc_cliente

ALL = $(SERVIDOR) $(CLIENTE)

.PHONY: all clean help

all: $(ALL)
	@echo ""
	@echo "✅ Calculadora compilada!"
	@echo ""
	@echo "Para testar:"
	@echo "  Terminal 1:  ./calc_servidor"
	@echo "  Terminal 2:  ./calc_cliente"

$(SERVIDOR): calc_servidor.c
	$(CC) $(CFLAGS) -o $@ $< -lm

$(CLIENTE): calc_cliente.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(ALL)
	@echo "🧹 Binários removidos."

help:
	@echo "Alvos disponíveis:"
	@echo "  make          Compila servidor e cliente"
	@echo "  make clean    Remove binários"
