/*
 * chat_servidor.c — Servidor de Chat Multicliente
 *
 * Funcionalidades:
 *   - Aceita múltiplos clientes simultâneos via select()
 *   - Cada cliente informa seu nome ao conectar
 *   - Retransmite (broadcast) mensagens para todos os outros
 *   - Notifica entrada e saída de participantes
 *
 * Compilação: gcc -Wall -Wextra -o chat_servidor chat_servidor.c
 * Execução:   ./chat_servidor
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
#define MAX_CLIENTES  20
#define BUFFER_SIZE   2048
#define NOME_SIZE     32

/* ============================================================
 * Estrutura para armazenar informações de cada cliente
 * ============================================================ */
typedef struct {
    int   fd;                /* file descriptor do socket */
    char  nome[NOME_SIZE];   /* nome do usuário */
    char  ip[INET_ADDRSTRLEN]; /* endereço IP */
    int   porta;             /* porta do cliente */
} Cliente;

/* Lista global de clientes conectados */
static Cliente clientes[MAX_CLIENTES];
static int     num_clientes = 0;

/* ============================================================
 * Funções auxiliares
 * ============================================================ */

/*
 * Envia uma mensagem para TODOS os clientes, EXCETO
 * o que possui o file descriptor 'excluir_fd'.
 * Se excluir_fd == -1, envia para todos sem exceção.
 */
void broadcast(const char *mensagem, int excluir_fd)
{
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].fd != excluir_fd) {
            if (send(clientes[i].fd, mensagem, strlen(mensagem), 0) == -1) {
                perror("Erro ao enviar broadcast");
            }
        }
    }
}

/*
 * Adiciona um novo cliente à lista.
 * Retorna 0 em sucesso, -1 se a lista está cheia.
 */
int adicionar_cliente(int fd, const char *nome, const char *ip, int porta)
{
    if (num_clientes >= MAX_CLIENTES) {
        return -1;
    }
    clientes[num_clientes].fd = fd;
    clientes[num_clientes].porta = porta;
    strncpy(clientes[num_clientes].nome, nome, NOME_SIZE - 1);
    clientes[num_clientes].nome[NOME_SIZE - 1] = '\0';
    strncpy(clientes[num_clientes].ip, ip, INET_ADDRSTRLEN - 1);
    num_clientes++;
    return 0;
}

/*
 * Remove um cliente da lista pelo file descriptor.
 * Preenche 'nome_out' com o nome do cliente removido (para notificação).
 */
void remover_cliente(int fd, char *nome_out, size_t nome_out_size)
{
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].fd == fd) {
            strncpy(nome_out, clientes[i].nome, nome_out_size - 1);
            nome_out[nome_out_size - 1] = '\0';

            /* Move o último cliente para a posição removida (swap) */
            clientes[i] = clientes[num_clientes - 1];
            num_clientes--;
            return;
        }
    }
    /* Se não encontrou (não deveria acontecer) */
    strncpy(nome_out, "Desconhecido", nome_out_size - 1);
}

/*
 * Encontra o nome de um cliente pelo fd.
 */
const char *nome_por_fd(int fd)
{
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].fd == fd) {
            return clientes[i].nome;
        }
    }
    return "Desconhecido";
}

/* ============================================================
 * Função principal
 * ============================================================ */
int main(void)
{
    int server_fd;
    struct sockaddr_in servidor_addr;
    int opt = 1;

    /* -------------------------------------------------------
     * PASSO 1: Criar o socket do servidor
     * ------------------------------------------------------- */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    /* Permite reutilizar a porta imediatamente */
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* -------------------------------------------------------
     * PASSO 2: Bind + Listen
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

    if (listen(server_fd, 10) == -1) {
        perror("Erro no listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("========================================\n");
    printf("   SERVIDOR DE CHAT - Porta %d\n", PORTA);
    printf("   Aguardando conexões...\n");
    printf("   Max clientes: %d\n", MAX_CLIENTES);
    printf("========================================\n\n");

    /* -------------------------------------------------------
     * PASSO 3: Loop principal com select()
     *
     * A cada iteração:
     *   1. Montamos o fd_set com server_fd + todos os clientes
     *   2. Chamamos select() — bloqueia até alguma atividade
     *   3. Verificamos QUEM tem atividade e tratamos
     * ------------------------------------------------------- */
    while (1) {
        fd_set read_fds;  /* conjunto de fds para monitorar */
        int    max_fd;    /* maior fd (necessário para select) */

        /*
         * Reconstruir o fd_set a cada iteração.
         * select() MODIFICA o fd_set — só os fds prontos ficam marcados.
         * Por isso, precisamos montar novamente.
         */
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        max_fd = server_fd;

        /* Adiciona todos os clientes conectados ao conjunto */
        for (int i = 0; i < num_clientes; i++) {
            FD_SET(clientes[i].fd, &read_fds);
            if (clientes[i].fd > max_fd) {
                max_fd = clientes[i].fd;
            }
        }

        /*
         * select() bloqueia aqui até que:
         *   - Um novo cliente tente conectar (server_fd pronto)
         *   - Um cliente conectado envie dados (cliente_fd pronto)
         *   - Um cliente desconecte (cliente_fd pronto com 0 bytes)
         */
        int atividade = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (atividade == -1) {
            /* EINTR acontece se um sinal interromper o select — normalmente
               podemos simplesmente continuar o loop */
            if (errno == EINTR) continue;
            perror("Erro no select");
            break;
        }

        /* -------------------------------------------------------
         * CASO 1: Atividade no server_fd → nova conexão!
         * ------------------------------------------------------- */
        if (FD_ISSET(server_fd, &read_fds)) {
            struct sockaddr_in cliente_addr;
            socklen_t cliente_len = sizeof(cliente_addr);

            int novo_fd = accept(server_fd, (struct sockaddr *)&cliente_addr, &cliente_len);
            if (novo_fd == -1) {
                perror("Erro no accept");
                continue;   /* não encerra o servidor por causa de um erro */
            }

            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cliente_addr.sin_addr, ip_str, sizeof(ip_str));
            int porta_cliente = ntohs(cliente_addr.sin_port);

            printf("[+] Nova conexão: %s:%d (fd=%d)\n", ip_str, porta_cliente, novo_fd);

            /*
             * O primeiro dado que o cliente envia é o nome do usuário.
             * Esperamos recebê-lo logo após a conexão.
             */
            char nome[NOME_SIZE] = {0};
            ssize_t n = recv(novo_fd, nome, NOME_SIZE - 1, 0);
            if (n <= 0) {
                printf("[-] Cliente desconectou antes de informar nome\n");
                close(novo_fd);
                continue;
            }
            nome[n] = '\0';

            /* Remove possível \n do nome */
            char *newline = strchr(nome, '\n');
            if (newline) *newline = '\0';

            /* Tenta adicionar à lista */
            if (adicionar_cliente(novo_fd, nome, ip_str, porta_cliente) == -1) {
                const char *msg_cheio = "Servidor cheio. Tente mais tarde.\n";
                send(novo_fd, msg_cheio, strlen(msg_cheio), 0);
                close(novo_fd);
                printf("[!] Servidor cheio, conexão recusada\n");
                continue;
            }

            printf("[+] \"%s\" entrou no chat (%d clientes online)\n",
                   nome, num_clientes);

            /* Notifica todos os outros */
            char aviso[BUFFER_SIZE];
            snprintf(aviso, sizeof(aviso),
                     ">>> %s entrou no chat (%d online) <<<\n", nome, num_clientes);
            broadcast(aviso, novo_fd);

            /* Mensagem de boas-vindas para o novo cliente */
            char bemvindo[BUFFER_SIZE];
            snprintf(bemvindo, sizeof(bemvindo),
                     ">>> Bem-vindo ao chat, %s! (%d online) <<<\n",
                     nome, num_clientes);
            send(novo_fd, bemvindo, strlen(bemvindo), 0);
        }

        /* -------------------------------------------------------
         * CASO 2: Atividade em algum cliente → mensagem ou desconexão
         *
         * Percorremos a lista de trás para frente (i--) para
         * evitar problemas ao remover clientes durante a iteração.
         * ------------------------------------------------------- */
        for (int i = num_clientes - 1; i >= 0; i--) {
            int cli_fd = clientes[i].fd;

            if (!FD_ISSET(cli_fd, &read_fds)) {
                continue;   /* este cliente não tem atividade */
            }

            char buffer[BUFFER_SIZE] = {0};
            ssize_t bytes = recv(cli_fd, buffer, BUFFER_SIZE - 1, 0);

            if (bytes <= 0) {
                /* ----- Cliente desconectou ----- */
                char nome_saiu[NOME_SIZE];
                remover_cliente(cli_fd, nome_saiu, sizeof(nome_saiu));
                close(cli_fd);

                printf("[-] \"%s\" saiu do chat (%d online)\n",
                       nome_saiu, num_clientes);

                char aviso[BUFFER_SIZE];
                snprintf(aviso, sizeof(aviso),
                         ">>> %s saiu do chat (%d online) <<<\n",
                         nome_saiu, num_clientes);
                broadcast(aviso, -1);  /* -1 = envia para todos */

            } else {
                /* ----- Mensagem recebida ----- */
                buffer[bytes] = '\0';

                /* Remove \n final se existir */
                char *newline = strchr(buffer, '\n');
                if (newline) *newline = '\0';

                /* Verifica se a mensagem não está vazia após trim */
                if (strlen(buffer) == 0) continue;

                const char *nome_remetente = nome_por_fd(cli_fd);
                printf("[%s]: %s\n", nome_remetente, buffer);

                /* Formata "[Nome]: mensagem" e envia para todos os outros */
                char msg_formatada[BUFFER_SIZE];
                snprintf(msg_formatada, sizeof(msg_formatada),
                         "[%s]: %s\n", nome_remetente, buffer);
                broadcast(msg_formatada, cli_fd);
            }
        }
    }

    /* -------------------------------------------------------
     * Limpeza final (só chega aqui se o loop for interrompido)
     * ------------------------------------------------------- */
    for (int i = 0; i < num_clientes; i++) {
        close(clientes[i].fd);
    }
    close(server_fd);
    printf("\n[SERVIDOR] Encerrado.\n");

    return 0;
}
