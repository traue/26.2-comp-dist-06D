/*
 * calc_cliente.c — Cliente da Calculadora Remota
 *
 * O cliente:
 *   1. Conecta ao servidor
 *   2. Recebe o banner/prompt do servidor
 *   3. Lê expressões do teclado e envia ao servidor
 *   4. Exibe o resultado recebido
 *   5. Repete até o usuário digitar "sair"
 *
 * Compilação: gcc -Wall -Wextra -o calc_cliente calc_cliente.c
 * Execução:   ./calc_cliente
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA         9090
#define SERVIDOR_IP   "127.0.0.1"
#define BUFFER_SIZE   1024

/*
 * Função auxiliar: recebe dados do servidor e imprime.
 * Usamos uma pequena espera (recv com timeout curto)
 * para capturar toda a resposta do servidor antes de
 * pedir a próxima entrada do usuário.
 */
void receber_e_imprimir(int sock_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes;

    /*
     * Configura um timeout curto no recv().
     * Sem isso, recv() bloquearia indefinidamente esperando
     * mais dados do servidor.
     *
     * 200ms é suficiente para capturar a resposta completa
     * em uma conexão local.
     */
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 200000;  /* 200ms */
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        bytes = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("%s", buffer);

            /*
             * Se a resposta termina com "> " (prompt do servidor),
             * é hora de ler a entrada do usuário.
             */
            if (bytes >= 2 && buffer[bytes - 2] == '>' && buffer[bytes - 1] == ' ') {
                break;
            }
        } else {
            /* Timeout ou erro — já recebemos tudo */
            break;
        }
    }

    /* Remove o timeout para não afetar operações futuras */
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

int main(void)
{
    int sock_fd;
    struct sockaddr_in servidor_addr;

    /* Criar socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    /* Configurar endereço do servidor */
    memset(&servidor_addr, 0, sizeof(servidor_addr));
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(PORTA);

    if (inet_pton(AF_INET, SERVIDOR_IP, &servidor_addr.sin_addr) <= 0) {
        perror("Endereço inválido");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    /* Conectar */
    printf("Conectando a %s:%d...\n", SERVIDOR_IP, PORTA);

    if (connect(sock_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Erro ao conectar (o servidor está rodando?)");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    /* Recebe e exibe o banner do servidor + primeiro prompt */
    receber_e_imprimir(sock_fd);

    /* Loop principal: lê expressões e envia ao servidor */
    while (1) {
        char input[BUFFER_SIZE] = {0};
        fflush(stdout);

        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            break;  /* EOF (Ctrl+D) */
        }

        /* Remove espaços/\n do final */
        size_t len = strlen(input);
        while (len > 0 && (input[len - 1] == '\n' || input[len - 1] == ' ')) {
            input[--len] = '\0';
        }

        /* Ignora linhas vazias */
        if (len == 0) {
            printf("calc> ");
            continue;
        }

        /* Envia ao servidor (com \n para facilitar o parse) */
        char envio[BUFFER_SIZE];
        snprintf(envio, sizeof(envio), "%s\n", input);
        send(sock_fd, envio, strlen(envio), 0);

        /* Se pediu para sair, recebe a despedida e encerra */
        if (strcmp(input, "sair") == 0 || strcmp(input, "exit") == 0 ||
            strcmp(input, "quit") == 0) {
            char buf[BUFFER_SIZE] = {0};
            ssize_t n = recv(sock_fd, buf, BUFFER_SIZE - 1, 0);
            if (n > 0) {
                buf[n] = '\0';
                printf("%s", buf);
            }
            break;
        }

        /* Recebe e exibe o resultado + próximo prompt */
        receber_e_imprimir(sock_fd);
    }

    close(sock_fd);
    return 0;
}
