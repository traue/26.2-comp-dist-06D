# ============================================================
# Makefile — Compilação dos exemplos de Sockets (Parte 1)
#
# Uso:
#   make          → compila todos os exemplos
#   make tcp      → compila apenas servidor e cliente TCP
#   make udp      → compila apenas servidor e cliente UDP
#   make clean    → remove todos os binários gerados
#   make help     → mostra esta ajuda
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11

# Alvos
BIN_DIR = bin
TCP_SERVER = $(BIN_DIR)/servidor
TCP_CLIENT = $(BIN_DIR)/cliente
UDP_SERVER = $(BIN_DIR)/servidor_udp
UDP_CLIENT = $(BIN_DIR)/cliente_udp

ALL = $(TCP_SERVER) $(TCP_CLIENT) $(UDP_SERVER) $(UDP_CLIENT)

# -------- Regras --------

.PHONY: all tcp udp clean help

all: $(ALL)
	@echo ""
	@echo "✅ Todos os exemplos compilados com sucesso!"
	@echo "   Execute './bin/servidor' e './bin/cliente' em terminais separados para testar TCP."
	@echo "   Execute './bin/servidor_udp' e './bin/cliente_udp' para testar UDP."

tcp: $(TCP_SERVER) $(TCP_CLIENT)
	@echo "✅ Exemplos TCP compilados."

udp: $(UDP_SERVER) $(UDP_CLIENT)
	@echo "✅ Exemplos UDP compilados."

$(TCP_SERVER): servidor_tcp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(TCP_CLIENT): cliente_tcp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(UDP_SERVER): servidor_udp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(UDP_CLIENT): cliente_udp.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)
	@echo "🧹 Binários removidos."

help:
	@echo "Alvos disponíveis:"
	@echo "  make          Compila todos os exemplos em bin/"
	@echo "  make tcp      Compila servidor e cliente TCP"
	@echo "  make udp      Compila servidor e cliente UDP"
	@echo "  make clean    Remove binários"
	@echo "  make help     Mostra esta ajuda"
