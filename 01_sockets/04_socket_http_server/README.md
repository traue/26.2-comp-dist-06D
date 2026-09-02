# 🌐 Servidor HTTP em C usando Sockets

> **Projeto Acadêmico** — Um servidor HTTP funcional implementado do zero em C puro, usando apenas a API de sockets do sistema operacional (POSIX). Nenhuma biblioteca externa é utilizada.

---

## 📖 Índice

1. [O que é este projeto?](#-o-que-é-este-projeto)
2. [Conceitos Fundamentais](#-conceitos-fundamentais)
3. [Como compilar e executar](#-como-compilar-e-executar)
4. [Estrutura do Projeto](#-estrutura-do-projeto)
5. [Como o servidor funciona (passo a passo)](#-como-o-servidor-funciona-passo-a-passo)
6. [O Protocolo HTTP simplificado](#-o-protocolo-http-simplificado)
7. [Fluxo de uma Requisição](#-fluxo-de-uma-requisição)
8. [Glossário de Funções de Socket](#-glossário-de-funções-de-socket)
9. [Exercícios Sugeridos](#-exercícios-sugeridos)

---

## 🎯 O que é este projeto?

Este é um **servidor HTTP simplificado** que:

- ✅ Aceita conexões de navegadores web reais (Chrome, Firefox, Safari, etc.)
- ✅ Interpreta requisições HTTP `GET`
- ✅ Serve arquivos estáticos (HTML, CSS, JavaScript, imagens)
- ✅ Responde com códigos HTTP corretos (200, 404, 405, 500)
- ✅ Permite configurar a porta via argumento de linha de comando
- ✅ Inclui uma aplicação web de demonstração para teste

**O objetivo é entender como a comunicação entre cliente e servidor funciona "por baixo dos panos"**, sem frameworks ou bibliotecas que abstraiam os detalhes.

---

## 📚 Conceitos Fundamentais

### O que é um Socket?

Um **socket** é um ponto de comunicação entre dois processos na rede. Pense nele como um **telefone**: você precisa de um para fazer ou receber chamadas.

```
[Navegador]  ←──── Socket TCP ────→  [Servidor HTTP em C]
  (cliente)        (conexão)            (este programa)
```

### O que é TCP/IP?

**TCP** (Transmission Control Protocol) é um protocolo que garante:
- Entrega **confiável** dos dados (se algo se perde, reenvia)
- Entrega **ordenada** (os dados chegam na ordem correta)
- **Controle de fluxo** (não sobrecarrega o receptor)

O HTTP funciona **sobre** o TCP — ou seja, usamos TCP como "meio de transporte".

### O que é HTTP?

**HTTP** (HyperText Transfer Protocol) é um protocolo de **texto simples** baseado em **requisição/resposta**:

1. O **cliente** (navegador) envia uma **requisição** (ex: "quero a página index.html")
2. O **servidor** processa e envia uma **resposta** (ex: "aqui está o conteúdo do arquivo")

---

## 🚀 Como compilar e executar

### Pré-requisitos

- **gcc** (compilador C) — já vem instalado no macOS e na maioria das distribuições Linux
- **make** (opcional, mas recomendado)

### Compilar e executar

```bash
# Opção 1: Usando Make (recomendado)
make            # Compila o servidor
make run        # Compila e executa na porta padrão (8080)
make run PORT=3000  # Compila e executa na porta 3000

# Opção 2: Compilação manual
gcc -Wall -Wextra -pedantic -std=c99 -o server server.c

# Executar
./server            # Usa a porta padrão 8080
./server 3000       # Usa a porta 3000
./server 9090       # Usa a porta 9090
```

### Testar no navegador

Após iniciar o servidor, abra seu navegador e acesse:

```
http://localhost:8080
```

(Substitua `8080` pela porta que você escolheu)

Você verá a aplicação web de demonstração com informações sobre o funcionamento do servidor!

### Encerrar o servidor

Pressione `Ctrl+C` no terminal onde o servidor está rodando.

---

## 📁 Estrutura do Projeto

```
socket_http_server/
│
├── server.c           ← Código-fonte do servidor HTTP em C
├── Makefile           ← Automação de compilação
├── README.md          ← Este arquivo (documentação)
│
└── www/               ← Pasta com os arquivos web (servidos pelo servidor)
    ├── index.html     ← Página principal (HTML)
    ├── style.css      ← Estilos visuais (CSS)
    └── app.js         ← Interatividade (JavaScript)
```

- O servidor busca os arquivos na pasta `www/`
- Quando você acessa `http://localhost:8080/`, ele serve `www/index.html`
- Quando o navegador pede `/style.css`, ele serve `www/style.css`
- E assim por diante...

---

## 🔧 Como o servidor funciona (passo a passo)

### Visão Geral: O Ciclo de Vida do Servidor

```
┌─────────────────────────────────────────────────────────┐
│                    SERVIDOR HTTP em C                     │
│                                                          │
│   1. socket()  → Cria o socket (como comprar um telefone)│
│           ↓                                              │
│   2. bind()    → Associa IP e porta (definir o número)   │
│           ↓                                              │
│   3. listen()  → Modo escuta (ligar o telefone)          │
│           ↓                                              │
│   4. accept()  → Espera conexão (atender a chamada)      │
│           ↓                                              │
│   5. read()    → Lê a requisição HTTP do cliente         │
│           ↓                                              │
│   6. Processa  → Interpreta o que o cliente pediu        │
│           ↓                                              │
│   7. write()   → Envia a resposta HTTP                   │
│           ↓                                              │
│   8. close()   → Fecha a conexão com o cliente           │
│           ↓                                              │
│   (volta para o passo 4, esperando nova conexão)         │
└─────────────────────────────────────────────────────────┘
```

### Passo 1: Criação do Socket — `socket()`

```c
int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
```

| Parâmetro     | Significado                              |
|---------------|------------------------------------------|
| `AF_INET`     | Usar IPv4 (Internet Protocol versão 4)   |
| `SOCK_STREAM` | Socket de fluxo = TCP (confiável, ordenado)|
| `0`           | Protocolo padrão (TCP para SOCK_STREAM)  |

**Retorna**: Um "file descriptor" (número inteiro) que identifica o socket. Em Unix, *tudo é arquivo* — sockets também!

### Passo 2: Associar endereço — `bind()`

```c
struct sockaddr_in endereco;
endereco.sin_family      = AF_INET;       // IPv4
endereco.sin_addr.s_addr = htonl(INADDR_ANY);  // Qualquer interface
endereco.sin_port        = htons(8080);   // Porta 8080

bind(socket_fd, (struct sockaddr *)&endereco, sizeof(endereco));
```

- `INADDR_ANY` = aceita conexões em qualquer interface de rede (localhost, Wi-Fi, Ethernet...)
- `htons()` = **H**ost **TO** **N**etwork **S**hort — converte a porta para a ordem de bytes da rede
- `htonl()` = **H**ost **TO** **N**etwork **L**ong — mesma coisa, mas para endereços de 32 bits

### Passo 3: Modo escuta — `listen()`

```c
listen(socket_fd, 10);
```

- O `10` é o **backlog**: tamanho máximo da fila de conexões pendentes
- Após `listen()`, o socket está pronto para receber conexões

### Passo 4: Aceitar conexão — `accept()`

```c
int socket_cliente = accept(socket_servidor, (struct sockaddr *)&endereco_cliente, &tamanho);
```

- **Bloqueia** a execução até um cliente conectar
- Retorna um **novo socket** exclusivo para comunicação com aquele cliente
- O socket original continua disponível para novas conexões

### Passo 5-7: Ler, processar e responder

```c
read(socket_cliente, buffer, tamanho);    // Lê a requisição
// ... processa ...
write(socket_cliente, resposta, tamanho);  // Envia a resposta
```

### Passo 8: Fechar conexão

```c
close(socket_cliente);  // Encerra a comunicação com o cliente
```

---

## 📨 O Protocolo HTTP simplificado

### Requisição HTTP (o que o navegador envia)

```
GET /index.html HTTP/1.1        ← Linha de Requisição (método + caminho + versão)
Host: localhost:8080             ← Cabeçalho: para qual servidor
User-Agent: Mozilla/5.0         ← Cabeçalho: qual navegador está pedindo
Accept: text/html               ← Cabeçalho: que tipo de conteúdo aceita
                                 ← Linha vazia = fim dos cabeçalhos
```

| Campo           | Descrição                                |
|-----------------|------------------------------------------|
| `GET`           | Método HTTP (solicitar um recurso)       |
| `/index.html`   | Caminho do recurso no servidor           |
| `HTTP/1.1`      | Versão do protocolo                      |
| Cabeçalhos      | Informações adicionais (um por linha)    |
| Linha vazia     | Marca o fim dos cabeçalhos               |

### Resposta HTTP (o que o servidor envia)

```
HTTP/1.1 200 OK                  ← Linha de Status (versão + código + texto)
Content-Type: text/html          ← Tipo do conteúdo
Content-Length: 1234             ← Tamanho em bytes
Connection: close                ← Conexão será fechada após a resposta
Server: MeuServidorHTTP/1.0      ← Nome do servidor
                                 ← Linha vazia = fim dos cabeçalhos
<!DOCTYPE html>                  ← Corpo da resposta (o arquivo pedido)
<html>...</html>
```

### Códigos de Status HTTP comuns

| Código | Significado              | Quando acontece                          |
|--------|--------------------------|------------------------------------------|
| `200`  | OK                       | Arquivo encontrado e enviado com sucesso |
| `404`  | Not Found                | Arquivo não existe no servidor           |
| `405`  | Method Not Allowed       | Método não suportado (ex: POST)          |
| `500`  | Internal Server Error    | Erro interno no servidor                 |

### Tipos MIME

O servidor precisa informar ao navegador **que tipo de conteúdo** está enviando, para que ele saiba como tratar:

| Extensão | Tipo MIME                    | O navegador...              |
|----------|------------------------------|-----------------------------|
| `.html`  | `text/html`                  | Renderiza como página web   |
| `.css`   | `text/css`                   | Aplica como folha de estilo |
| `.js`    | `application/javascript`     | Executa como script         |
| `.png`   | `image/png`                  | Exibe como imagem           |
| `.json`  | `application/json`           | Trata como dados JSON       |

---

## 🔄 Fluxo de uma Requisição

```
┌──────────────┐                              ┌──────────────────┐
│              │   1. Conexão TCP (connect)     │                  │
│  NAVEGADOR   │ ──────────────────────────→    │  SERVIDOR HTTP   │
│              │                                │  (nosso server.c)│
│              │   2. Requisição HTTP           │                  │
│   (Chrome,   │ ──────────────────────────→    │                  │
│    Firefox)  │   "GET /index.html HTTP/1.1"  │  Recebe e processa│
│              │                                │                  │
│              │   3. Resposta HTTP             │  Lê o arquivo    │
│              │ ←──────────────────────────    │  www/index.html  │
│              │   "HTTP/1.1 200 OK" + HTML    │                  │
│              │                                │                  │
│  Renderiza   │   4. Nova requisição (CSS)    │                  │
│  o HTML e    │ ──────────────────────────→    │                  │
│  descobre    │   "GET /style.css HTTP/1.1"   │  Lê o arquivo    │
│  que precisa │                                │  www/style.css   │
│  do CSS e JS │   5. Resposta CSS             │                  │
│              │ ←──────────────────────────    │                  │
│              │                                │                  │
│              │   6. Nova requisição (JS)      │                  │
│              │ ──────────────────────────→    │                  │
│              │   "GET /app.js HTTP/1.1"      │  Lê o arquivo    │
│              │                                │  www/app.js      │
│              │   7. Resposta JS              │                  │
│              │ ←──────────────────────────    │                  │
│              │                                │                  │
│  Página      │   8. Fecha conexões           │                  │
│  completa!   │ ─────── close() ──────────    │                  │
└──────────────┘                              └──────────────────┘
```

---

## 📖 Glossário de Funções de Socket

| Função         | Descrição                                                       |
|----------------|-----------------------------------------------------------------|
| `socket()`     | Cria um novo socket (endpoint de comunicação)                   |
| `bind()`       | Associa o socket a um endereço IP e porta                       |
| `listen()`     | Coloca o socket em modo passivo (espera conexões)               |
| `accept()`     | Aceita uma conexão e retorna novo socket para o cliente         |
| `read()`       | Lê dados recebidos do socket                                    |
| `write()`      | Envia dados pelo socket                                         |
| `close()`      | Fecha o socket e libera recursos                                |
| `setsockopt()` | Configura opções do socket (ex: reutilizar porta)               |
| `htons()`      | Converte porta de host byte order para network byte order       |
| `htonl()`      | Converte endereço de host byte order para network byte order    |
| `inet_ntoa()`  | Converte endereço IP binário para string legível                |

---

## 🧪 Exercícios Sugeridos

Para aprofundar o aprendizado, tente implementar estas melhorias:

### Nível Básico
1. **Adicionar mais tipos MIME** — Adicione suporte a `.mp3`, `.mp4`, `.pdf`
2. **Página 404 personalizada** — Crie um arquivo `404.html` na pasta `www/` e sirva-o ao invés da página embutida no código
3. **Logging em arquivo** — Salve os logs em um arquivo `server.log` além de imprimir no terminal

### Nível Intermediário
4. **Suporte a POST** — Leia o corpo da requisição POST e exiba no terminal
5. **Listagem de diretório** — Se o caminho for um diretório, gere um HTML listando os arquivos
6. **Arquivo de configuração** — Leia a porta e o diretório web de um arquivo `config.txt`

### Nível Avançado
7. **Multi-threading** — Use `pthread_create()` para atender múltiplos clientes simultaneamente
8. **Keep-Alive** — Implemente conexões persistentes (HTTP/1.1 Connection: keep-alive)
9. **HTTPS** — Adicione suporte a TLS/SSL usando a biblioteca OpenSSL

---

## 📝 Referências

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) — O melhor guia de sockets em C
- [RFC 2616 - HTTP/1.1](https://www.rfc-editor.org/rfc/rfc2616) — Especificação oficial do HTTP/1.1
- [man pages: socket(2)](https://man7.org/linux/man-pages/man2/socket.2.html) — Documentação oficial das system calls

---

## 📄 Licença

Projeto acadêmico para fins educacionais. Uso livre para aprendizado.
