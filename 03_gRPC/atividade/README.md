# Atividade Avaliativa — gRPC com Java

## Disciplina: Computação Distribuídos
## Tema: Comunicação entre serviços com gRPC e Protocol Buffers

---

# Pré-requisitos

- Projeto base **Calc gRPC** funcionando localmente (servidor e cliente se comunicando).
- Java 17 + Maven instalados.
- IDE de preferência (IntelliJ IDEA recomendado).

---

# Enunciado

Você foi contratado para desenvolver o backend de um **Sistema de Gerenciamento de Biblioteca Digital** distribuído. O sistema é composto por um servidor gRPC central que oferece serviços para gerenciar livros, empréstimos e relatórios em tempo real.

A aplicação deve ser entregue em **Java 17 + Maven + gRPC**, seguindo a mesma estrutura do projeto base estudado em aula.

---

# Requisitos funcionais

## Serviço: `BibliotecaService`

Você deve implementar **quatro operações RPC**, cada uma usando um tipo diferente de comunicação gRPC:

### 1️⃣ Unary RPC — `cadastrarLivro`
Cadastra um novo livro no acervo.

- **Request:** dados do livro (título, autor, ano, ISBN).
- **Response:** confirmação com ID gerado e status.

### 2️⃣ Server Streaming RPC — `listarLivrosPorAutor`
Retorna todos os livros de um autor específico em formato de stream.

- **Request:** nome do autor.
- **Response:** stream de livros (um a um).

### 3️⃣ Client Streaming RPC — `registrarEmprestimos`
Recebe múltiplos empréstimos do cliente em stream e responde com um resumo ao final.

- **Request:** stream de empréstimos (usuário + livro_id).
- **Response:** resumo único com total de empréstimos registrados e tempo total de processamento.

### 4️⃣ Bidirectional Streaming RPC — `chatBibliotecario`
Simula um chat entre usuário e bibliotecário virtual. Para cada mensagem do usuário, o servidor responde com uma sugestão de livro relacionada à palavra-chave enviada.

- **Request:** stream de mensagens do usuário.
- **Response:** stream de sugestões de livros.

---

# Requisitos técnicos

| Requisito | Descrição |
|---|---|
| **Linguagem** | Java 17 (ou seperior) |
| **Build** | Maven |
| **Comunicação** | gRPC 1.68.x + Protocol Buffers 3 |
| **Persistência** | Em memória (`HashMap` / `List`) — não precisa de banco de dados |
| **Estrutura** | Pacotes separados: `server/`, `client/`, `model/` |
| **Tratamento de erros** | Usar `Status` do gRPC (ex: `NOT_FOUND`, `INVALID_ARGUMENT`) |
| **Logs** | Servidor deve imprimir cada chamada recebida (método, parâmetros) |

---

# Estrutura sugerida do projeto

```text
biblioteca-grpc/
│
├── pom.xml
│
└── src/
    └── main/
        ├── java/
        │   └── br/
        │       └── mackenzie/
        │           └── biblioteca/
        │               ├── server/
        │               │   ├── BibliotecaServiceImpl.java
        │               │   └── ServidorBiblioteca.java
        │               │
        │               ├── client/
        │               │   └── ClienteBiblioteca.java
        │               │
        │               └── model/
        │                   └── (classes auxiliares se necessário)
        │
        └── proto/
            └── biblioteca.proto
```

---

# Roteiro de testes (entregar como demonstração)

No `ClienteBiblioteca.java`, demonstre **cada um dos 4 RPCs** com pelo menos:

1. Cadastrar **3 livros** diferentes (Unary).
2. Listar livros de **um autor cadastrado** (Server Streaming).
3. Listar livros de um **autor inexistente** — deve retornar stream vazia ou erro `NOT_FOUND`.
4. Registrar **5 empréstimos** consecutivos (Client Streaming).
5. Realizar um **chat com pelo menos 3 mensagens** (Bidirectional Streaming).
6. Tentar cadastrar um livro com **ISBN duplicado** — deve retornar erro `ALREADY_EXISTS`.

---

# Entrega

## Formato
- Repositório **público no GitHub** com o código completo.
- Arquivo `README.md` na raiz contendo:
  - Descrição do projeto
  - Como compilar e executar (`mvn clean compile`, comandos para servidor e cliente)
  - Print(s) da saída esperada
  - Nome completo e RA do(s) aluno(s)
- No Moodle, entregar um aruivo com o link para o repositório e os testes (prints mesmo) realizados.


---

# Critérios de avaliação (100 pontos)

| Critério | Pontos |
|---|---:|
| Projeto compila e executa sem erros (`mvn clean package`) | 10 |
| Arquivo `.proto` bem definido, com mensagens e serviços corretos | 10 |
| Implementação correta do **Unary RPC** (cadastrarLivro) | 15 |
| Implementação correta do **Server Streaming** (listarLivrosPorAutor) | 15 |
| Implementação correta do **Client Streaming** (registrarEmprestimos) | 15 |
| Implementação correta do **Bidirectional Streaming** (chatBibliotecario) | 15 |
| Tratamento de erros com `Status` do gRPC | 10 |
| Organização do código, nomenclatura, separação em pacotes | 5 |
| `README.md` claro e completo | 5 |

**Bônus (+10 pontos):**
- Implementar um **interceptor gRPC** para log automático de todas as chamadas.
- Adicionar **deadline/timeout** nas chamadas do cliente.
- Implementar **autenticação simples** via metadata (token no header).

> Cada bônus vale +5 pontos. Limite total: **110 pontos**.

---

# Dicas

- Comece pelo **`.proto`** — defina todas as mensagens e serviços antes de codar.
- Reaproveite o `pom.xml` do projeto base (apenas troque `groupId`/`artifactId`).
- Para streams, use `StreamObserver` corretamente:
  - `onNext()` → enviar item
  - `onCompleted()` → encerrar stream
  - `onError()` → reportar falha
- Teste **um RPC por vez** antes de partir para o próximo.
- Para o chat bidirecional, o servidor pode ter um `Map<String, String>` simples mapeando palavras-chave a sugestões de livros.

---

# Material de apoio

- Documentação oficial gRPC Java: https://grpc.io/docs/languages/java/
- Tipos de RPC: https://grpc.io/docs/what-is-grpc/core-concepts/
- Protocol Buffers Guide: https://protobuf.dev/programming-guides/proto3/
- Projeto base **Calc gRPC** (entregue em aula).

---

# Aprendizado esperado

Ao final da atividade, você deverá ser capaz de:

- Modelar contratos de comunicação distribuída usando Protocol Buffers.
- Diferenciar e implementar os quatro tipos de RPC do gRPC.
- Tratar erros de forma idiomática em sistemas distribuídos.
- Compreender o papel da serialização binária em performance de microsserviços.
- Justificar quando usar gRPC em detrimento de REST/HTTP tradicional.

---

#Dúvidas

Entrar em contrato com o professor o mais breve possível
**Não compartilhe código** com outros grupos — apenas conceitos.

---

**Bom trabalho!**