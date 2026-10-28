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
    // TODO (Aula 2) — implementar os RPCs do PostService.
    //
    // Enquanto um método não for sobrescrito aqui, o gRPC responde
    // UNIMPLEMENTED ao cliente (o projeto compila e roda mesmo assim).
    // Todo método precisa terminar com onCompleted() ou onError();
    // senão o cliente fica esperando para sempre.
    //
    // Com a turma:
    //   @Override public void createPost(CreatePostRequest request, StreamObserver<Post> responseObserver)
    //   @Override public void getPost(GetPostRequest request, StreamObserver<Post> responseObserver)
    //   @Override public void listPosts(ListPostsRequest request, StreamObserver<Post> responseObserver)
    //
    // Sozinhos, no mesmo padrão:
    //   @Override public void updatePost(UpdatePostRequest request, StreamObserver<Post> responseObserver)
    //   @Override public void deletePost(DeletePostRequest request, StreamObserver<DeleteResponse> responseObserver)
    // =========================================================
}
