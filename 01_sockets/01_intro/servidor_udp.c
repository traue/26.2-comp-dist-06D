/*
 * servidor_udp.c — Servidor UDP básico
 *
 * Diferenças em relação ao TCP:
 *   - Não precisa de listen() nem accept()
 *   - Usa recvfrom() e sendto() ao invés de recv() e send()
 *   - Cada mensagem é independente (datagrama)
 *
 * Compilação: gcc -Wall -Wextra -o servidor_udp servidor_udp.c
 * Execução:   ./servidor_udp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA 9090
#define BUFFER_SIZE 1024

int main(void)
{
    int server_fd;
    struct sockaddr_in servidor_addr, cliente_addr;
    socklen_t cliente_len = sizeof(cliente_addr);
    char buffer[BUFFER_SIZE] = {0};

    /* -------------------------------------------------------
     * PASSO 1: Criar o socket
     * Note: SOCK_DGRAM ao invés de SOCK_STREAM
     * ------------------------------------------------------- */
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }
    printf("[SERVIDOR UDP] Socket criado (fd=%d)\n", server_fd);

    /* -------------------------------------------------------
     * PASSO 2: Bind — mesma lógica do TCP
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
    printf("[SERVIDOR UDP] Escutando na porta %d...\n", PORTA);

    /* -------------------------------------------------------
     * PASSO 3: Receber dados com recvfrom()
     *
     * Diferente do TCP, aqui NÃO tem listen/accept.
     * recvfrom() recebe o datagrama E o endereço de quem enviou.
     * Isso é necessário porque não há conexão estabelecida —
     * precisamos saber para quem responder.
     * ------------------------------------------------------- */
    ssize_t bytes_recebidos = recvfrom(server_fd, buffer, BUFFER_SIZE - 1, 0,
                                        (struct sockaddr *)&cliente_addr, &cliente_len);
    if (bytes_recebidos == -1) {
        perror("Erro no recvfrom");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_recebidos] = '\0';
    printf("[SERVIDOR UDP] Mensagem de %s:%d (%zd bytes): \"%s\"\n",
           inet_ntoa(cliente_addr.sin_addr),
           ntohs(cliente_addr.sin_port),
           bytes_recebidos, buffer);

    /* -------------------------------------------------------
     * PASSO 4: Responder com sendto()
     *
     * sendto() precisa do endereço de destino porque, como
     * não há conexão, o socket não "sabe" para quem enviar.
     * ------------------------------------------------------- */
    const char *resposta = "Datagrama recebido com sucesso!";
    ssize_t bytes_enviados = sendto(server_fd, resposta, strlen(resposta), 0,
                                     (struct sockaddr *)&cliente_addr, cliente_len);
    if (bytes_enviados == -1) {
        perror("Erro no sendto");
    } else {
        printf("[SERVIDOR UDP] Resposta enviada (%zd bytes)\n", bytes_enviados);
    }

    /* -------------------------------------------------------
     * PASSO 5: Fechar
     * ------------------------------------------------------- */
    close(server_fd);
    printf("[SERVIDOR UDP] Encerrado.\n");

    return 0;
}
