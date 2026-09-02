/*
 * chat_cliente.c — Cliente de Chat Interativo
 *
 * Funcionalidades:
 *   - Conecta ao servidor de chat
 *   - Envia o nome do usuário ao conectar
 *   - Usa select() para monitorar teclado E socket ao mesmo tempo
 *   - Exibe mensagens recebidas em tempo real
 *   - Comando "sair" para desconectar
 *
 * Compilação: gcc -Wall -Wextra -o chat_cliente chat_cliente.c
 * Execução:   ./chat_cliente
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORTA         8080
#define SERVIDOR_IP   "127.0.0.1"
#define BUFFER_SIZE   2048
#define NOME_SIZE     32

int main(void)
{
    int sock_fd;
    struct sockaddr_in servidor_addr;

    /* -------------------------------------------------------
     * PASSO 1: Pedir o nome do usuário
     * ------------------------------------------------------- */
    char nome[NOME_SIZE];
    printf("========================================\n");
    printf("        CHAT - Cliente\n");
    printf("========================================\n");
    printf("Digite seu nome: ");
    fflush(stdout);

    if (fgets(nome, NOME_SIZE, stdin) == NULL) {
        printf("Erro ao ler nome.\n");
        exit(EXIT_FAILURE);
    }
    /* Remove \n do nome */
    char *newline = strchr(nome, '\n');
    if (newline) *newline = '\0';

    if (strlen(nome) == 0) {
        printf("Nome não pode ser vazio.\n");
        exit(EXIT_FAILURE);
    }

    /* -------------------------------------------------------
     * PASSO 2: Criar socket e conectar ao servidor
     * ------------------------------------------------------- */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    memset(&servidor_addr, 0, sizeof(servidor_addr));
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(PORTA);

    if (inet_pton(AF_INET, SERVIDOR_IP, &servidor_addr.sin_addr) <= 0) {
        perror("Endereço inválido");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Conectando a %s:%d...\n", SERVIDOR_IP, PORTA);

    if (connect(sock_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Erro ao conectar (o servidor está rodando?)");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    /* -------------------------------------------------------
     * PASSO 3: Enviar o nome ao servidor
     *
     * O servidor espera que o primeiro dado recebido seja
     * o nome do usuário. Esse é um "protocolo" simples que
     * definimos entre cliente e servidor.
     * ------------------------------------------------------- */
    send(sock_fd, nome, strlen(nome), 0);

    /* Recebe a mensagem de boas-vindas */
    char buffer[BUFFER_SIZE] = {0};
    ssize_t n = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    printf("----------------------------------------\n");
    printf("  Digite mensagens e pressione Enter.\n");
    printf("  Digite 'sair' para desconectar.\n");
    printf("----------------------------------------\n");

    /* -------------------------------------------------------
     * PASSO 4: Loop principal com select()
     *
     * Monitoramos DOIS file descriptors ao mesmo tempo:
     *   - STDIN_FILENO (0): teclado do usuário
     *   - sock_fd: dados vindos do servidor
     *
     * Sem select(), teríamos que escolher: ou lemos o teclado
     * (e não recebemos mensagens) ou lemos o socket (e não
     * conseguimos digitar). Com select(), fazemos os dois.
     * ------------------------------------------------------- */
    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);   /* monitora teclado */
        FD_SET(sock_fd, &read_fds);         /* monitora socket  */

        /* O maior fd entre STDIN_FILENO (0) e sock_fd */
        int max_fd = (sock_fd > STDIN_FILENO) ? sock_fd : STDIN_FILENO;

        int atividade = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (atividade == -1) {
            if (errno == EINTR) continue;
            perror("Erro no select");
            break;
        }

        /* -------------------------------------------------------
         * CASO 1: Dados no SOCKET → mensagem do servidor
         * (pode ser mensagem de outro cliente ou notificação)
         * ------------------------------------------------------- */
        if (FD_ISSET(sock_fd, &read_fds)) {
            char recv_buf[BUFFER_SIZE] = {0};
            ssize_t bytes = recv(sock_fd, recv_buf, BUFFER_SIZE - 1, 0);

            if (bytes <= 0) {
                /* Servidor desconectou */
                printf("\n[!] Conexão com o servidor perdida.\n");
                break;
            }

            recv_buf[bytes] = '\0';
            /*
             * Limpa a linha atual (onde o usuário pode estar digitando)
             * para exibir a mensagem recebida sem bagunçar o terminal.
             *
             * \r           = volta ao início da linha
             * \033[K       = limpa do cursor até o fim da linha (escape ANSI)
             */
            printf("\r\033[K%s", recv_buf);

            /* Recoloca o prompt para o usuário continuar digitando */
            printf("Você: ");
            fflush(stdout);
        }

        /* -------------------------------------------------------
         * CASO 2: Dados no STDIN → usuário digitou algo
         * ------------------------------------------------------- */
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char input[BUFFER_SIZE] = {0};

            if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
                break;  /* EOF (Ctrl+D) */
            }

            /* Remove \n */
            char *nl = strchr(input, '\n');
            if (nl) *nl = '\0';

            /* Ignora mensagens vazias */
            if (strlen(input) == 0) {
                printf("Você: ");
                fflush(stdout);
                continue;
            }

            /* Comando para sair */
            if (strcmp(input, "sair") == 0) {
                printf("[!] Desconectando...\n");
                break;
            }

            /* Envia a mensagem ao servidor */
            send(sock_fd, input, strlen(input), 0);

            /* Mostra a própria mensagem localmente */
            printf("Você: ");
            fflush(stdout);
        }
    }

    /* -------------------------------------------------------
     * PASSO 5: Limpeza
     * ------------------------------------------------------- */
    close(sock_fd);
    printf("Desconectado. Até mais!\n");

    return 0;
}
