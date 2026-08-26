# Parte 1 — Introdução a Sockets em C

## O que são Sockets?

Um **socket** é um ponto de comunicação entre dois processos, que podem estar na mesma máquina ou em máquinas diferentes conectadas por uma rede. Pense nele como uma **tomada de comunicação**: assim como você conecta um cabo numa tomada para transmitir energia, você conecta um socket para transmitir dados.

Na prática, sockets são a base de praticamente toda comunicação em rede que usamos no dia a dia — navegadores web, jogos online, aplicativos de chat, streaming de vídeo, tudo isso utiliza sockets por baixo dos panos.

### Analogia simples

Imagine uma ligação telefônica:

1. Você precisa de um **telefone** (criar o socket)
2. Precisa de um **número** para ligar (endereço IP + porta)
3. Alguém do outro lado precisa **atender** (aceitar a conexão)
4. Vocês **conversam** (enviam e recebem dados)
5. No final, **desligam** (fecham o socket)

Essa é, essencialmente, a sequência de operações que faremos em C.

---

## Tipos de Socket

Existem dois tipos principais que você vai usar no dia a dia:

### 1. `SOCK_STREAM` (TCP)

- **Orientado à conexão** — precisa estabelecer uma conexão antes de trocar dados
- **Confiável** — garante que os dados chegam na ordem correta e sem perda
- **Analogia**: uma ligação telefônica (conexão contínua e ordenada)
- **Uso típico**: HTTP, SSH, FTP, bancos de dados

### 2. `SOCK_DGRAM` (UDP)

- **Sem conexão** — envia dados diretamente, sem "handshake"
- **Não confiável** — pacotes podem chegar fora de ordem ou se perder
- **Mais rápido** — menos overhead por não ter controle de conexão
- **Analogia**: enviar cartas pelo correio (cada uma é independente)
- **Uso típico**: DNS, jogos online, streaming de vídeo, VoIP

### Comparativo rápido

| Característica      | TCP (`SOCK_STREAM`)     | UDP (`SOCK_DGRAM`)       |
|---------------------|-------------------------|--------------------------|
| Conexão             | Sim (connect/accept)    | Não                      |
| Ordem garantida     | Sim                     | Não                      |
| Entrega garantida   | Sim                     | Não                      |
| Velocidade          | Mais lento              | Mais rápido              |
| Overhead            | Maior                   | Menor                    |

---

## Arquitetura Cliente-Servidor

A comunicação via sockets geralmente segue o modelo **cliente-servidor**:

- **Servidor**: fica "escutando" em uma porta, esperando conexões
- **Cliente**: inicia a conexão com o servidor

```
┌──────────┐                    ┌──────────┐
│  CLIENTE │ ───── rede ──────▶ │ SERVIDOR │
│          │ ◀──── rede ─────── │          │
└──────────┘                    └──────────┘
```

---

## Fluxo de um Servidor TCP

```
socket()    →  cria o socket
    │
bind()      →  associa o socket a um endereço (IP + porta)
    │
listen()    →  coloca o socket em modo de escuta
    │
accept()    →  aceita uma conexão de um cliente (bloqueia até chegar uma)
    │
recv/send() →  recebe e envia dados
    │
close()     →  encerra o socket
```

## Fluxo de um Cliente TCP

```
socket()    →  cria o socket
    │
connect()   →  conecta ao servidor (IP + porta)
    │
send/recv() →  envia e recebe dados
    │
close()     →  encerra o socket
```

---

## As Funções Essenciais

Todas as funções abaixo estão nos headers:

```c
#include <sys/socket.h>   // socket, bind, listen, accept, connect, send, recv
#include <netinet/in.h>   // struct sockaddr_in, INADDR_ANY
#include <arpa/inet.h>    // inet_pton, inet_ntoa
#include <unistd.h>       // close
```

---

### `socket()` — Criar o socket

```c
int socket(int domain, int type, int protocol);
```

| Parâmetro  | O que é                              | Valor típico     |
|------------|--------------------------------------|------------------|
| `domain`   | Família de endereços                 | `AF_INET` (IPv4) |
| `type`     | Tipo do socket                       | `SOCK_STREAM` (TCP) ou `SOCK_DGRAM` (UDP) |
| `protocol` | Protocolo específico (0 = automático)| `0`              |

**Retorno**: um **file descriptor** (inteiro positivo) em caso de sucesso, ou `-1` em caso de erro.

```c
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
if (server_fd == -1) {
    perror("Erro ao criar socket");
    exit(EXIT_FAILURE);
}
```

> **Curiosidade**: em sistemas Unix, "tudo é um arquivo". Um socket é tratado como um file descriptor, então você pode usar funções como `read()` e `write()` nele também.

---

### `bind()` — Associar a um endereço

```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

O `bind()` associa o socket a um endereço IP e uma porta. É como dizer: "este socket escuta **neste** endereço".

Antes de chamar `bind()`, você precisa preencher uma estrutura `sockaddr_in`:

```c
struct sockaddr_in endereco;
endereco.sin_family = AF_INET;           // IPv4
endereco.sin_addr.s_addr = INADDR_ANY;   // aceita conexões de qualquer IP
endereco.sin_port = htons(8080);          // porta 8080 (convertida para network byte order)

bind(server_fd, (struct sockaddr *)&endereco, sizeof(endereco));
```

> **O que é `htons()`?** Significa *Host TO Network Short*. Converte a porta do formato da sua máquina (little-endian, geralmente) para o formato da rede (big-endian). **Sempre use `htons()` para portas.**

---

### `listen()` — Colocar em modo de escuta

```c
int listen(int sockfd, int backlog);
```

| Parâmetro | O que é                                              |
|-----------|------------------------------------------------------|
| `sockfd`  | O file descriptor do socket                          |
| `backlog` | Tamanho máximo da fila de conexões pendentes         |

```c
listen(server_fd, 5);  // fila de até 5 conexões pendentes
```

Após essa chamada, o socket está pronto para receber conexões. Ele **não** aceita automaticamente — isso é papel do `accept()`.

---

### `accept()` — Aceitar uma conexão

```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

Essa função **bloqueia** a execução até que um cliente se conecte. Quando alguém conecta, ela retorna um **novo file descriptor** que representa essa conexão específica.

```c
struct sockaddr_in cliente_addr;
socklen_t cliente_len = sizeof(cliente_addr);

int cliente_fd = accept(server_fd, (struct sockaddr *)&cliente_addr, &cliente_len);
if (cliente_fd == -1) {
    perror("Erro no accept");
    exit(EXIT_FAILURE);
}
printf("Cliente conectado!\n");
```

> **Importante**: o socket original (`server_fd`) continua escutando. O `accept()` cria um **novo** socket para conversar com aquele cliente específico.

---

### `connect()` — Conectar ao servidor (lado do cliente)

```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

```c
struct sockaddr_in servidor_addr;
servidor_addr.sin_family = AF_INET;
servidor_addr.sin_port = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &servidor_addr.sin_addr);  // localhost

connect(client_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));
```

> **`inet_pton()`** converte um endereço IP em formato texto ("127.0.0.1") para o formato binário que o socket precisa. O nome vem de *"presentation to network"*.

---

### `send()` e `recv()` — Enviar e receber dados

```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

```c
// Enviar
char *mensagem = "Olá, servidor!";
send(client_fd, mensagem, strlen(mensagem), 0);

// Receber
char buffer[1024] = {0};
int bytes_recebidos = recv(client_fd, buffer, sizeof(buffer), 0);
printf("Recebido: %s\n", buffer);
```

- O `flags` geralmente é `0` (comportamento padrão)
- `recv()` retorna o número de bytes recebidos, `0` se a conexão foi fechada, ou `-1` em erro
- **`recv()` é bloqueante** — espera até chegar algum dado

---

### `close()` — Fechar o socket

```c
close(server_fd);
close(cliente_fd);
```

Simples assim. Libera o file descriptor e encerra a conexão.

---

## Byte Order — Um detalhe importante

Máquinas diferentes podem armazenar bytes em ordens diferentes:

- **Big-endian** (network byte order): byte mais significativo primeiro
- **Little-endian** (most x86 machines): byte menos significativo primeiro

Para garantir que todos se entendam, a rede usa **big-endian**. As funções de conversão são:

| Função     | Significado                        | Uso                    |
|------------|------------------------------------|------------------------|
| `htons()`  | Host to Network Short (16 bits)    | Converter portas       |
| `htonl()`  | Host to Network Long (32 bits)     | Converter IPs          |
| `ntohs()`  | Network to Host Short              | Ler portas recebidas   |
| `ntohl()`  | Network to Host Long               | Ler IPs recebidos      |

---

## A estrutura `sockaddr_in`

Essa é a estrutura que armazena as informações de endereço para IPv4:

```c
struct sockaddr_in {
    sa_family_t    sin_family;   // AF_INET (IPv4)
    in_port_t      sin_port;     // porta (em network byte order)
    struct in_addr sin_addr;     // endereço IP
};

struct in_addr {
    uint32_t s_addr;             // endereço IP (em network byte order)
};
```

Valores comuns para `sin_addr.s_addr`:
- `INADDR_ANY` → aceita conexões de qualquer interface (0.0.0.0)
- `INADDR_LOOPBACK` → apenas localhost (127.0.0.1)

---

## Compilação

Todos os exemplos desta pasta podem ser compilados com:

```bash
make          # compila tudo
make clean    # limpa os binários
```

Ou manualmente:

```bash
gcc -Wall -Wextra -o servidor servidor_tcp.c
gcc -Wall -Wextra -o cliente cliente_tcp.c
gcc -Wall -Wextra -o servidor_udp servidor_udp.c
gcc -Wall -Wextra -o cliente_udp cliente_udp.c
```

## Como testar

### TCP
```bash
# Terminal 1 — inicie o servidor
./servidor

# Terminal 2 — conecte o cliente
./cliente
```

### UDP
```bash
# Terminal 1 — inicie o servidor
./servidor_udp

# Terminal 2 — envie mensagem
./cliente_udp
```

---

## Exercício proposto

Após rodar os exemplos, tente:

1. **Modificar a porta** — troque `8080` por outra e verifique que funciona
2. **Trocar a mensagem** — envie seu nome ao invés do texto padrão
3. **Loop de mensagens** — faça o cliente enviar várias mensagens em sequência
4. **Servidor que responde** — faça o servidor ecoar (devolver) a mensagem recebida

---

## Arquivos desta pasta

| Arquivo             | Descrição                                  |
|---------------------|---------------------------------------------|
| `servidor_tcp.c`    | Servidor TCP básico (aceita 1 conexão)      |
| `cliente_tcp.c`     | Cliente TCP que se conecta e envia mensagem |
| `servidor_udp.c`    | Servidor UDP básico (recebe datagramas)     |
| `cliente_udp.c`     | Cliente UDP que envia um datagrama          |
| `Makefile`          | Compilação automática de todos os exemplos  |

---

> **Próxima aula**: veremos como lidar com múltiplos clientes simultâneos, tratamento robusto de erros e o uso de `select()`/`poll()`.
