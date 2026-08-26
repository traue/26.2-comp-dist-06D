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
TCP_SERVER = servidor
TCP_CLIENT = cliente
UDP_SERVER = servidor_udp
UDP_CLIENT = cliente_udp

ALL = $(TCP_SERVER) $(TCP_CLIENT) $(UDP_SERVER) $(UDP_CLIENT)

# -------- Regras --------

.PHONY: all tcp udp clean help

all: $(ALL)
	@echo ""
	@echo "✅ Todos os exemplos compilados com sucesso!"
	@echo "   Execute './servidor' e './cliente' em terminais separados para testar TCP."
	@echo "   Execute './servidor_udp' e './cliente_udp' para testar UDP."

tcp: $(TCP_SERVER) $(TCP_CLIENT)
	@echo "✅ Exemplos TCP compilados."

udp: $(UDP_SERVER) $(UDP_CLIENT)
	@echo "✅ Exemplos UDP compilados."

$(TCP_SERVER): servidor_tcp.c
	$(CC) $(CFLAGS) -o $@ $<

$(TCP_CLIENT): cliente_tcp.c
	$(CC) $(CFLAGS) -o $@ $<

$(UDP_SERVER): servidor_udp.c
	$(CC) $(CFLAGS) -o $@ $<

$(UDP_CLIENT): cliente_udp.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(ALL)
	@echo "🧹 Binários removidos."

help:
	@echo "Alvos disponíveis:"
	@echo "  make          Compila todos os exemplos"
	@echo "  make tcp      Compila servidor e cliente TCP"
	@echo "  make udp      Compila servidor e cliente UDP"
	@echo "  make clean    Remove binários"
	@echo "  make help     Mostra esta ajuda"
