/*
 * servidor_tcp.c — Servidor TCP básico
 *
 * Este servidor:
 *   1. Cria um socket TCP
 *   2. Faz bind na porta 8080
 *   3. Escuta conexões
 *   4. Aceita UM cliente
 *   5. Recebe uma mensagem e responde
 *   6. Encerra
 *
 * Compilação: gcc -Wall -Wextra -o servidor servidor_tcp.c
 * Execução:   ./servidor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA 8080
#define BUFFER_SIZE 1024

int main(void)
{
    int server_fd, cliente_fd;
    struct sockaddr_in servidor_addr, cliente_addr;
    socklen_t cliente_len = sizeof(cliente_addr);
    char buffer[BUFFER_SIZE] = {0};
    int opt = 1;

    /* -------------------------------------------------------
     * PASSO 1: Criar o socket
     * AF_INET     = IPv4
     * SOCK_STREAM = TCP
     * 0           = protocolo padrão (TCP neste caso)
     * ------------------------------------------------------- */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }
    printf("[SERVIDOR] Socket criado com sucesso (fd=%d)\n", server_fd);

    /* -------------------------------------------------------
     * PASSO 1.5 (opcional mas recomendado): SO_REUSEADDR
     * Permite reutilizar a porta imediatamente após fechar
     * o servidor. Sem isso, pode dar "Address already in use"
     * ao reiniciar rápido.
     * ------------------------------------------------------- */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Erro no setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /* -------------------------------------------------------
     * PASSO 2: Bind — associar o socket a um endereço
     * INADDR_ANY = aceita conexão em qualquer interface
     * htons()    = converte a porta para network byte order
     * ------------------------------------------------------- */
    memset(&servidor_addr, 0, sizeof(servidor_addr));
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = INADDR_ANY;
    servidor_addr.sin_port = htons(PORTA);

    if (bind(server_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Erro no bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("[SERVIDOR] Bind realizado na porta %d\n", PORTA);

    /* -------------------------------------------------------
     * PASSO 3: Listen — colocar o socket em modo de escuta
     * O "5" é o tamanho da fila de conexões pendentes
     * ------------------------------------------------------- */
    if (listen(server_fd, 5) == -1) {
        perror("Erro no listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("[SERVIDOR] Aguardando conexões na porta %d...\n", PORTA);

    /* -------------------------------------------------------
     * PASSO 4: Accept — aceitar uma conexão
     * Esta chamada BLOQUEIA até que um cliente se conecte.
     * Retorna um NOVO file descriptor para essa conexão.
     * ------------------------------------------------------- */
    cliente_fd = accept(server_fd, (struct sockaddr *)&cliente_addr, &cliente_len);
    if (cliente_fd == -1) {
        perror("Erro no accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("[SERVIDOR] Cliente conectado! IP: %s, Porta: %d\n",
           inet_ntoa(cliente_addr.sin_addr),
           ntohs(cliente_addr.sin_port));

    /* -------------------------------------------------------
     * PASSO 5: Comunicação — receber e enviar dados
     * recv() bloqueia até receber dados ou a conexão fechar
     * ------------------------------------------------------- */
    ssize_t bytes_recebidos = recv(cliente_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_recebidos > 0) {
        buffer[bytes_recebidos] = '\0';  /* garante terminação da string */
        printf("[SERVIDOR] Mensagem recebida (%zd bytes): \"%s\"\n",
               bytes_recebidos, buffer);

        /* Envia resposta ao cliente */
        const char *resposta = "Mensagem recebida com sucesso!";
        send(cliente_fd, resposta, strlen(resposta), 0);
        printf("[SERVIDOR] Resposta enviada ao cliente\n");
    } else if (bytes_recebidos == 0) {
        printf("[SERVIDOR] Cliente desconectou\n");
    } else {
        perror("Erro no recv");
    }

    /* -------------------------------------------------------
     * PASSO 6: Fechar os sockets
     * Sempre feche ambos: o socket do cliente e o do servidor
     * ------------------------------------------------------- */
    close(cliente_fd);
    close(server_fd);
    printf("[SERVIDOR] Encerrado.\n");

    return 0;
}
