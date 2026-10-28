# Mini Blog — Computação Distribuída com gRPC

> Projeto acadêmico para a disciplina **Computação Distribuída**
> Tecnologias: Java 21 · **Maven** · gRPC · Protocol Buffers · MongoDB · Docker

---

## Visão Geral da Arquitetura

```
┌──────────────────────────────────────────────────────────────────┐
│                         CLIENTE (blog-client)                    │
│                                                                  │
│  BlogClient.java                                                 │
│    ├── PostServiceBlockingStub  ─── RPC Unário + Server Stream   │
│    └── CommentServiceBlockingStub ─ RPC Unário + Server Stream   │
└──────────────────────────┬───────────────────────────────────────┘
                           │ HTTP/2 + Protocol Buffers (porta 9090)
                           │ (binário, comprimido, tipado)
┌──────────────────────────▼───────────────────────────────────────┐
│                         SERVIDOR (blog-server)                   │
│                                                                  │
│  BlogServer.java                                                 │
│    ├── PostServiceImpl    ── CRUD + Streaming de Posts           │
│    └── CommentServiceImpl ── CRUD + Streaming de Comentários     │
│         ├── PostRepository    ──→ coleção "posts"                │
│         └── CommentRepository ──→ coleção "comments"             │
└──────────────────────────┬───────────────────────────────────────┘
                           │ Driver MongoDB Java (porta 27017)
┌──────────────────────────▼───────────────────────────────────────┐
│                    INFRAESTRUTURA (Docker)                       │
│                                                                  │
│  MongoDB 7.0         → porta 27017 (banco de dados)              │
│  Mongo Express 1.0.2 → porta 8081  (painel admin web)            │
└──────────────────────────────────────────────────────────────────┘
```

### Tipos de RPC demonstrados

| Método gRPC          | Tipo                     | Uso no projeto                    |
|----------------------|--------------------------|-----------------------------------|
| `CreatePost`         | Unário                   | Cria uma postagem                 |
| `GetPost`            | Unário                   | Busca post por ID                 |
| `UpdatePost`         | Unário                   | Atualiza título/conteúdo          |
| `DeletePost`         | Unário                   | Remove uma postagem               |
| `ListPosts`          | **Server Streaming**     | Lista posts página a página       |
| `AddComment`         | Unário                   | Adiciona comentário               |
| `DeleteComment`      | Unário                   | Remove comentário                 |
| `GetComments`        | **Server Streaming**     | Lista comentários de um post      |

---

## Estrutura do Projeto

```
grpc-blog/
├── pom.xml                             ← POM raiz (parent) multi-módulo
├── mvnw, mvnw.cmd, .mvn/               ← Maven Wrapper (só se não houver Maven instalado)
├── docker-compose.yml                  ← MongoDB + Mongo Express
├── db-init/
│   └── init.js                         ← dados iniciais de exemplo
│
├── blog-proto/                         ← MÓDULO 1: definição do contrato
│   ├── pom.xml                         ← protobuf-maven-plugin
│   └── src/main/proto/
│       └── blog.proto                  ← ★ arquivo central do projeto
│
├── blog-server/                        ← MÓDULO 2: servidor gRPC
│   ├── pom.xml
│   └── src/main/java/com/blog/server/
│       ├── BlogServer.java             ← ponto de entrada do servidor
│       ├── config/
│       │   └── MongoDBConfig.java      ← conexão singleton com MongoDB
│       ├── repository/
│       │   ├── PostRepository.java     ← CRUD de posts no MongoDB
│       │   └── CommentRepository.java  ← CRUD de comentários
│       └── service/
│           ├── PostServiceImpl.java    ← implementação dos RPCs de post
│           └── CommentServiceImpl.java ← implementação dos RPCs de comentário
│
└── blog-client/                        ← MÓDULO 3: cliente CLI interativo
    ├── pom.xml
    └── src/main/java/com/blog/client/
        └── BlogClient.java             ← menu interativo gRPC
```

> **Importante:** o `pom.xml` raiz lista os três módulos (`blog-proto`,
> `blog-server`, `blog-client`), e o Maven lê **todos** antes de executar
> qualquer comando, mesmo com `-pl`. Cada módulo precisa existir como pasta
> **com o seu próprio `pom.xml`**, ainda que sem código. Sem isso, o build
> para com `Child module ... does not exist`.

---

## Pré-requisitos

| Ferramenta      | Versão mínima | Verificar                    |
|-----------------|---------------|------------------------------|
| Java (JDK)      | 21            | `java -version`              |
| Docker          | 24+           | `docker --version`           |
| Docker Compose  | 2.x           | `docker compose version`     |
| Git             | qualquer      | `git --version`              |
| Maven           | 3.6.3         | `mvn -v`                     |

> **Sobre o Maven:** os comandos deste guia usam o `mvn` instalado na máquina
> (versão 3.6.3 ou superior; o Maven dos repositórios do Ubuntu serve).
> Se a máquina **não** tiver Maven, use o **Maven Wrapper** incluído no projeto:
> troque `mvn` por `./mvnw` (Linux/macOS) ou `mvnw.cmd` (Windows) em todos os
> comandos. Na primeira execução, o wrapper baixa o Maven sozinho.

---

## Guia de Laboratório — Passo a Passo

### ETAPA 1 — Clonar/Baixar o Projeto

```bash
# Opção A: clonar via Git
git clone <URL-do-repositório>
cd grpc-blog

# Opção B: extrair o ZIP fornecido
unzip grpc-blog.zip
cd grpc-blog
```

Confira se você está na **raiz do projeto**. Todos os comandos `mvn` e
`docker compose` deste guia são executados a partir dela:

```bash
ls
# blog-client  blog-proto  blog-server  db-init  docker-compose.yml  mvnw  mvnw.cmd  pom.xml  README.md
```

Se for usar o wrapper no **Linux/macOS**, garanta a permissão de execução:
```bash
chmod +x mvnw
```

---

### ETAPA 2 — Subir a Infraestrutura com Docker

```bash
# Sobe MongoDB e Mongo Express em background
docker compose up -d

# Verifica se os containers estão rodando
docker compose ps

# Saída esperada:
# NAME                 STATUS              PORTS
# blog-mongodb         running (healthy)   0.0.0.0:27017->27017/tcp
# blog-mongo-express   running             0.0.0.0:8081->8081/tcp
```

**Verificar o painel admin:**
- Abra o navegador em: **http://localhost:8081**
- Login: `admin` / Senha: `senha123`
- Você verá o banco `blogdb` com dados de exemplo já inseridos

**Explorar os dados iniciais:**
```bash
# Conectar ao MongoDB via terminal
docker exec -it blog-mongodb mongosh \
  "mongodb://root:senha123@localhost:27017/blogdb?authSource=admin"

# Dentro do mongosh:
use blogdb
db.posts.find().pretty()       # ver posts de exemplo
db.comments.find().pretty()    # ver comentários de exemplo
exit
```

---

### ETAPA 3 — Entender o Arquivo Proto (Ponto Central)

Antes de compilar, leia o arquivo que define o contrato do sistema:

```bash
cat blog-proto/src/main/proto/blog.proto
```

**Conceitos para discutir em aula:**
1. O que é um `message` no Protobuf? Como se compara a uma classe Java?
2. O que são os números `= 1`, `= 2` nos campos?
3. Qual a diferença entre `returns (Post)` e `returns (stream Post)`?
4. Por que o .proto é o "contrato" entre cliente e servidor?

---

### ETAPA 4 — Gerar o Código Java a partir do Proto

O `protobuf-maven-plugin` (declarado em [blog-proto/pom.xml](blog-proto/pom.xml))
executa duas etapas durante a fase `generate-sources`:

| Goal Maven                 | Função                                                       |
|----------------------------|--------------------------------------------------------------|
| `protobuf:compile`         | Roda o `protoc` → gera mensagens (`Post.java`, `Comment.java`...) |
| `protobuf:compile-custom`  | Roda o `protoc-gen-grpc-java` → gera os stubs gRPC           |

**Montando o projeto do zero?** Para gerar o código do proto, a estrutura
mínima é esta (os módulos `blog-server` e `blog-client` podem estar só com o
`pom.xml`, sem código ainda):

```
grpc-blog/
├── pom.xml                            ← POM raiz (lista os 3 módulos)
├── blog-proto/
│   ├── pom.xml
│   └── src/main/proto/blog.proto      ← o .proto precisa estar exatamente aqui
├── blog-server/
│   └── pom.xml
└── blog-client/
    └── pom.xml
```

```bash
# Gera apenas o código a partir do blog.proto (sem compilar Java ainda)
mvn -pl blog-proto generate-sources

# Onde o código é gerado:
ls blog-proto/target/generated-sources/protobuf/
# java/      → mensagens (Post.java, Comment.java, CreatePostRequest.java...)
# grpc-java/ → stubs (PostServiceGrpc.java, CommentServiceGrpc.java)
```

**Explorar o código gerado:**
```bash
# Stubs gRPC (servidor + cliente)
ls blog-proto/target/generated-sources/protobuf/grpc-java/com/blog/grpc/generated/
# PostServiceGrpc.java        ← contém PostServiceImplBase (servidor)
# CommentServiceGrpc.java     #          e PostServiceBlockingStub (cliente)

# Classes de mensagem
ls blog-proto/target/generated-sources/protobuf/java/com/blog/grpc/generated/
# Post.java, Comment.java, CreatePostRequest.java ...
```

> **Observação:** O `protoc` é um binário nativo. O plugin usa o
> `os-maven-plugin` (extension declarado no POM) para detectar o sistema
> operacional/arquitetura e baixar o binário correto do Maven Central
> (`linux-x86_64`, `osx-aarch_64`, `windows-x86_64` etc.). Não é
> necessário instalar o `protoc` manualmente.

**Para discutir em aula:**
- Abra `PostServiceGrpc.java` e encontre `PostServiceImplBase`
- Esta é a classe que `PostServiceImpl.java` estende
- Como o Maven descobriu o que gerar? (resposta: o plugin
  `org.xolstice.maven.plugins:protobuf-maven-plugin` configurado em
  [blog-proto/pom.xml](blog-proto/pom.xml))

---

### ETAPA 5 — Compilar o Projeto Completo

```bash
# Compila todos os módulos (proto → server → client)
mvn clean package

# Saída esperada: BUILD SUCCESS
# Os JARs ficam em: <modulo>/target/<modulo>-1.0.0.jar
```

Pulando a fase de testes (mais rápido):
```bash
mvn clean package -DskipTests
```

Se houver erro, verifique:
```bash
# Confirmar versão do Java
java -version   # deve mostrar 21.x

# Limpar e recompilar com saída detalhada
mvn clean package -X
```

---

### ETAPA 6 — Iniciar o Servidor gRPC

Abra um **novo terminal** (ou aba) na raiz do projeto:

```bash
# Compila o blog-proto (do qual o servidor depende) no mesmo build
# e depois executa o servidor.
mvn -pl blog-server -am compile exec:java

# Saída esperada:
# ╔══════════════════════════════════════════╗
# ║   Blog gRPC Server — porta: 9090         ║
# ║   MongoDB: conectado                     ║
# ║   Serviços: PostService, CommentService  ║
# ╚══════════════════════════════════════════╝
```

Explicando as flags:
- `-pl blog-server` → executa apenas no módulo blog-server.
- `-am` → também compila os módulos dos quais ele depende (blog-proto).
- `compile exec:java` → compila e em seguida roda a classe principal
  declarada no `pom.xml` (`com.blog.server.BlogServer`).

**Servidor em porta diferente** (passando argumentos):
```bash
mvn -pl blog-server -am compile exec:java -Dexec.args="8080"
```

**Verificar que o servidor está aceitando conexões:**
```bash
# Em outro terminal
nc -zv localhost 9090
# Saída: Connection to localhost port 9090 [tcp] succeeded!
```

---

### ETAPA 7 — Executar o Cliente gRPC

Abra **outro terminal** e execute:

```bash
# Inicia o cliente interativo
mvn -pl blog-client -am compile exec:java

# Menu exibido:
# ╔══════════════════════════════════════╗
# ║       BLOG — Cliente gRPC            ║
# ╠══════════════════════════════════════╣
# ...
```

> **Dica:** Se o terminal não estiver repassando o stdin para o processo
> Java (cenário raro), execute o JAR diretamente:
> ```bash
> mvn -pl blog-client -am install -DskipTests
> java -cp "blog-client/target/blog-client-1.0.0.jar:$(mvn -pl blog-client dependency:build-classpath -q -DincludeScope=runtime -Dmdep.outputFile=/dev/stdout 2>/dev/null)" \
>   com.blog.client.BlogClient
> ```

---

### ETAPA 8 — Exercícios Práticos em Laboratório

#### Exercício 1 — Criar e buscar um post

1. No cliente, escolha **1 (Criar post)**
2. Preencha título, autor e conteúdo
3. Copie o **ID** retornado
4. Escolha **2 (Buscar post por ID)** e cole o ID
5. Observe no Mongo Express: **http://localhost:8081** → `blogdb` → `posts`

#### Exercício 2 — Demonstrar Server Streaming

1. Crie 3 posts diferentes pelo cliente
2. Escolha **3 (Listar posts)**
3. Observe no terminal do **servidor** as linhas de log:
   ```
   INFO: RPC listPosts (streaming). Página: 0, Tamanho: 10
   INFO: Stream listPosts concluído. Posts enviados: 3
   ```
4. **Pergunta:** como o cliente recebe os dados? (Iterator, um por vez)

#### Exercício 3 — Integridade referencial

1. Tente adicionar um comentário com um ID de post **inexistente**
2. O servidor retorna: `NOT_FOUND: Post não encontrado`
3. Discuta: como bancos SQL resolveriam isso? (Foreign Key)
4. Como o gRPC comunica o erro ao cliente? (StatusRuntimeException)

#### Exercício 4 — Observar o Protocol Buffers

```bash
# No servidor, adicione um log temporário em PostServiceImpl.createPost()
# para imprimir request.getSerializedSize() (tamanho em bytes)
# Compare com o equivalente JSON para entender a compressão
```

#### Exercício 5 — Parar o servidor e observar o comportamento do cliente

1. Com o cliente aberto, pressione `Ctrl+C` no servidor
2. Tente fazer uma operação no cliente
3. O que acontece? Qual exceção é lançada?

---

### ETAPA 9 — Comandos Maven Úteis

```bash
# Ver logs do MongoDB em tempo real
docker compose logs -f mongodb

# Ver logs do Mongo Express
docker compose logs -f mongo-express

# Reiniciar apenas o MongoDB
docker compose restart mongodb

# Parar tudo (preserva dados)
docker compose down

# Parar e apagar todos os dados (volume)
docker compose down -v

# Verificar posts diretamente no banco
docker exec -it blog-mongodb mongosh \
  "mongodb://root:senha123@localhost:27017/blogdb?authSource=admin" \
  --eval "db.posts.find({}, {title:1, author:1, commentCount:1}).pretty()"

# ----- Maven -----

# Compilar apenas um módulo (sem dependências)
mvn -pl blog-proto package

# Compilar um módulo e tudo que ele depende (ex: blog-server precisa de blog-proto)
mvn -pl blog-server -am package

# Rodar o servidor em uma porta específica
mvn -pl blog-server -am compile exec:java -Dexec.args="8080"

# Forçar regeneração dos protos
mvn -pl blog-proto clean generate-sources

# Listar a árvore de dependências de um módulo
mvn -pl blog-server -am dependency:tree

# Build offline (após primeira execução, usa só o cache em ~/.m2)
mvn -o clean package
```

---

## Como o Projeto Funciona — Detalhes Técnicos

### Fluxo de uma chamada RPC Unária (ex: CreatePost)

```
BlogClient.java               PostServiceImpl.java         MongoDB
     │                               │                        │
     │ CreatePostRequest {           │                        │
     │   title: "Meu Post"           │                        │
     │   author: "João"              │                        │
     │   content: "..."              │                        │
     │ }                             │                        │
     │ ──── HTTP/2 frame ──────────► │                        │
     │      (binário protobuf)       │                        │
     │                               │─── insertOne(doc) ────►│
     │                               │◄── ObjectId gerado ────│
     │                               │                        │
     │ Post {                        │                        │
     │   id: "6789abc...",           │                        │
     │   title: "Meu Post"           │                        │
     │   ...                         │                        │
     │ }                             │                        │
     │◄─── HTTP/2 frame ──────────── │                        │
     │     (binário protobuf)        │                        │
```

### Fluxo de Server Streaming (ex: ListPosts)

```
BlogClient.java               PostServiceImpl.java         MongoDB
     │                               │                        │
     │ ListPostsRequest {            │                        │
     │   page: 0, page_size: 10      │                        │
     │ }                             │                        │
     │ ──── HTTP/2 frame ──────────► │                        │
     │                               │──── find().limit() ───►│
     │                               │◄─── cursor (lazy) ─────│
     │ Post { id: "aaa..." }         │                        │
     │◄──────────────────────────────│ onNext(post1)          │
     │ Post { id: "bbb..." }         │                        │
     │◄──────────────────────────────│ onNext(post2)          │
     │ Post { id: "ccc..." }         │                        │
     │◄──────────────────────────────│ onNext(post3)          │
     │ [FIM DO STREAM]               │                        │
     │◄──────────────────────────────│ onCompleted()          │
```

### Serialização Protocol Buffers vs JSON

```
Post em JSON (texto):
{"id":"507f1f77bcf86cd799439011","title":"Intro ao gRPC","author":"João",...}
Tamanho: ~120 bytes

Post em Protocol Buffers (binário):
0a 18 35 30 37 66 31 66 37 37...
Tamanho: ~60 bytes (≈ 50% menor)
Velocidade de parse: ~10x mais rápida
```

### Tratamento de Erros no gRPC

O gRPC define códigos de status (análogos ao HTTP):

| Código gRPC        | HTTP equiv | Uso no projeto                          |
|--------------------|------------|------------------------------------------|
| `OK`               | 200        | Sucesso (implícito no `onCompleted()`)   |
| `NOT_FOUND`        | 404        | Post ou comentário não existe            |
| `INVALID_ARGUMENT` | 400        | Campos obrigatórios ausentes             |
| `INTERNAL`         | 500        | Erro inesperado no servidor              |

No cliente, erros chegam como `StatusRuntimeException`:
```java
try {
    Post post = postStub.getPost(request);
} catch (StatusRuntimeException e) {
    e.getStatus().getCode();        // NOT_FOUND
    e.getStatus().getDescription(); // "Post não encontrado. ID: ..."
}
```

---

## Estrutura do banco de dados (MongoDB)

### Coleção: `posts`
```json
{
  "_id": "ObjectId(...)",
  "title": "Introdução ao gRPC",
  "content": "Texto do post...",
  "author": "Prof. Silva",
  "createdAt": "2024-01-15T10:30:00Z",
  "updatedAt": "2024-01-15T10:30:00Z",
  "commentCount": 2
}
```

### Coleção: `comments`
```json
{
  "_id": "ObjectId(...)",
  "postId": "507f1f77bcf86cd799439011",
  "author": "Aluno João",
  "content": "Ótima explicação!",
  "createdAt": "2024-01-15T11:00:00Z"
}
```

**Índices criados automaticamente:**
- `posts.createdAt` (decrescente) → acelera listagem por data
- `comments.postId` (crescente) → acelera busca de comentários por post

---

## Perguntas para Reflexão (Discussão em Aula)

1. **RPC vs REST:** Quais as vantagens e desvantagens do gRPC em relação ao REST/JSON?

2. **Streaming:** Em quais cenários do mundo real o Server Streaming seria mais útil que uma resposta única? (feeds, notificações, exportação de dados)

3. **Protocol Buffers:** Por que definir um contrato em um arquivo .proto separado ao invés de usar JSON livre?

4. **Integridade referencial:** Como o MongoDB difere do PostgreSQL no tratamento de relacionamentos entre coleções?

5. **Escalabilidade:** Se precisarmos de múltiplas instâncias do servidor, o que mudaria na arquitetura? (load balancer, sessões, etc.)

6. **Segurança:** Este projeto usa `usePlaintext()`. O que seria necessário para adicionar TLS em produção?

---

## Troubleshooting

### "Connection refused" ao iniciar o servidor
```bash
# Verifique se o MongoDB está rodando
docker compose ps
docker compose up -d mongodb
```

### "BUILD FAILURE" ao compilar
```bash
# Verifique a versão do Java (precisa ser 21+)
java -version

# Limpe caches do projeto
mvn clean

# Compile com saída detalhada para diagnóstico
mvn clean package -X
```

### `Child module ... does not exist`
A pasta de um módulo (ou o `pom.xml` dentro dela) não existe. O `pom.xml` raiz
lista os três módulos, e o Maven lê todos antes de rodar qualquer comando,
mesmo com `-pl`. Confira a estrutura a partir da raiz do projeto:
```bash
ls */pom.xml
# blog-client/pom.xml  blog-proto/pom.xml  blog-server/pom.xml
```

### `./mvnw: Arquivo ou diretório inexistente`
O `mvnw` não está na pasta atual: você não está na raiz do projeto, ou a pasta
foi copiada pela metade. Se a máquina tiver Maven, basta usar `mvn`.

### `Could not find the selected project in the reactor`
O nome passado no `-pl` está errado (ex.: `blogo-proto`). Use exatamente
`blog-proto`, `blog-server` ou `blog-client`.

### Avisos `WARNING: ... sun.misc.Unsafe ...` no início da saída
Aparecem com JDK 24 ou mais novo e vêm do próprio Maven. São inofensivos.

### Erro `Could not resolve dependencies` no primeiro build
A primeira execução baixa todas as dependências do Maven Central.
É necessário acesso à internet. Tente novamente:
```bash
mvn -U clean package    # -U força update dos metadados
```

### Maven Wrapper sem permissão de execução (Linux/macOS, só para quem usa o wrapper)
```bash
chmod +x mvnw
```

### Geração de código Protobuf falhou
```bash
# Limpa apenas o blog-proto e regenera
mvn -pl blog-proto clean generate-sources

# Se o erro mencionar 'os.detected.classifier' indefinido,
# significa que a extension os-maven-plugin não foi carregada.
# Confirme que blog-proto/pom.xml contém a tag <extensions>.
```

Se o build termina sem erro, mas nada aparece em
`blog-proto/target/generated-sources/protobuf/`, o `.proto` não está em
`blog-proto/src/main/proto/`.

### Mongo Express não abre
```bash
# Verifique se o MongoDB está healthy primeiro
docker compose logs mongo-express
docker compose restart mongo-express
```

### Dados de exemplo não aparecem
```bash
# O init.js só roda na PRIMEIRA vez que o volume é criado
# Para rodar novamente, apague o volume (isso apaga os dados do banco):
docker compose down -v
docker compose up -d

# Alternativa sem apagar os dados (rode uma vez só):
docker exec blog-mongodb mongosh "mongodb://root:senha123@localhost:27017/admin" /docker-entrypoint-initdb.d/init.js
```

---

## Referências

- [gRPC — Documentação oficial](https://grpc.io/docs/)
- [Protocol Buffers — Guia de linguagem](https://protobuf.dev/programming-guides/proto3/)
- [gRPC Java — GitHub](https://github.com/grpc/grpc-java)
- [MongoDB Java Driver](https://www.mongodb.com/docs/drivers/java/sync/current/)
- [protobuf-maven-plugin (xolstice)](https://www.xolstice.org/protobuf-maven-plugin/)
- [Apache Maven](https://maven.apache.org/guides/index.html)
- [Maven Wrapper](https://maven.apache.org/wrapper/)
