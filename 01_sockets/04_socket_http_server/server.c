/**
 * =============================================================================
 *  SERVIDOR HTTP SIMPLES EM C USANDO SOCKETS
 * =============================================================================
 *
 *  Este programa implementa um servidor HTTP básico capaz de:
 *    - Aceitar conexões TCP de clientes (navegadores web)
 *    - Interpretar requisições HTTP (método GET)
 *    - Servir arquivos estáticos (HTML, CSS, JS, imagens)
 *    - Responder com códigos HTTP apropriados (200, 404, 405, 500)
 *
 *  Uso: ./server [porta]
 *    - Se nenhuma porta for especificada, usa a porta padrão 8080
 *
 *  Autor: Projeto Acadêmico
 *  Data: 2026
 *
 * =============================================================================
 *  CONCEITOS FUNDAMENTAIS:
 *
 *  1. SOCKET: É um "ponto de comunicação" entre dois processos na rede.
 *     Pense como um telefone: você precisa de um para fazer/receber chamadas.
 *
 *  2. TCP/IP: Protocolo que garante entrega ordenada e confiável dos dados.
 *     O HTTP funciona sobre TCP.
 *
 *  3. HTTP: Protocolo de texto simples baseado em requisição/resposta.
 *     - Cliente envia: "GET /index.html HTTP/1.1\r\n..."
 *     - Servidor responde: "HTTP/1.1 200 OK\r\n..." + conteúdo do arquivo
 *
 *  4. FLUXO DO SERVIDOR:
 *     socket() → bind() → listen() → accept() → read() → write() → close()
 *
 * =============================================================================
 */

/* ========================= INCLUDES ========================= */

#include <stdio.h>      /* printf, fprintf, perror, snprintf, fopen, fread, fclose */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* strlen, strcmp, strstr, memset, strncpy, strncat */
#include <unistd.h>     /* close, read, write */
#include <sys/socket.h> /* socket, bind, listen, accept, setsockopt */
#include <netinet/in.h> /* struct sockaddr_in, htons, INADDR_ANY */
#include <arpa/inet.h>  /* inet_ntoa (para converter IP para string) */
#include <sys/stat.h>   /* stat (para verificar tamanho de arquivos) */
#include <signal.h>     /* signal, SIGINT (para capturar Ctrl+C) */
#include <errno.h>      /* errno (código de erro do sistema) */
#include <time.h>       /* time, ctime (para log com timestamp) */

/* ========================= CONSTANTES ========================= */

/**
 * PORTA_PADRAO: Porta usada quando nenhuma é especificada via argumento.
 * Portas acima de 1024 não exigem permissão de root.
 */
#define PORTA_PADRAO 8080

/**
 * TAM_BUFFER: Tamanho do buffer para leitura das requisições HTTP.
 * 8KB é suficiente para a maioria das requisições GET.
 */
#define TAM_BUFFER 8192

/**
 * TAM_MAX_CAMINHO: Tamanho máximo do caminho de arquivo solicitado.
 */
#define TAM_MAX_CAMINHO 512

/**
 * TAM_MAX_RESPOSTA_HEADER: Tamanho máximo para o cabeçalho da resposta HTTP.
 */
#define TAM_MAX_RESPOSTA_HEADER 1024

/**
 * DIRETORIO_WEB: Pasta onde ficam os arquivos do site (HTML, CSS, JS).
 * O servidor serve arquivos a partir deste diretório.
 */
#define DIRETORIO_WEB "./www"

/**
 * LOG_ATIVO: Define se o servidor imprime logs no terminal.
 * 1 = ativo, 0 = desativado
 */
#define LOG_ATIVO 1

/* ========================= VARIÁVEIS GLOBAIS ========================= */

/**
 * Variável global para o socket do servidor.
 * É global para que o handler de SIGINT possa fechá-lo corretamente.
 */
int socket_servidor = -1;

/* ========================= PROTÓTIPOS DE FUNÇÕES ========================= */

void        tratar_sinal(int sinal);
int         criar_socket_servidor(int porta);
void        tratar_requisicao(int socket_cliente, struct sockaddr_in endereco_cliente);
void        enviar_resposta(int socket_cliente, int codigo_status, 
                           const char *tipo_conteudo, const char *corpo, long tamanho_corpo);
void        enviar_arquivo(int socket_cliente, const char *caminho_arquivo);
const char *obter_tipo_mime(const char *caminho_arquivo);
void        enviar_erro_404(int socket_cliente);
void        enviar_erro_405(int socket_cliente);
void        enviar_erro_500(int socket_cliente);
void        log_requisicao(const char *ip_cliente, const char *metodo, 
                           const char *caminho, int codigo_status);

/* ========================= FUNÇÃO PRINCIPAL ========================= */

/**
 * main() - Ponto de entrada do programa
 *
 * Parâmetros:
 *   argc - Número de argumentos da linha de comando
 *   argv - Vetor de strings com os argumentos
 *          argv[0] = nome do programa
 *          argv[1] = porta (opcional)
 *
 * Fluxo:
 *   1. Lê a porta dos argumentos (ou usa a padrão)
 *   2. Configura o handler de sinais (para Ctrl+C)
 *   3. Cria e configura o socket do servidor
 *   4. Entra no loop principal aceitando conexões
 *   5. Para cada conexão, trata a requisição e fecha o socket do cliente
 */
int main(int argc, char *argv[])
{
    int                porta;
    int                socket_cliente;
    struct sockaddr_in endereco_cliente;
    socklen_t          tamanho_endereco;

    /* ---- Passo 1: Definir a porta ---- */
    if (argc >= 2) {
        porta = atoi(argv[1]);  /* Converte o argumento de string para inteiro */
        if (porta <= 0 || porta > 65535) {
            fprintf(stderr, "Erro: Porta inválida '%s'. Use um valor entre 1 e 65535.\n", argv[1]);
            return EXIT_FAILURE;
        }
    } else {
        porta = PORTA_PADRAO;
    }

    /* ---- Passo 2: Configurar handler de sinal (Ctrl+C) ---- */
    /*
     * Quando o usuário pressiona Ctrl+C, o sistema operacional envia
     * o sinal SIGINT para o processo. Configuramos um handler para
     * fechar o socket do servidor antes de encerrar, liberando a porta.
     */
    signal(SIGINT, tratar_sinal);

    /* ---- Passo 3: Criar o socket do servidor ---- */
    socket_servidor = criar_socket_servidor(porta);
    if (socket_servidor < 0) {
        return EXIT_FAILURE;
    }

    printf("==============================================\n");
    printf("  Servidor HTTP iniciado!\n");
    printf("  Porta: %d\n", porta);
    printf("  Acesse: http://localhost:%d\n", porta);
    printf("  Diretório web: %s\n", DIRETORIO_WEB);
    printf("  Pressione Ctrl+C para encerrar\n");
    printf("==============================================\n\n");

    /* ---- Passo 4: Loop principal - aceitar conexões ---- */
    /*
     * O servidor fica em um loop infinito esperando novas conexões.
     * Para cada conexão:
     *   1. accept() bloqueia até um cliente conectar
     *   2. Trata a requisição do cliente
     *   3. Fecha o socket do cliente
     *   4. Volta a esperar novas conexões
     *
     * NOTA: Este é um servidor sequencial (single-threaded).
     * Ele trata um cliente por vez. Para alta performance,
     * seria necessário usar threads, fork() ou I/O assíncrono.
     */
    while (1) {
        tamanho_endereco = sizeof(endereco_cliente);

        /*
         * accept() - Aceita uma conexão pendente
         *
         * Parâmetros:
         *   - socket_servidor: o socket que está "ouvindo"
         *   - endereco_cliente: será preenchido com o IP/porta do cliente
         *   - tamanho_endereco: tamanho da struct de endereço
         *
         * Retorna:
         *   - Um NOVO socket para comunicação com este cliente específico
         *   - O socket_servidor continua disponível para novas conexões
         */
        socket_cliente = accept(socket_servidor, 
                               (struct sockaddr *)&endereco_cliente, 
                               &tamanho_endereco);

        if (socket_cliente < 0) {
            /* Se foi interrompido por sinal (Ctrl+C), sai do loop */
            if (errno == EINTR) break;
            perror("Erro no accept()");
            continue;  /* Tenta aceitar a próxima conexão */
        }

        /* ---- Passo 5: Tratar a requisição ---- */
        tratar_requisicao(socket_cliente, endereco_cliente);

        /* ---- Passo 6: Fechar o socket do cliente ---- */
        /*
         * Após enviar a resposta, fechamos o socket do cliente.
         * Cada conexão HTTP/1.0 é independente (stateless).
         */
        close(socket_cliente);
    }

    /* Limpeza final (caso saia do loop normalmente) */
    if (socket_servidor >= 0) {
        close(socket_servidor);
    }

    printf("\nServidor encerrado.\n");
    return EXIT_SUCCESS;
}

/* ========================= FUNÇÕES AUXILIARES ========================= */

/**
 * tratar_sinal() - Handler para sinais do sistema (ex: Ctrl+C)
 *
 * Quando o usuário pressiona Ctrl+C, esta função é chamada automaticamente.
 * Ela fecha o socket do servidor (liberando a porta) e encerra o programa.
 *
 * @param sinal  O número do sinal recebido (ex: SIGINT = 2)
 */
void tratar_sinal(int sinal)
{
    (void)sinal;  /* Evita warning de parâmetro não usado */

    printf("\n\nRecebido sinal de interrupção. Encerrando servidor...\n");

    if (socket_servidor >= 0) {
        close(socket_servidor);
        socket_servidor = -1;
    }

    exit(EXIT_SUCCESS);
}

/**
 * criar_socket_servidor() - Cria, configura e coloca o socket em modo escuta
 *
 * Esta função encapsula os 3 passos para criar um socket servidor:
 *   1. socket()  - Cria o socket
 *   2. bind()    - Associa o socket a um endereço IP e porta
 *   3. listen()  - Coloca o socket em modo de escuta
 *
 * @param porta  A porta TCP em que o servidor vai escutar
 * @return       O file descriptor do socket criado, ou -1 em caso de erro
 *
 * ANALOGIA:
 *   Imagine abrir uma loja:
 *   - socket()  = comprar um telefone
 *   - bind()    = associar um número de telefone a ele
 *   - listen()  = ligar o telefone e esperar chamadas
 */
int criar_socket_servidor(int porta)
{
    int                socket_fd;
    int                opcao = 1;
    struct sockaddr_in endereco_servidor;

    /* ---- Passo 1: Criar o socket ---- */
    /*
     * socket(domínio, tipo, protocolo)
     *
     * - AF_INET:      Família de endereços IPv4 (Internet)
     * - SOCK_STREAM:  Socket de fluxo (TCP - conexão confiável e ordenada)
     * - 0:            Protocolo padrão para o tipo (TCP para SOCK_STREAM)
     *
     * Retorna um "file descriptor" (número inteiro) que identifica o socket.
     * Em Unix, "tudo é arquivo" - sockets também são tratados como arquivos.
     */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Erro ao criar socket");
        return -1;
    }

    /* ---- Configuração extra: SO_REUSEADDR ---- */
    /*
     * setsockopt() com SO_REUSEADDR permite reutilizar a porta imediatamente
     * após o servidor ser encerrado. Sem isso, o sistema operacional pode
     * manter a porta "ocupada" por até 2 minutos (estado TIME_WAIT).
     *
     * Isso é especialmente útil durante desenvolvimento, quando iniciamos
     * e paramos o servidor frequentemente.
     */
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(opcao)) < 0) {
        perror("Erro no setsockopt(SO_REUSEADDR)");
        close(socket_fd);
        return -1;
    }

    /* ---- Passo 2: Associar o socket a um endereço (bind) ---- */
    /*
     * Preenchemos a struct sockaddr_in com:
     * - sin_family:      AF_INET (IPv4)
     * - sin_addr.s_addr: INADDR_ANY (aceita conexões em qualquer interface de rede)
     * - sin_port:        A porta desejada (convertida para "network byte order" com htons)
     *
     * htons() = Host TO Network Short
     * A ordem dos bytes pode ser diferente entre sua máquina e a rede.
     * htons() garante que a porta seja interpretada corretamente.
     */
    memset(&endereco_servidor, 0, sizeof(endereco_servidor));
    endereco_servidor.sin_family      = AF_INET;
    endereco_servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    endereco_servidor.sin_port        = htons(porta);

    /*
     * bind() associa o socket ao endereço especificado.
     * É como dizer: "Este socket atende na porta X".
     */
    if (bind(socket_fd, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor)) < 0) {
        perror("Erro no bind()");
        fprintf(stderr, "Dica: A porta %d pode estar em uso. Tente outra porta.\n", porta);
        close(socket_fd);
        return -1;
    }

    /* ---- Passo 3: Colocar o socket em modo escuta ---- */
    /*
     * listen(socket, backlog)
     *
     * - socket:  O socket que vai escutar
     * - backlog: Tamanho máximo da fila de conexões pendentes (10 é um bom valor)
     *
     * Após listen(), o socket está pronto para aceitar conexões com accept().
     */
    if (listen(socket_fd, 10) < 0) {
        perror("Erro no listen()");
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

/**
 * tratar_requisicao() - Lê e processa uma requisição HTTP de um cliente
 *
 * Uma requisição HTTP GET tem o seguinte formato:
 *
 *   GET /caminho/arquivo.html HTTP/1.1\r\n
 *   Host: localhost:8080\r\n
 *   User-Agent: Mozilla/5.0 ...\r\n
 *   \r\n
 *
 * A primeira linha (Request Line) contém:
 *   - Método: GET, POST, PUT, DELETE, etc.
 *   - Caminho: O recurso solicitado (ex: /index.html)
 *   - Versão: HTTP/1.0 ou HTTP/1.1
 *
 * @param socket_cliente     Socket conectado ao cliente
 * @param endereco_cliente   Endereço IP e porta do cliente
 */
void tratar_requisicao(int socket_cliente, struct sockaddr_in endereco_cliente)
{
    char buffer[TAM_BUFFER];
    char metodo[16];
    char caminho[TAM_MAX_CAMINHO];
    char caminho_arquivo[TAM_MAX_CAMINHO];
    int  bytes_lidos;

    /* ---- Ler a requisição do cliente ---- */
    /*
     * read() lê dados do socket para o buffer.
     * O cliente enviou uma requisição HTTP em texto puro.
     * Lemos até TAM_BUFFER - 1 bytes para deixar espaço para o '\0'.
     */
    memset(buffer, 0, TAM_BUFFER);
    bytes_lidos = read(socket_cliente, buffer, TAM_BUFFER - 1);

    if (bytes_lidos <= 0) {
        /* Conexão fechada pelo cliente ou erro de leitura */
        return;
    }

    /* Garante que o buffer termine com '\0' (string C válida) */
    buffer[bytes_lidos] = '\0';

    /* ---- Extrair o método e o caminho da requisição ---- */
    /*
     * sscanf() faz parsing da primeira linha da requisição.
     * Exemplo: "GET /index.html HTTP/1.1"
     *   metodo  → "GET"
     *   caminho → "/index.html"
     *
     * %15s lê no máximo 15 caracteres para metodo (buffer de 16)
     * %511s lê no máximo 511 caracteres para caminho (buffer de 512)
     */
    if (sscanf(buffer, "%15s %511s", metodo, caminho) != 2) {
        /* Requisição malformada - não conseguimos extrair método e caminho */
        enviar_erro_500(socket_cliente);
        return;
    }

    /* ---- Verificar se o método é GET ---- */
    /*
     * Nosso servidor simples suporta apenas o método GET.
     * GET é usado para solicitar recursos (páginas, imagens, etc.)
     *
     * Outros métodos comuns:
     *   POST   - Enviar dados ao servidor (formulários)
     *   PUT    - Atualizar um recurso existente
     *   DELETE - Remover um recurso
     */
    if (strcmp(metodo, "GET") != 0) {
        log_requisicao(inet_ntoa(endereco_cliente.sin_addr), metodo, caminho, 405);
        enviar_erro_405(socket_cliente);
        return;
    }

    /* ---- Mapear o caminho da URL para um arquivo no disco ---- */
    /*
     * Se o cliente pedir "/", servimos "/index.html" (página padrão).
     * Caso contrário, servimos o arquivo correspondente ao caminho.
     *
     * Exemplo:
     *   URL "/"           → arquivo "./www/index.html"
     *   URL "/style.css"  → arquivo "./www/style.css"
     *   URL "/js/app.js"  → arquivo "./www/js/app.js"
     *
     * SEGURANÇA: Em um servidor real, devemos verificar se o caminho
     * não contém ".." para evitar "directory traversal attacks".
     */
    if (strcmp(caminho, "/") == 0) {
        snprintf(caminho_arquivo, TAM_MAX_CAMINHO, "%s/index.html", DIRETORIO_WEB);
    } else {
        /* Verificação básica de segurança contra directory traversal */
        if (strstr(caminho, "..") != NULL) {
            log_requisicao(inet_ntoa(endereco_cliente.sin_addr), metodo, caminho, 403);
            enviar_erro_404(socket_cliente);
            return;
        }
        snprintf(caminho_arquivo, TAM_MAX_CAMINHO, "%s%s", DIRETORIO_WEB, caminho);
    }

    /* ---- Enviar o arquivo solicitado ---- */
    enviar_arquivo(socket_cliente, caminho_arquivo);

    /* Log da requisição */
    log_requisicao(inet_ntoa(endereco_cliente.sin_addr), metodo, caminho, 200);
}

/**
 * enviar_resposta() - Monta e envia uma resposta HTTP completa
 *
 * Uma resposta HTTP tem o seguinte formato:
 *
 *   HTTP/1.1 200 OK\r\n                    ← Linha de status
 *   Content-Type: text/html\r\n             ← Cabeçalhos
 *   Content-Length: 1234\r\n                 ← (um por linha)
 *   Connection: close\r\n                   ← 
 *   \r\n                                    ← Linha vazia (fim dos cabeçalhos)
 *   <html>...</html>                        ← Corpo (conteúdo)
 *
 * @param socket_cliente   Socket conectado ao cliente
 * @param codigo_status    Código HTTP (200, 404, 405, 500)
 * @param tipo_conteudo    Tipo MIME do conteúdo (ex: "text/html")
 * @param corpo            Ponteiro para os dados do corpo da resposta
 * @param tamanho_corpo    Tamanho do corpo em bytes
 */
void enviar_resposta(int socket_cliente, int codigo_status,
                     const char *tipo_conteudo, const char *corpo, long tamanho_corpo)
{
    char header[TAM_MAX_RESPOSTA_HEADER];
    const char *texto_status;

    /* Determinar o texto do código de status */
    switch (codigo_status) {
        case 200: texto_status = "OK";                    break;
        case 404: texto_status = "Not Found";             break;
        case 405: texto_status = "Method Not Allowed";    break;
        case 500: texto_status = "Internal Server Error"; break;
        default:  texto_status = "Unknown";               break;
    }

    /* ---- Montar o cabeçalho HTTP ---- */
    /*
     * snprintf() formata o cabeçalho HTTP em uma string.
     *
     * Cabeçalhos importantes:
     * - Content-Type:   Informa ao navegador o tipo do conteúdo
     * - Content-Length:  Tamanho do corpo em bytes
     * - Connection:     "close" indica que a conexão será fechada após a resposta
     * - Server:         Nome do servidor (opcional, informativo)
     *
     * \r\n (CRLF) é o separador de linhas no protocolo HTTP (RFC 2616).
     * A linha vazia \r\n\r\n marca o fim dos cabeçalhos.
     */
    snprintf(header, TAM_MAX_RESPOSTA_HEADER,
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n"
             "Server: MeuServidorHTTP/1.0\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "\r\n",
             codigo_status, texto_status,
             tipo_conteudo,
             tamanho_corpo);

    /* ---- Enviar cabeçalho e corpo ---- */
    /*
     * write() envia dados pelo socket para o cliente.
     * Primeiro enviamos o cabeçalho, depois o corpo.
     */
    write(socket_cliente, header, strlen(header));

    if (corpo != NULL && tamanho_corpo > 0) {
        write(socket_cliente, corpo, tamanho_corpo);
    }
}

/**
 * enviar_arquivo() - Lê um arquivo do disco e o envia como resposta HTTP
 *
 * @param socket_cliente   Socket conectado ao cliente
 * @param caminho_arquivo  Caminho completo do arquivo no disco
 *
 * Fluxo:
 *   1. Verifica se o arquivo existe (stat)
 *   2. Abre o arquivo para leitura binária
 *   3. Lê todo o conteúdo para a memória
 *   4. Determina o tipo MIME baseado na extensão
 *   5. Envia a resposta HTTP com o conteúdo
 *   6. Libera a memória alocada
 */
void enviar_arquivo(int socket_cliente, const char *caminho_arquivo)
{
    struct stat info_arquivo;
    FILE       *arquivo;
    char       *conteudo;
    long        tamanho;
    const char *tipo_mime;

    /* ---- Verificar se o arquivo existe ---- */
    /*
     * stat() obtém informações sobre o arquivo (tamanho, permissões, etc.)
     * Se retornar -1, o arquivo não existe ou não pode ser acessado.
     */
    if (stat(caminho_arquivo, &info_arquivo) < 0) {
        enviar_erro_404(socket_cliente);
        return;
    }

    /* Verificar se é um arquivo regular (não é diretório) */
    if (!S_ISREG(info_arquivo.st_mode)) {
        enviar_erro_404(socket_cliente);
        return;
    }

    tamanho = info_arquivo.st_size;

    /* ---- Abrir o arquivo ---- */
    /*
     * "rb" = read binary (leitura em modo binário)
     * Modo binário é importante para arquivos como imagens,
     * que não são texto puro.
     */
    arquivo = fopen(caminho_arquivo, "rb");
    if (arquivo == NULL) {
        enviar_erro_500(socket_cliente);
        return;
    }

    /* ---- Alocar memória e ler o conteúdo ---- */
    /*
     * malloc() aloca 'tamanho' bytes na memória heap.
     * Lemos todo o arquivo de uma vez para simplificar.
     *
     * NOTA: Para arquivos muito grandes, seria melhor ler e enviar
     * em blocos (chunks) para não consumir muita memória.
     */
    conteudo = (char *)malloc(tamanho);
    if (conteudo == NULL) {
        fclose(arquivo);
        enviar_erro_500(socket_cliente);
        return;
    }

    /* fread() lê 'tamanho' bytes do arquivo para o buffer 'conteudo' */
    fread(conteudo, 1, tamanho, arquivo);
    fclose(arquivo);

    /* ---- Determinar o tipo MIME e enviar ---- */
    tipo_mime = obter_tipo_mime(caminho_arquivo);
    enviar_resposta(socket_cliente, 200, tipo_mime, conteudo, tamanho);

    /* Liberar a memória alocada */
    free(conteudo);
}

/**
 * obter_tipo_mime() - Determina o tipo MIME baseado na extensão do arquivo
 *
 * O tipo MIME (Multipurpose Internet Mail Extensions) informa ao navegador
 * como interpretar o conteúdo recebido.
 *
 * Exemplos:
 *   .html → text/html         (o navegador renderiza como página web)
 *   .css  → text/css          (o navegador aplica como folha de estilo)
 *   .js   → application/javascript  (o navegador executa como script)
 *   .png  → image/png         (o navegador exibe como imagem)
 *
 * @param caminho_arquivo  Nome do arquivo (usado para extrair a extensão)
 * @return                 String com o tipo MIME correspondente
 */
const char *obter_tipo_mime(const char *caminho_arquivo)
{
    /* strrchr() encontra a ÚLTIMA ocorrência de '.' no nome do arquivo */
    const char *extensao = strrchr(caminho_arquivo, '.');

    if (extensao == NULL) {
        return "application/octet-stream";  /* Tipo genérico (binário) */
    }

    /* Comparar a extensão com tipos conhecidos */
    if (strcmp(extensao, ".html") == 0 || strcmp(extensao, ".htm") == 0)
        return "text/html; charset=UTF-8";
    if (strcmp(extensao, ".css") == 0)
        return "text/css; charset=UTF-8";
    if (strcmp(extensao, ".js") == 0)
        return "application/javascript; charset=UTF-8";
    if (strcmp(extensao, ".json") == 0)
        return "application/json; charset=UTF-8";
    if (strcmp(extensao, ".png") == 0)
        return "image/png";
    if (strcmp(extensao, ".jpg") == 0 || strcmp(extensao, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(extensao, ".gif") == 0)
        return "image/gif";
    if (strcmp(extensao, ".svg") == 0)
        return "image/svg+xml";
    if (strcmp(extensao, ".ico") == 0)
        return "image/x-icon";
    if (strcmp(extensao, ".txt") == 0)
        return "text/plain; charset=UTF-8";

    /* Tipo padrão para extensões desconhecidas */
    return "application/octet-stream";
}

/**
 * enviar_erro_404() - Envia uma página de erro "404 Not Found"
 *
 * O código 404 indica que o recurso solicitado não foi encontrado.
 * É o erro mais comum na web (aquela famosa "Página não encontrada").
 *
 * @param socket_cliente  Socket conectado ao cliente
 */
void enviar_erro_404(int socket_cliente)
{
    const char *corpo =
        "<!DOCTYPE html>"
        "<html lang=\"pt-BR\"><head><meta charset=\"UTF-8\">"
        "<title>404 - Não Encontrado</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; text-align: center; padding: 50px;"
        "       background: #1a1a2e; color: #eee; }"
        "h1 { font-size: 72px; margin: 0; color: #e94560; }"
        "p { font-size: 20px; color: #aaa; }"
        "</style></head><body>"
        "<h1>404</h1>"
        "<p>Arquivo não encontrado no servidor.</p>"
        "<p><a href=\"/\" style=\"color:#0f3460;\">Voltar ao início</a></p>"
        "</body></html>";

    enviar_resposta(socket_cliente, 404, "text/html; charset=UTF-8", corpo, strlen(corpo));
}

/**
 * enviar_erro_405() - Envia uma página de erro "405 Method Not Allowed"
 *
 * O código 405 indica que o método HTTP usado não é suportado.
 * Nosso servidor suporta apenas GET.
 *
 * @param socket_cliente  Socket conectado ao cliente
 */
void enviar_erro_405(int socket_cliente)
{
    const char *corpo =
        "<!DOCTYPE html>"
        "<html lang=\"pt-BR\"><head><meta charset=\"UTF-8\">"
        "<title>405 - Método Não Permitido</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; text-align: center; padding: 50px;"
        "       background: #1a1a2e; color: #eee; }"
        "h1 { font-size: 72px; margin: 0; color: #e94560; }"
        "p { font-size: 20px; color: #aaa; }"
        "</style></head><body>"
        "<h1>405</h1>"
        "<p>Método HTTP não permitido. Este servidor suporta apenas GET.</p>"
        "</body></html>";

    enviar_resposta(socket_cliente, 405, "text/html; charset=UTF-8", corpo, strlen(corpo));
}

/**
 * enviar_erro_500() - Envia uma página de erro "500 Internal Server Error"
 *
 * O código 500 indica um erro interno do servidor.
 *
 * @param socket_cliente  Socket conectado ao cliente
 */
void enviar_erro_500(int socket_cliente)
{
    const char *corpo =
        "<!DOCTYPE html>"
        "<html lang=\"pt-BR\"><head><meta charset=\"UTF-8\">"
        "<title>500 - Erro Interno</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; text-align: center; padding: 50px;"
        "       background: #1a1a2e; color: #eee; }"
        "h1 { font-size: 72px; margin: 0; color: #e94560; }"
        "p { font-size: 20px; color: #aaa; }"
        "</style></head><body>"
        "<h1>500</h1>"
        "<p>Erro interno do servidor.</p>"
        "</body></html>";

    enviar_resposta(socket_cliente, 500, "text/html; charset=UTF-8", corpo, strlen(corpo));
}

/**
 * log_requisicao() - Imprime informações sobre a requisição no terminal
 *
 * Formato do log:
 *   [2026-02-20 14:30:15] 192.168.1.1 GET /index.html → 200
 *
 * @param ip_cliente     Endereço IP do cliente
 * @param metodo         Método HTTP (GET, POST, etc.)
 * @param caminho        Caminho solicitado (/index.html, etc.)
 * @param codigo_status  Código de status HTTP da resposta
 */
void log_requisicao(const char *ip_cliente, const char *metodo,
                    const char *caminho, int codigo_status)
{
    time_t     agora;
    struct tm *info_tempo;
    char       timestamp[64];

    if (!LOG_ATIVO) return;

    /* Obter data/hora atual formatada */
    time(&agora);
    info_tempo = localtime(&agora);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", info_tempo);

    /* Imprimir log colorido (códigos ANSI para terminal) */
    if (codigo_status == 200) {
        /* Verde para sucesso */
        printf("[%s] %s \033[32m%s %s → %d\033[0m\n",
               timestamp, ip_cliente, metodo, caminho, codigo_status);
    } else {
        /* Vermelho para erros */
        printf("[%s] %s \033[31m%s %s → %d\033[0m\n",
               timestamp, ip_cliente, metodo, caminho, codigo_status);
    }
}
