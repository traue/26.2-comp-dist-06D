package com.blog.server;

import com.blog.server.config.MongoDBConfig;
import com.blog.server.repository.CommentRepository;
import com.blog.server.repository.PostRepository;
import com.blog.server.service.CommentServiceImpl;
import com.blog.server.service.PostServiceImpl;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.model.IndexOptions;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import org.bson.Document;

import java.io.IOException;
import java.util.concurrent.TimeUnit;
import java.util.logging.Logger;

/**
 * Ponto de entrada do servidor gRPC do Mini Blog.
 *
 * ┌─────────────────────────────────────────────────────┐
 * │              Arquitetura do Servidor                 │
 * │                                                     │
 * │  [Cliente gRPC]                                     │
 * │       ↕  HTTP/2 + Protocol Buffers                  │
 * │  [gRPC Server : porta 9090]                         │
 * │       ├── PostServiceImpl    ──→ PostRepository     │
 * │       └── CommentServiceImpl ──→ CommentRepository  │
 * │                                      ↕              │
 * │                               [MongoDB : 27017]     │
 * └─────────────────────────────────────────────────────┘
 *
 * Características demonstradas neste projeto:
 *  - RPC Unário (CreatePost, GetPost, UpdatePost, DeletePost, AddComment...)
 *  - Server Streaming (ListPosts, GetComments)
 *  - Serialização binária eficiente com Protocol Buffers
 *  - Persistência NoSQL com MongoDB
 *  - Encerramento gracioso (Graceful Shutdown)
 */
public class BlogServer {

    private static final Logger logger = Logger.getLogger(BlogServer.class.getName());

    // Porta padrão — pode ser sobrescrita via argumento de linha de comando
    private static final int DEFAULT_PORT = 9090;

    private final Server         server;
    private final MongoDBConfig  mongoConfig;

    public BlogServer(int port) {
        this.mongoConfig = MongoDBConfig.getInstance();
        MongoDatabase db = mongoConfig.getDatabase();

        // Cria índices no MongoDB para otimizar consultas frequentes
        // Índice em "postId" na coleção "comments" → acelera GetComments
        createIndexes(db);

        // Inicializa repositórios com as coleções do banco
        PostRepository    postRepo    = new PostRepository(db.getCollection("posts"));
        CommentRepository commentRepo = new CommentRepository(db.getCollection("comments"));

        // Constrói o servidor gRPC registrando os dois serviços
        // ServerBuilder configura o transporte HTTP/2 (Netty) na porta especificada
        this.server = ServerBuilder.forPort(port)
                .addService(new PostServiceImpl(postRepo))
                .addService(new CommentServiceImpl(commentRepo, postRepo))
                .build();
    }

    /** Inicia o servidor e registra hook de encerramento. */
    public void start() throws IOException {
        server.start();

        logger.info("╔══════════════════════════════════════════╗");
        logger.info("║   Blog gRPC Server — porta: " + server.getPort() + "         ║");
        logger.info("║   MongoDB: conectado                     ║");
        logger.info("║   Serviços: PostService, CommentService  ║");
        logger.info("╚══════════════════════════════════════════╝");

        // Graceful Shutdown: ao receber SIGTERM (Ctrl+C ou docker stop),
        // aguarda requisições em andamento terminarem antes de encerrar.
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            logger.info("Sinal de encerramento recebido. Aguardando requisições...");
            try {
                stop();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                logger.warning("Encerramento interrompido.");
            }
            mongoConfig.close();
            logger.info("Servidor encerrado com sucesso.");
        }, "shutdown-hook"));
    }

    /** Para o servidor aguardando até 30 segundos para limpar. */
    public void stop() throws InterruptedException {
        if (server != null) {
            // shutdown() pede encerramento; awaitTermination() aguarda
            server.shutdown().awaitTermination(30, TimeUnit.SECONDS);
        }
    }

    /**
     * Bloqueia a thread principal até o servidor ser encerrado.
     * Sem isso, o processo Java terminaria imediatamente após start().
     */
    public void awaitTermination() throws InterruptedException {
        if (server != null) {
            server.awaitTermination();
        }
    }

    /**
     * Cria índices no MongoDB para otimizar as consultas mais frequentes.
     *
     * Em produção, índices são criados em migrations separadas.
     * Aqui os criamos no startup por simplicidade acadêmica.
     * createIndex() é idempotente — seguro chamar múltiplas vezes.
     */
    private void createIndexes(MongoDatabase db) {
        // Índice em comments.postId → acelera busca de comentários por post
        db.getCollection("comments").createIndex(
                new Document("postId", 1),                      // 1 = crescente
                new IndexOptions().name("idx_comments_postId")
        );

        // Índice em posts.createdAt → acelera ordenação na listagem
        db.getCollection("posts").createIndex(
                new Document("createdAt", -1),                  // -1 = decrescente
                new IndexOptions().name("idx_posts_createdAt")
        );

        logger.info("Índices MongoDB criados/verificados.");
    }

    // =========================================================
    // MAIN
    // =========================================================

    public static void main(String[] args) throws IOException, InterruptedException {
        // Permite configurar a porta via argumento: ./gradlew :blog-server:run --args="8080"
        int port = DEFAULT_PORT;
        if (args.length > 0) {
            try {
                port = Integer.parseInt(args[0]);
            } catch (NumberFormatException e) {
                logger.warning("Porta inválida '" + args[0] + "'. Usando padrão: " + DEFAULT_PORT);
            }
        }

        BlogServer blogServer = new BlogServer(port);
        blogServer.start();
        blogServer.awaitTermination(); // bloqueia aqui até Ctrl+C
    }
}
