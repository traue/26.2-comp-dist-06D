package com.blog.server.service;

import com.blog.grpc.generated.*;
import com.blog.server.repository.CommentRepository;
import com.blog.server.repository.PostRepository;
import io.grpc.Status;
import io.grpc.stub.StreamObserver;
import org.bson.Document;

import java.util.List;
import java.util.logging.Logger;

/**
 * Implementação do CommentService gRPC.
 *
 * Demonstra:
 *   1. Validação de integridade referencial em nível de aplicação
 *      (verificar se o post existe antes de permitir comentário)
 *   2. Coordenação entre dois repositórios (PostRepository + CommentRepository)
 *      para manter o contador de comentários no documento Post.
 *   3. Server streaming para GetComments (mesmo padrão do ListPosts).
 */
public class CommentServiceImpl extends CommentServiceGrpc.CommentServiceImplBase {

    private static final Logger logger = Logger.getLogger(CommentServiceImpl.class.getName());

    private final CommentRepository commentRepository;
    private final PostRepository    postRepository;   // necessário para validar post e atualizar contador

    public CommentServiceImpl(CommentRepository commentRepository,
                              PostRepository postRepository) {
        this.commentRepository = commentRepository;
        this.postRepository    = postRepository;
    }

    // =========================================================
    // RPC UNÁRIO: AddComment
    //
    // Antes de persistir o comentário, verifica se o post pai existe.
    // Após persistir, incrementa o contador de comentários no post pai.
    // Este padrão (contador desnormalizado) evita um COUNT(*) custoso
    // a cada leitura de post — técnica comum em bancos NoSQL.
    // =========================================================

    @Override
    public void addComment(AddCommentRequest request,
                           StreamObserver<Comment> responseObserver) {

        logger.info("RPC addComment. Post: " + request.getPostId()
                + " — Autor: " + request.getAuthor());

        // Validação dos campos obrigatórios
        if (request.getPostId().isBlank() || request.getAuthor().isBlank()
                || request.getContent().isBlank()) {
            responseObserver.onError(
                    Status.INVALID_ARGUMENT
                            .withDescription("postId, autor e conteúdo são obrigatórios.")
                            .asRuntimeException()
            );
            return;
        }

        // Integridade referencial: o post deve existir antes de comentar
        // (MongoDB não faz isso automaticamente — responsabilidade da aplicação)
        var postExiste = postRepository.findById(request.getPostId());
        if (postExiste.isEmpty()) {
            responseObserver.onError(
                    Status.NOT_FOUND
                            .withDescription("Post não encontrado. ID: " + request.getPostId())
                            .asRuntimeException()
            );
            return;
        }

        try {
            // Persiste o comentário
            Comment comment = commentRepository.create(
                    request.getPostId(),
                    request.getAuthor(),
                    request.getContent()
            );

            // Mantém o contador desnormalizado no documento do post pai
            postRepository.incrementCommentCount(request.getPostId());

            responseObserver.onNext(comment);
            responseObserver.onCompleted();

        } catch (Exception e) {
            logger.severe("Erro ao adicionar comentário: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL.withCause(e).asRuntimeException()
            );
        }
    }

    // =========================================================
    // RPC UNÁRIO: DeleteComment
    //
    // Remove o comentário e decrementa o contador no post pai.
    // Precisamos buscar o comentário primeiro para saber o postId.
    // =========================================================

    @Override
    public void deleteComment(DeleteCommentRequest request,
                              StreamObserver<DeleteResponse> responseObserver) {

        logger.info("RPC deleteComment. ID: " + request.getId());

        try {
            // Busca o comentário para obter o postId antes de deletar
            Document commentDoc = commentRepository.findRawById(request.getId());

            if (commentDoc == null) {
                responseObserver.onNext(DeleteResponse.newBuilder()
                        .setSuccess(false)
                        .setMessage("Comentário não encontrado. ID: " + request.getId())
                        .build());
                responseObserver.onCompleted();
                return;
            }

            // Extrai o postId do documento antes de deletar
            String postId = commentDoc.getString("postId");

            // Remove o comentário
            boolean deleted = commentRepository.delete(request.getId());

            if (deleted) {
                // Decrementa o contador no post pai (mantém consistência)
                postRepository.decrementCommentCount(postId);
            }

            responseObserver.onNext(DeleteResponse.newBuilder()
                    .setSuccess(deleted)
                    .setMessage(deleted ? "Comentário removido." : "Falha ao remover.")
                    .build());
            responseObserver.onCompleted();

        } catch (Exception e) {
            logger.severe("Erro ao deletar comentário: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL.withCause(e).asRuntimeException()
            );
        }
    }

    // =========================================================
    // RPC COM STREAMING DO SERVIDOR: GetComments
    //
    // Retorna todos os comentários de um post como um stream.
    // Cada Comment é enviado individualmente com onNext().
    //
    // Por que streaming aqui?
    //   Um post popular pode ter centenas de comentários.
    //   Com streaming, o cliente processa cada comentário imediatamente
    //   sem precisar esperar o servidor construir uma lista completa,
    //   e a memória usada é proporcional a 1 comentário, não a todos.
    // =========================================================

    @Override
    public void getComments(GetCommentsRequest request,
                            StreamObserver<Comment> responseObserver) {

        logger.info("RPC getComments (streaming). Post: " + request.getPostId());

        if (request.getPostId().isBlank()) {
            responseObserver.onError(
                    Status.INVALID_ARGUMENT
                            .withDescription("postId é obrigatório.")
                            .asRuntimeException()
            );
            return;
        }

        try {
            List<Comment> comments = commentRepository.findByPostId(request.getPostId());

            // Streaming: envia um Comment por vez
            for (Comment comment : comments) {
                responseObserver.onNext(comment);
            }

            responseObserver.onCompleted();
            logger.info("Stream getComments concluído. Comentários: " + comments.size());

        } catch (Exception e) {
            logger.severe("Erro ao buscar comentários: " + e.getMessage());
            responseObserver.onError(
                    Status.INTERNAL.withCause(e).asRuntimeException()
            );
        }
    }
}
