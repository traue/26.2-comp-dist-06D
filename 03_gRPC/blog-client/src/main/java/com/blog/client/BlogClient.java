package com.blog.client;

import com.blog.grpc.generated.*;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.StatusRuntimeException;

import java.util.Iterator;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;
import java.util.logging.Logger;

/**
 * Cliente gRPC interativo para o Mini Blog.
 *
 * Este cliente demonstra como um processo externo se comunica com o
 * servidor gRPC usando o mesmo contrato definido em blog.proto.
 *
 * Tipos de Stub utilizados:
 * ┌─────────────────────────────────────────────────────────────────┐
 * │ BlockingStub (síncrono):                                        │
 * │   - Bloqueia a thread até receber a resposta                    │
 * │   - Ideal para CLI e scripts simples                            │
 * │   - Para streaming: retorna Iterator<T> (leitura em loop)       │
 * │                                                                 │
 * │ AsyncStub (assíncrono):                                         │
 * │   - Não bloqueia; usa callbacks (StreamObserver)                │
 * │   - Ideal para servidores e UIs reativas                        │
 * │                                                                 │
 * │ FutureStub:                                                     │
 * │   - Retorna ListenableFuture (apenas para RPCs unários)         │
 * └─────────────────────────────────────────────────────────────────┘
 *
 * Canal gRPC (ManagedChannel):
 *   - Representa a conexão com o servidor via HTTP/2
 *   - Gerencia reconexão, load balancing e TLS internamente
 *   - Deve ser fechado ao final (close() libera recursos)
 */
public class BlogClient {

    private static final Logger logger = Logger.getLogger(BlogClient.class.getName());

    // Stubs — gerados a partir do blog.proto pelo protoc-gen-grpc-java
    private final PostServiceGrpc.PostServiceBlockingStub       postStub;
    private final CommentServiceGrpc.CommentServiceBlockingStub commentStub;

    private final ManagedChannel channel;
    private final Scanner        scanner;

    /**
     * Inicializa o canal gRPC e os stubs de cliente.
     *
     * usePlaintext() desativa TLS — adequado para desenvolvimento local.
     * Em produção, use useTransportSecurity() com certificados.
     */
    public BlogClient(String host, int port) {
        // ManagedChannel: conexão HTTP/2 reutilizável com o servidor
        this.channel = ManagedChannelBuilder
                .forAddress(host, port)
                .usePlaintext()              // sem TLS (apenas desenvolvimento)
                .build();

        // Stubs criam as chamadas RPC usando o canal
        this.postStub    = PostServiceGrpc.newBlockingStub(channel);
        this.commentStub = CommentServiceGrpc.newBlockingStub(channel);
        this.scanner     = new Scanner(System.in);

        System.out.println("✓ Conectado ao servidor gRPC em " + host + ":" + port);
    }

    /** Fecha o canal ao encerrar — libera conexões HTTP/2. */
    public void close() throws InterruptedException {
        channel.shutdown().awaitTermination(5, TimeUnit.SECONDS);
    }

    // =========================================================
    // MENU PRINCIPAL
    // =========================================================

    public void iniciar() {
        while (true) {
            printMenu();
            String opcao = scanner.nextLine().trim();

            switch (opcao) {
                case "1"  -> criarPost();
                case "2"  -> buscarPost();
                case "3"  -> listarPosts();
                case "4"  -> atualizarPost();
                case "5"  -> deletarPost();
                case "6"  -> adicionarComentario();
                case "7"  -> listarComentarios();
                case "8"  -> deletarComentario();
                case "0"  -> { System.out.println("Encerrando..."); return; }
                default   -> System.out.println("Opção inválida. Tente novamente.");
            }
        }
    }

    private void printMenu() {
        System.out.println("\n╔══════════════════════════════════════╗");
        System.out.println("║       BLOG — Cliente gRPC            ║");
        System.out.println("╠══════════════════════════════════════╣");
        System.out.println("║  Posts                               ║");
        System.out.println("║  1. Criar post                       ║");
        System.out.println("║  2. Buscar post por ID               ║");
        System.out.println("║  3. Listar posts (streaming)         ║");
        System.out.println("║  4. Atualizar post                   ║");
        System.out.println("║  5. Deletar post                     ║");
        System.out.println("╠══════════════════════════════════════╣");
        System.out.println("║  Comentários                         ║");
        System.out.println("║  6. Adicionar comentário             ║");
        System.out.println("║  7. Listar comentários (streaming)   ║");
        System.out.println("║  8. Deletar comentário               ║");
        System.out.println("╠══════════════════════════════════════╣");
        System.out.println("║  0. Sair                             ║");
        System.out.println("╚══════════════════════════════════════╝");
        System.out.print("Escolha: ");
    }

    // =========================================================
    // OPERAÇÕES DE POSTS
    // =========================================================

    /** RPC Unário: cria um novo post. */
    private void criarPost() {
        System.out.println("\n--- Criar Post ---");
        System.out.print("Título: ");
        String titulo = scanner.nextLine().trim();
        System.out.print("Autor: ");
        String autor = scanner.nextLine().trim();
        System.out.print("Conteúdo (uma linha): ");
        String conteudo = scanner.nextLine().trim();

        // Constrói a mensagem Protobuf de requisição
        CreatePostRequest request = CreatePostRequest.newBuilder()
                .setTitle(titulo)
                .setAuthor(autor)
                .setContent(conteudo)
                .build();

        try {
            // Chamada RPC — bloqueia até o servidor responder
            Post post = postStub.createPost(request);
            System.out.println("\n✓ Post criado com sucesso!");
            printPost(post);
        } catch (StatusRuntimeException e) {
            // StatusRuntimeException carrega o código de erro gRPC e a mensagem
            System.out.println("✗ Erro: " + e.getStatus().getDescription());
        }
    }

    /** RPC Unário: busca post por ID. */
    private void buscarPost() {
        System.out.println("\n--- Buscar Post ---");
        System.out.print("ID do post: ");
        String id = scanner.nextLine().trim();

        try {
            Post post = postStub.getPost(
                    GetPostRequest.newBuilder().setId(id).build()
            );
            printPost(post);
        } catch (StatusRuntimeException e) {
            System.out.println("✗ " + e.getStatus().getCode() + ": "
                    + e.getStatus().getDescription());
        }
    }

    /**
     * RPC com Streaming do Servidor: lista posts paginados.
     *
     * Com BlockingStub, o streaming retorna um Iterator<Post>.
     * Cada chamada a iterator.next() recebe o próximo Post enviado
     * pelo servidor — o cliente NÃO espera todos chegarem para começar.
     *
     * Compare com HTTP tradicional: você receberia uma lista completa
     * de uma vez. Com streaming, cada item chega individualmente.
     */
    private void listarPosts() {
        System.out.println("\n--- Listar Posts (Server Streaming) ---");
        System.out.print("Página (0 = primeira): ");
        int pagina = lerInt(0);
        System.out.print("Posts por página [10]: ");
        String tamanhoStr = scanner.nextLine().trim();
        int tamanho = tamanhoStr.isEmpty() ? 10 : Integer.parseInt(tamanhoStr);

        ListPostsRequest request = ListPostsRequest.newBuilder()
                .setPage(pagina)
                .setPageSize(tamanho)
                .build();

        try {
            // Iterator recebe posts um a um conforme o servidor os envia
            Iterator<Post> stream = postStub.listPosts(request);

            System.out.println("\n=== Posts (recebidos via streaming) ===");
            int count = 0;

            while (stream.hasNext()) {      // hasNext() espera pelo próximo item do stream
                Post post = stream.next();
                count++;
                System.out.printf("%d. [%s] %s — %s (%d comentários)%n",
                        count, post.getId(), post.getTitle(),
                        post.getAuthor(), post.getCommentCount());
            }

            System.out.println("Total recebido: " + count + " posts.");

        } catch (StatusRuntimeException e) {
            System.out.println("✗ Erro: " + e.getStatus().getDescription());
        }
    }

    /** RPC Unário: atualiza título e conteúdo de um post. */
    private void atualizarPost() {
        System.out.println("\n--- Atualizar Post ---");
        System.out.print("ID do post: ");
        String id = scanner.nextLine().trim();
        System.out.print("Novo título: ");
        String titulo = scanner.nextLine().trim();
        System.out.print("Novo conteúdo: ");
        String conteudo = scanner.nextLine().trim();

        UpdatePostRequest request = UpdatePostRequest.newBuilder()
                .setId(id)
                .setTitle(titulo)
                .setContent(conteudo)
                .build();

        try {
            Post post = postStub.updatePost(request);
            System.out.println("✓ Post atualizado!");
            printPost(post);
        } catch (StatusRuntimeException e) {
            System.out.println("✗ " + e.getStatus().getCode() + ": "
                    + e.getStatus().getDescription());
        }
    }

    /** RPC Unário: remove um post. */
    private void deletarPost() {
        System.out.println("\n--- Deletar Post ---");
        System.out.print("ID do post: ");
        String id = scanner.nextLine().trim();
        System.out.print("Confirmar deleção? (s/n): ");
        if (!scanner.nextLine().trim().equalsIgnoreCase("s")) {
            System.out.println("Operação cancelada.");
            return;
        }

        try {
            DeleteResponse resp = postStub.deletePost(
                    DeletePostRequest.newBuilder().setId(id).build()
            );
            System.out.println(resp.getSuccess() ? "✓ " : "✗ " + resp.getMessage());
        } catch (StatusRuntimeException e) {
            System.out.println("✗ Erro: " + e.getStatus().getDescription());
        }
    }

    // =========================================================
    // OPERAÇÕES DE COMENTÁRIOS
    // =========================================================

    /** RPC Unário: adiciona comentário a um post. */
    private void adicionarComentario() {
        System.out.println("\n--- Adicionar Comentário ---");
        System.out.print("ID do post: ");
        String postId = scanner.nextLine().trim();
        System.out.print("Seu nome: ");
        String autor = scanner.nextLine().trim();
        System.out.print("Comentário: ");
        String conteudo = scanner.nextLine().trim();

        AddCommentRequest request = AddCommentRequest.newBuilder()
                .setPostId(postId)
                .setAuthor(autor)
                .setContent(conteudo)
                .build();

        try {
            Comment comment = commentStub.addComment(request);
            System.out.println("✓ Comentário adicionado! ID: " + comment.getId());
        } catch (StatusRuntimeException e) {
            System.out.println("✗ " + e.getStatus().getCode() + ": "
                    + e.getStatus().getDescription());
        }
    }

    /**
     * RPC com Streaming do Servidor: lista comentários de um post.
     * Demonstra o mesmo padrão de streaming que ListPosts.
     */
    private void listarComentarios() {
        System.out.println("\n--- Listar Comentários (Server Streaming) ---");
        System.out.print("ID do post: ");
        String postId = scanner.nextLine().trim();

        GetCommentsRequest request = GetCommentsRequest.newBuilder()
                .setPostId(postId)
                .build();

        try {
            Iterator<Comment> stream = commentStub.getComments(request);

            System.out.println("\n=== Comentários (via streaming) ===");
            int count = 0;

            while (stream.hasNext()) {
                Comment c = stream.next();
                count++;
                System.out.printf("%d. [%s] %s: \"%s\"%n",
                        count, c.getCreatedAt(), c.getAuthor(), c.getContent());
            }

            if (count == 0) System.out.println("Nenhum comentário encontrado.");
            else System.out.println("Total: " + count + " comentários.");

        } catch (StatusRuntimeException e) {
            System.out.println("✗ Erro: " + e.getStatus().getDescription());
        }
    }

    /** RPC Unário: remove um comentário. */
    private void deletarComentario() {
        System.out.println("\n--- Deletar Comentário ---");
        System.out.print("ID do comentário: ");
        String id = scanner.nextLine().trim();

        try {
            DeleteResponse resp = commentStub.deleteComment(
                    DeleteCommentRequest.newBuilder().setId(id).build()
            );
            System.out.println(resp.getSuccess() ? "✓ " : "✗ " + resp.getMessage());
        } catch (StatusRuntimeException e) {
            System.out.println("✗ Erro: " + e.getStatus().getDescription());
        }
    }

    // =========================================================
    // HELPERS
    // =========================================================

    private void printPost(Post post) {
        System.out.println("┌─────────────────────────────────────────");
        System.out.println("│ ID      : " + post.getId());
        System.out.println("│ Título  : " + post.getTitle());
        System.out.println("│ Autor   : " + post.getAuthor());
        System.out.println("│ Criado  : " + post.getCreatedAt());
        System.out.println("│ Atualiz : " + post.getUpdatedAt());
        System.out.println("│ Coments : " + post.getCommentCount());
        System.out.println("│ Conteúdo:");
        System.out.println("│   " + post.getContent());
        System.out.println("└─────────────────────────────────────────");
    }

    private int lerInt(int defaultValue) {
        String linha = scanner.nextLine().trim();
        if (linha.isEmpty()) return defaultValue;
        try { return Integer.parseInt(linha); }
        catch (NumberFormatException e) { return defaultValue; }
    }

    // =========================================================
    // MAIN
    // =========================================================

    public static void main(String[] args) throws InterruptedException {
        // Host e porta do servidor (padrão: localhost:9090)
        String host = System.getenv().getOrDefault("GRPC_HOST", "localhost");
        int port    = Integer.parseInt(System.getenv().getOrDefault("GRPC_PORT", "9090"));

        System.out.println("Conectando ao servidor gRPC: " + host + ":" + port);

        BlogClient client = new BlogClient(host, port);
        try {
            client.iniciar();
        } finally {
            client.close(); // sempre fecha o canal ao sair
        }
    }
}
