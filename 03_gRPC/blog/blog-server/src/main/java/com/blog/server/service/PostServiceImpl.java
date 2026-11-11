package com.blog.server.service;

import com.blog.grpc.generated.*;
import com.blog.server.repository.PostRepository;
import io.grpc.Status;
import io.grpc.stub.StreamObserver;

import java.util.List;
import java.util.Optional;
import java.util.logging.Logger;

/**
 * Implementação do PostService gRPC.
 *
 * Esta classe estende o stub gerado automaticamente pelo compilador
 * do Protobuf (PostServiceGrpc.PostServiceImplBase) e sobrescreve
 * cada método RPC definido no arquivo blog.proto.
 *
 * Conceito central — StreamObserver<T>:
 *   É a interface do gRPC para comunicação assíncrona com o cliente.
 *   Para todo RPC, o servidor deve chamar:
 *     - responseObserver.onNext(value)     → envia uma resposta ao cliente
 *     - responseObserver.onCompleted()     → sinaliza fim do stream
 *     - responseObserver.onError(throwable)→ sinaliza erro (encerra o stream)
 *
 * Status gRPC (análogo ao HTTP status code):
 *   - Status.NOT_FOUND       → recurso não existe (404)
 *   - Status.INVALID_ARGUMENT → dados inválidos na requisição (400)
 *   - Status.INTERNAL         → erro interno do servidor (500)
 *   - Status.OK               → sucesso (implícito no onCompleted)
 */
public class PostServiceImpl extends PostServiceGrpc.PostServiceImplBase {

    private static final Logger logger = Logger.getLogger(PostServiceImpl.class.getName());

    private final PostRepository postRepository;

    public PostServiceImpl(PostRepository postRepository) {
        this.postRepository = postRepository;
    }

    // =========================================================
    // RPC UNÁRIO: CreatePost
    // Fluxo: Cliente → [CreatePostRequest] → Servidor → [Post] → Cliente
    // =========================================================

    @Override
    public void createPost(CreatePostRequest request,
                           StreamObserver<Post> responseObserver) {

        logger.info("RPC createPost chamado. Autor: " + request.getAuthor());

        // Validação de entrada (Status.INVALID_ARGUMENT = HTTP 400)
        if (request.getTitle().isBlank() || request.getContent().isBlank()
                || request.getAuthor().isBlank()) {
            responseObserver.onError(
                    Status.INVALID_ARGUMENT
                            .withDescription("Título, conteúdo e autor são obrigatórios.")
                            .asRuntimeException()
            );
            return;
        }

        try {
            Post post = postRepository.create(
                    request.getTitle(),
                    request.getContent(),
                    request.getAuthor()
            );

            // Envia a resposta única (RPC unário = exatamente 1 onNext + onCompleted)
            responseObserver.onNext(post);
            responseObserver.onCompleted();

        } catch (Exception e) {
            logger.severe("Erro ao criar post: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL
                            .withDescription("Erro interno ao criar post.")
                            .withCause(e)
                            .asRuntimeException()
            );
        }
    }

    // =========================================================
    // RPC UNÁRIO: GetPost
    // =========================================================

    @Override
    public void getPost(GetPostRequest request,
                        StreamObserver<Post> responseObserver) {

        logger.info("RPC getPost chamado. ID: " + request.getId());

        if (request.getId().isBlank()) {
            responseObserver.onError(
                    Status.INVALID_ARGUMENT
                            .withDescription("ID do post é obrigatório.")
                            .asRuntimeException()
            );
            return;
        }

        try {
            Optional<Post> post = postRepository.findById(request.getId());

            if (post.isPresent()) {
                responseObserver.onNext(post.get());
                responseObserver.onCompleted();
            } else {
                // Status.NOT_FOUND encerra o RPC com erro — cliente recebe StatusException
                responseObserver.onError(
                        Status.NOT_FOUND
                                .withDescription("Post não encontrado. ID: " + request.getId())
                                .asRuntimeException()
                );
            }

        } catch (Exception e) {
            logger.severe("Erro ao buscar post: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL.withCause(e).asRuntimeException()
            );
        }
    }

    // =========================================================
    // RPC UNÁRIO: UpdatePost
    // =========================================================

    @Override
    public void updatePost(UpdatePostRequest request,
                           StreamObserver<Post> responseObserver) {

        logger.info("RPC updatePost chamado. ID: " + request.getId());

        if (request.getId().isBlank() || request.getTitle().isBlank()
                || request.getContent().isBlank()) {
            responseObserver.onError(
                    Status.INVALID_ARGUMENT
                            .withDescription("ID, título e conteúdo são obrigatórios para atualização.")
                            .asRuntimeException()
            );
            return;
        }

        try {
            Optional<Post> updated = postRepository.update(
                    request.getId(),
                    request.getTitle(),
                    request.getContent()
            );

            if (updated.isPresent()) {
                responseObserver.onNext(updated.get());
                responseObserver.onCompleted();
            } else {
                responseObserver.onError(
                        Status.NOT_FOUND
                                .withDescription("Post não encontrado para atualização. ID: " + request.getId())
                                .asRuntimeException()
                );
            }

        } catch (Exception e) {
            logger.severe("Erro ao atualizar post: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL.withCause(e).asRuntimeException()
            );
        }
    }

    // =========================================================
    // RPC UNÁRIO: DeletePost
    // =========================================================

    @Override
    public void deletePost(DeletePostRequest request,
                           StreamObserver<DeleteResponse> responseObserver) {

        logger.info("RPC deletePost chamado. ID: " + request.getId());

        try {
            boolean deleted = postRepository.delete(request.getId());

            DeleteResponse response = DeleteResponse.newBuilder()
                    .setSuccess(deleted)
                    .setMessage(deleted
                            ? "Post removido com sucesso."
                            : "Post não encontrado. ID: " + request.getId())
                    .build();

            responseObserver.onNext(response);
            responseObserver.onCompleted();

        } catch (Exception e) {
            logger.severe("Erro ao deletar post: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL.withCause(e).asRuntimeException()
            );
        }
    }

    // =========================================================
    // RPC COM STREAMING DO SERVIDOR: ListPosts
    //
    // Diferença fundamental em relação ao RPC unário:
    //   - Chamamos onNext() MÚLTIPLAS vezes — uma por post
    //   - O cliente recebe cada Post assim que o servidor chama onNext()
    //   - onCompleted() só é chamado depois de enviar todos os posts
    //
    // Vantagem: o cliente pode começar a processar o primeiro post
    // antes do servidor terminar de buscar o restante no banco.
    //
    // Fluxo:
    //   Cliente → [ListPostsRequest] → Servidor
    //   Servidor → [Post1] → Cliente
    //   Servidor → [Post2] → Cliente
    //   ...
    //   Servidor → [PostN] → Cliente
    //   Servidor → onCompleted() → Cliente (stream encerrado)
    // =========================================================

    @Override
    public void listPosts(ListPostsRequest request,
                          StreamObserver<Post> responseObserver) {

        int page     = Math.max(0, request.getPage());         // página mínima = 0
        int pageSize = request.getPageSize() > 0
                ? request.getPageSize() : 10;                  // padrão = 10

        logger.info("RPC listPosts (streaming). Página: " + page
                + ", Tamanho: " + pageSize);

        try {
            List<Post> posts = postRepository.findAll(page, pageSize);

            // Envia cada post individualmente pelo stream
            for (Post post : posts) {
                responseObserver.onNext(post);
                // Nota: em produção, verifique responseObserver.isCancelled()
                // para parar cedo se o cliente cancelar a stream
            }

            // Sinaliza que não há mais posts — cliente encerra a leitura
            responseObserver.onCompleted();

            logger.info("Stream listPosts concluído. Posts enviados: " + posts.size());

        } catch (Exception e) {
            logger.severe("Erro ao listar posts: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL.withCause(e).asRuntimeException()
            );
        }
    }
}
