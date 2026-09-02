# Chat multiclientes com Sockets em C


## Objetivo

Construir um **chat em tempo real** onde múltiplos clientes se conectam a um servidor e trocam mensagens livremente entre si. Qualquer mensagem enviada por um cliente é retransmitida a **todos os outros** conectados.

```
┌──────────┐          ┌──────────────┐          ┌──────────┐
│ Cliente A │ ──TCP──▶ │              │ ──TCP──▶ │ Cliente B │
│           │ ◀──TCP── │   SERVIDOR   │ ◀──TCP── │           │
└──────────┘          │  (broadcast) │          └──────────┘
                      │              │
┌──────────┐          │              │          ┌──────────┐
│ Cliente C │ ──TCP──▶ │              │ ──TCP──▶ │ Cliente D │
│           │ ◀──TCP── │              │ ◀──TCP── │           │
└──────────┘          └──────────────┘          └──────────┘
```

---

## O problema: como atender vários clientes ao mesmo tempo?

Na Parte 1, nosso servidor era **sequencial** — aceitava um cliente, atendia, e encerrava. Num chat, o servidor precisa:

1. Aceitar **novas conexões** a qualquer momento
2. Receber mensagens de **qualquer cliente** conectado
3. **Retransmitir** cada mensagem para todos os outros

Se usarmos `accept()` ou `recv()` diretamente, o programa **bloqueia** esperando aquela operação específica — e ignora tudo o mais. Precisamos de uma forma de monitorar **múltiplos file descriptors ao mesmo tempo**.

---

## A solução: `select()`

A função `select()` resolve exatamente esse problema. Ela permite que você passe uma **lista de file descriptors** e pergunte ao sistema operacional: *"algum destes tem dados prontos para leitura?"*

```c
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```

| Parâmetro   | O que é                                                       |
|-------------|---------------------------------------------------------------|
| `nfds`      | Valor do maior file descriptor + 1                            |
| `readfds`   | Conjunto de fds que queremos monitorar para **leitura**       |
| `writefds`  | Conjunto de fds para escrita (usamos `NULL` aqui)             |
| `exceptfds` | Conjunto de fds para exceções (usamos `NULL` aqui)            |
| `timeout`   | Tempo máximo de espera (`NULL` = espera indefinidamente)      |

**Retorno**: número de fds prontos, `0` se deu timeout, `-1` se erro.

### Macros para manipular `fd_set`

`fd_set` é um conjunto (bitfield) de file descriptors. Você manipula com estas macros:

| Macro                 | O que faz                                        |
|-----------------------|--------------------------------------------------|
| `FD_ZERO(&set)`       | Limpa o conjunto (zera todos os bits)            |
| `FD_SET(fd, &set)`    | Adiciona `fd` ao conjunto                        |
| `FD_CLR(fd, &set)`    | Remove `fd` do conjunto                          |
| `FD_ISSET(fd, &set)`  | Retorna verdadeiro se `fd` está pronto no conjunto |

### Fluxo com `select()`

```
Início do loop:
    │
    ├── FD_ZERO()         →  limpa o conjunto
    ├── FD_SET(server_fd) →  adiciona o socket do servidor
    ├── FD_SET(client1)   →  adiciona cada cliente conectado
    ├── FD_SET(client2)   →  ...
    │
    ├── select()          →  BLOQUEIA até algum fd ter atividade
    │
    ├── FD_ISSET(server_fd)?  →  Sim: nova conexão! → accept()
    ├── FD_ISSET(client1)?    →  Sim: mensagem! → recv() + broadcast
    ├── FD_ISSET(client2)?    →  Sim: mensagem! → recv() + broadcast
    │
    └── Volta ao início do loop
```

> **Ponto-chave**: `select()` **modifica** o `fd_set` passado — ao retornar, apenas os fds prontos continuam marcados. Por isso, precisamos reconstruir o conjunto a cada iteração do loop.

---

## Lado do cliente: mesmo problema

O cliente também tem duas fontes de entrada simultâneas:

1. **Teclado** (stdin) — o usuário digitando mensagens
2. **Socket** — mensagens chegando dos outros clientes

Se fizermos `fgets()` no teclado, bloqueamos e não recebemos mensagens. Se fizermos `recv()` no socket, bloqueamos e não lemos o teclado.

**Solução**: usar `select()` no cliente também, monitorando o file descriptor `STDIN_FILENO` (valor `0`) e o socket ao mesmo tempo.

---

## Arquitetura do projeto

### Servidor (`chat_servidor.c`)

```
main()
  │
  ├── Cria socket, bind, listen
  │
  └── Loop infinito:
        │
        ├── Monta fd_set com server_fd + todos os clientes
        ├── select()
        │
        ├── Se server_fd pronto → accept() nova conexão
        │     └── Avisa todos: "Fulano entrou no chat"
        │
        └── Para cada cliente pronto:
              ├── recv() a mensagem
              ├── Se recebeu 0 bytes → cliente desconectou
              │     └── Avisa todos: "Fulano saiu do chat"
              └── Senão → broadcast da mensagem para todos os OUTROS
```

### Cliente (`chat_cliente.c`)

```
main()
  │
  ├── Cria socket, connect ao servidor
  ├── Pede o nome do usuário
  │
  └── Loop infinito:
        │
        ├── Monta fd_set com STDIN e o socket
        ├── select()
        │
        ├── Se STDIN pronto → lê mensagem do teclado → send()
        │
        └── Se socket pronto → recv() → exibe na tela
```

---

## Conceitos novos nesta parte

### `setsockopt()` com `SO_REUSEADDR`

Já vimos brevemente na Parte 1. Aqui é **essencial**, porque durante o desenvolvimento você vai reiniciar o servidor muitas vezes:

```c
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

### Broadcast de mensagens

"Broadcast" aqui significa percorrer todos os clientes conectados e enviar a mensagem para cada um, **exceto** quem enviou:

```c
for (int i = 0; i < num_clientes; i++) {
    if (clientes[i] != remetente_fd) {
        send(clientes[i], mensagem, tamanho, 0);
    }
}
```

### Tratamento de desconexão

Quando `recv()` retorna `0`, significa que o outro lado fechou a conexão. Precisamos:

1. Fechar o file descriptor com `close()`
2. Remover o cliente da lista
3. Avisar os outros que ele saiu

---

## Compilação e execução

```bash
make              # compila servidor e cliente
make clean        # remove binários
```

### Testando

Abra **3+ terminais** na pasta `02_socket_chat`:

```bash
# Terminal 1 — servidor
./bin/chat_servidor

# Terminal 2 — primeiro cliente
./bin/chat_cliente

# Terminal 3 — segundo cliente
./bin/chat_cliente

# Terminal 4 (opcional) — mais clientes...
./bin/chat_cliente
```

Cada cliente escolhe um nome ao entrar. Todas as mensagens digitadas em um cliente aparecem nos outros.

Para sair, o cliente pode digitar `sair` ou pressionar `Ctrl+C`.

---

## Limitações

Este é um exemplo **didático**. Em produção, você precisaria lidar com:

- **Mensagens parciais** — `send()`/`recv()` podem não enviar/receber tudo de uma vez
- **Buffer overflow** — validar tamanhos rigorosamente
- **Concorrência** — `select()` tem limite de fds (`FD_SETSIZE`, geralmente 1024). Para escala real, use `epoll` (Linux) ou `kqueue` (macOS)
- **Segurança** — sem criptografia, sem autenticação

---