/*
 * cliente_udp.c — Cliente UDP básico
 *
 * Este cliente:
 *   1. Cria um socket UDP
 *   2. Envia um datagrama ao servidor (sem connect!)
 *   3. Recebe a resposta
 *   4. Encerra
 *
 * Compilação: gcc -Wall -Wextra -o cliente_udp cliente_udp.c
 * Execução:   ./cliente_udp
 *
 * IMPORTANTE: o servidor UDP deve estar rodando antes!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA 9090
#define SERVIDOR_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main(void)
{
    int client_fd;
    struct sockaddr_in servidor_addr;
    socklen_t servidor_len = sizeof(servidor_addr);
    char buffer[BUFFER_SIZE] = {0};

    /* -------------------------------------------------------
     * PASSO 1: Criar o socket UDP
     * ------------------------------------------------------- */
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }
    printf("[CLIENTE UDP] Socket criado (fd=%d)\n", client_fd);

    /* -------------------------------------------------------
     * PASSO 2: Configurar endereço do servidor
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
     * PASSO 3: Enviar datagrama com sendto()
     *
     * Em UDP, NÃO precisamos de connect().
     * Simplesmente enviamos o dado diretamente para o endereço.
     * ------------------------------------------------------- */
    const char *mensagem = "Olá, servidor UDP! Aqui é o cliente.";

    ssize_t bytes_enviados = sendto(client_fd, mensagem, strlen(mensagem), 0,
                                     (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));
    if (bytes_enviados == -1) {
        perror("Erro no sendto");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("[CLIENTE UDP] Mensagem enviada (%zd bytes): \"%s\"\n",
           bytes_enviados, mensagem);

    /* -------------------------------------------------------
     * PASSO 4: Receber resposta com recvfrom()
     * ------------------------------------------------------- */
    ssize_t bytes_recebidos = recvfrom(client_fd, buffer, BUFFER_SIZE - 1, 0,
                                        (struct sockaddr *)&servidor_addr, &servidor_len);
    if (bytes_recebidos > 0) {
        buffer[bytes_recebidos] = '\0';
        printf("[CLIENTE UDP] Resposta do servidor: \"%s\"\n", buffer);
    } else {
        perror("Erro no recvfrom");
    }

    /* -------------------------------------------------------
     * PASSO 5: Fechar
     * ------------------------------------------------------- */
    close(client_fd);
    printf("[CLIENTE UDP] Encerrado.\n");

    return 0;
}
