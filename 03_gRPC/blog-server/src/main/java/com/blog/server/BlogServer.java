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

    // =========================================================
    // TODO (Aula 2, com a turma) — montar o servidor:
    //   1. Campos: Server server e MongoDBConfig mongoConfig
    //   2. Construtor BlogServer(int port): pegar o banco com
    //      MongoDBConfig.getInstance(), chamar createIndexes(db), criar os
    //      dois repositórios e montar o servidor com ServerBuilder.forPort(port),
    //      registrando PostServiceImpl e CommentServiceImpl com addService(...)
    //   3. start(): server.start() e um shutdown hook que chama stop()
    //      e depois mongoConfig.close()
    //   4. stop(): server.shutdown().awaitTermination(30, TimeUnit.SECONDS)
    //   5. awaitTermination(): server.awaitTermination()
    //   6. main(String[] args): porta em args[0] (padrão DEFAULT_PORT),
    //      new BlogServer(porta), start() e awaitTermination()
    // =========================================================

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
}
