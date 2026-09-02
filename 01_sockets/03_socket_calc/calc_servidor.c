/*
 * calc_servidor.c — Servidor de Calculadora via Socket
 *
 * O servidor:
 *   1. Aceita múltiplos clientes (um por vez, em loop)
 *   2. Recebe operações no formato:  operando1 operador operando2
 *      Exemplo: "12.5 + 7.3"
 *   3. Calcula o resultado no servidor
 *   4. Envia o resultado de volta ao cliente
 *   5. O cliente pode enviar várias operações até digitar "sair"
 *
 * Protocolo definido:
 *   Cliente → Servidor:  "operando1 operador operando2\n"
 *   Servidor → Cliente:  "Resultado: valor\n"  ou  "ERRO: descrição\n"
 *
 * Compilação: gcc -Wall -Wextra -o calc_servidor calc_servidor.c -lm
 * Execução:   ./calc_servidor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA       9090
#define BUFFER_SIZE 256

/* ============================================================
 * Função que processa a operação recebida do cliente.
 *
 * Recebe a string com a expressão (ex: "10 + 3"),
 * faz o parse, calcula e escreve o resultado em 'resposta'.
 *
 * Retorna 0 se a operação é válida, -1 se houve erro de parse.
 * ============================================================ */
int calcular(const char *expressao, char *resposta, size_t resp_size)
{
    double a, b, resultado;
    char operador;

    /*
     * sscanf tenta extrair: double, char, double
     * O formato " %c" (com espaço antes) ignora espaços em branco
     * antes do operador.
     */
    int parsed = sscanf(expressao, "%lf %c %lf", &a, &operador, &b);
    if (parsed != 3) {
        snprintf(resposta, resp_size,
                 "ERRO: formato inválido. Use: <num> <op> <num>  (ex: 10 + 3)\n");
        return -1;
    }

    switch (operador) {
        case '+':
            resultado = a + b;
            break;
        case '-':
            resultado = a - b;
            break;
        case '*':
        case 'x':
        case 'X':
            resultado = a * b;
            break;
        case '/':
            if (b == 0.0) {
                snprintf(resposta, resp_size, "ERRO: divisão por zero!\n");
                return -1;
            }
            resultado = a / b;
            break;
        case '^':
            resultado = pow(a, b);
            break;
        case '%':
            if (b == 0.0) {
                snprintf(resposta, resp_size, "ERRO: módulo por zero!\n");
                return -1;
            }
            resultado = fmod(a, b);
            break;
        default:
            snprintf(resposta, resp_size,
                     "ERRO: operador '%c' não suportado. Use: + - * / ^ %%\n",
                     operador);
            return -1;
    }

    /*
     * Formata o resultado. Se é inteiro (ex: 10.0), mostra sem casas decimais.
     * Senão, mostra com até 6 casas decimais.
     */
    if (resultado == (long long)resultado && fabs(resultado) < 1e15) {
        snprintf(resposta, resp_size, "= %.0f\n", resultado);
    } else {
        snprintf(resposta, resp_size, "= %.6g\n", resultado);
    }

    return 0;
}

/* ============================================================
 * Função que atende um cliente conectado.
 * Recebe operações em loop até o cliente desconectar ou enviar "sair".
 * ============================================================ */
void atender_cliente(int cliente_fd, const char *ip, int porta)
{
    char buffer[BUFFER_SIZE];
    char resposta[BUFFER_SIZE];

    /* Envia banner ao cliente */
    const char *banner =
        "========================================\n"
        "  Calculadora Remota - Conectado!\n"
        "  Formato: <num> <op> <num>\n"
        "  Operadores: + - * / ^ %\n"
        "  Digite 'sair' para desconectar.\n"
        "========================================\n";
    send(cliente_fd, banner, strlen(banner), 0);

    while (1) {
        /* Envia prompt */
        const char *prompt = "calc> ";
        send(cliente_fd, prompt, strlen(prompt), 0);

        /* Recebe expressão do cliente */
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = recv(cliente_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes <= 0) {
            printf("[%s:%d] Cliente desconectou.\n", ip, porta);
            break;
        }

        buffer[bytes] = '\0';

        /* Remove \n */
        char *nl = strchr(buffer, '\n');
        if (nl) *nl = '\0';

        /* Ignora mensagens vazias */
        if (strlen(buffer) == 0) continue;

        /* Comando de saída */
        if (strcmp(buffer, "sair") == 0 || strcmp(buffer, "exit") == 0 ||
            strcmp(buffer, "quit") == 0) {
            const char *tchau = "Até mais!\n";
            send(cliente_fd, tchau, strlen(tchau), 0);
            printf("[%s:%d] Cliente solicitou desconexão.\n", ip, porta);
            break;
        }

        /* Processa a operação */
        printf("[%s:%d] Operação: %s", ip, porta, buffer);

        memset(resposta, 0, sizeof(resposta));
        calcular(buffer, resposta, sizeof(resposta));

        printf(" → %s", resposta);

        /* Envia resultado ao cliente */
        send(cliente_fd, resposta, strlen(resposta), 0);
    }
}

/* ============================================================
 * Função principal
 * ============================================================ */
int main(void)
{
    int server_fd;
    struct sockaddr_in servidor_addr;
    int opt = 1;

    /* Criar socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* Bind */
    memset(&servidor_addr, 0, sizeof(servidor_addr));
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = INADDR_ANY;
    servidor_addr.sin_port = htons(PORTA);

    if (bind(server_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Erro no bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /* Listen */
    if (listen(server_fd, 5) == -1) {
        perror("Erro no listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("========================================\n");
    printf("  CALCULADORA - Servidor (porta %d)\n", PORTA);
    printf("  Aguardando conexões...\n");
    printf("========================================\n\n");

    /*
     * Loop principal: aceita clientes um por vez.
     * Quando um cliente desconecta, volta a aceitar o próximo.
     *
     * Para atender múltiplos ao mesmo tempo, seria necessário
     * usar select(), threads ou fork() — que são temas de aulas futuras.
     */
    while (1) {
        struct sockaddr_in cliente_addr;
        socklen_t cliente_len = sizeof(cliente_addr);

        int cliente_fd = accept(server_fd, (struct sockaddr *)&cliente_addr, &cliente_len);
        if (cliente_fd == -1) {
            perror("Erro no accept");
            continue;
        }

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cliente_addr.sin_addr, ip_str, sizeof(ip_str));
        int porta_cliente = ntohs(cliente_addr.sin_port);

        printf("[+] Cliente conectado: %s:%d\n", ip_str, porta_cliente);

        /* Atende o cliente (bloqueia até ele desconectar) */
        atender_cliente(cliente_fd, ip_str, porta_cliente);

        close(cliente_fd);
        printf("[-] Conexão encerrada. Aguardando próximo cliente...\n\n");
    }

    close(server_fd);
    return 0;
}
