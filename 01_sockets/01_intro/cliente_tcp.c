/*
 * cliente_tcp.c — Cliente TCP básico
 *
 * Este cliente:
 *   1. Cria um socket TCP
 *   2. Conecta ao servidor em 127.0.0.1:8080
 *   3. Envia uma mensagem
 *   4. Recebe a resposta
 *   5. Encerra
 *
 * Compilação: gcc -Wall -Wextra -o cliente cliente_tcp.c
 * Execução:   ./cliente
 *
 * IMPORTANTE: o servidor deve estar rodando antes de executar o cliente!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA 8080
#define SERVIDOR_IP "127.0.0.1"   /* localhost */
#define BUFFER_SIZE 1024

int main(void)
{
    int client_fd;
    struct sockaddr_in servidor_addr;
    char buffer[BUFFER_SIZE] = {0};

    /* -------------------------------------------------------
     * PASSO 1: Criar o socket
     * Mesma chamada do servidor — a "direção" (cliente/servidor)
     * não muda a criação do socket.
     * ------------------------------------------------------- */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }
    printf("[CLIENTE] Socket criado (fd=%d)\n", client_fd);

    /* -------------------------------------------------------
     * PASSO 2: Configurar o endereço do servidor
     * inet_pton() converte o IP em texto para formato binário
     * ------------------------------------------------------- */
    memset(&servidor_addr, 0, sizeof(servidor_addr));
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(PORTA);

    if (inet_pton(AF_INET, SERVIDOR_IP, &servidor_addr.sin_addr) <= 0) {
        perror("Endereço inválido");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    /* -------------------------------------------------------
     * PASSO 3: Conectar ao servidor
     * Esta chamada BLOQUEIA até a conexão ser estabelecida
     * ou falhar (ex: servidor não está rodando).
     * ------------------------------------------------------- */
    printf("[CLIENTE] Conectando a %s:%d...\n", SERVIDOR_IP, PORTA);

    if (connect(client_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Erro ao conectar");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("[CLIENTE] Conectado ao servidor!\n");

    /* -------------------------------------------------------
     * PASSO 4: Enviar uma mensagem
     * ------------------------------------------------------- */
    const char *mensagem = "Olá, servidor! Aqui é o cliente TCP.";

    ssize_t bytes_enviados = send(client_fd, mensagem, strlen(mensagem), 0);
    if (bytes_enviados == -1) {
        perror("Erro ao enviar");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("[CLIENTE] Mensagem enviada (%zd bytes): \"%s\"\n",
           bytes_enviados, mensagem);

    /* -------------------------------------------------------
     * PASSO 5: Receber a resposta do servidor
     * ------------------------------------------------------- */
    ssize_t bytes_recebidos = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_recebidos > 0) {
        buffer[bytes_recebidos] = '\0';
        printf("[CLIENTE] Resposta do servidor: \"%s\"\n", buffer);
    } else if (bytes_recebidos == 0) {
        printf("[CLIENTE] Servidor fechou a conexão\n");
    } else {
        perror("Erro ao receber");
    }

    /* -------------------------------------------------------
     * PASSO 6: Fechar o socket
     * ------------------------------------------------------- */
    close(client_fd);
    printf("[CLIENTE] Encerrado.\n");

    return 0;
}
