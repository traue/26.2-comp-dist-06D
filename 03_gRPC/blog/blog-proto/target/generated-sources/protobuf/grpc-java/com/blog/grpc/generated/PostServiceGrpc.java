package com.blog.grpc.generated;

import static io.grpc.MethodDescriptor.generateFullMethodName;

/**
 * <pre>
 * Serviço de gerenciamento de postagens
 * </pre>
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.65.0)",
    comments = "Source: blog.proto")
@io.grpc.stub.annotations.GrpcGenerated
public final class PostServiceGrpc {

  private PostServiceGrpc() {}

  public static final java.lang.String SERVICE_NAME = "blog.PostService";

  // Static method descriptors that strictly reflect the proto.
  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.CreatePostRequest,
      com.blog.grpc.generated.Post> getCreatePostMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "CreatePost",
      requestType = com.blog.grpc.generated.CreatePostRequest.class,
      responseType = com.blog.grpc.generated.Post.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.CreatePostRequest,
      com.blog.grpc.generated.Post> getCreatePostMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.CreatePostRequest, com.blog.grpc.generated.Post> getCreatePostMethod;
    if ((getCreatePostMethod = PostServiceGrpc.getCreatePostMethod) == null) {
      synchronized (PostServiceGrpc.class) {
        if ((getCreatePostMethod = PostServiceGrpc.getCreatePostMethod) == null) {
          PostServiceGrpc.getCreatePostMethod = getCreatePostMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.CreatePostRequest, com.blog.grpc.generated.Post>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "CreatePost"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.CreatePostRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.Post.getDefaultInstance()))
              .setSchemaDescriptor(new PostServiceMethodDescriptorSupplier("CreatePost"))
              .build();
        }
      }
    }
    return getCreatePostMethod;
  }

  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.GetPostRequest,
      com.blog.grpc.generated.Post> getGetPostMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "GetPost",
      requestType = com.blog.grpc.generated.GetPostRequest.class,
      responseType = com.blog.grpc.generated.Post.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.GetPostRequest,
      com.blog.grpc.generated.Post> getGetPostMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.GetPostRequest, com.blog.grpc.generated.Post> getGetPostMethod;
    if ((getGetPostMethod = PostServiceGrpc.getGetPostMethod) == null) {
      synchronized (PostServiceGrpc.class) {
        if ((getGetPostMethod = PostServiceGrpc.getGetPostMethod) == null) {
          PostServiceGrpc.getGetPostMethod = getGetPostMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.GetPostRequest, com.blog.grpc.generated.Post>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "GetPost"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.GetPostRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.Post.getDefaultInstance()))
              .setSchemaDescriptor(new PostServiceMethodDescriptorSupplier("GetPost"))
              .build();
        }
      }
    }
    return getGetPostMethod;
  }

  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.UpdatePostRequest,
      com.blog.grpc.generated.Post> getUpdatePostMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "UpdatePost",
      requestType = com.blog.grpc.generated.UpdatePostRequest.class,
      responseType = com.blog.grpc.generated.Post.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.UpdatePostRequest,
      com.blog.grpc.generated.Post> getUpdatePostMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.UpdatePostRequest, com.blog.grpc.generated.Post> getUpdatePostMethod;
    if ((getUpdatePostMethod = PostServiceGrpc.getUpdatePostMethod) == null) {
      synchronized (PostServiceGrpc.class) {
        if ((getUpdatePostMethod = PostServiceGrpc.getUpdatePostMethod) == null) {
          PostServiceGrpc.getUpdatePostMethod = getUpdatePostMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.UpdatePostRequest, com.blog.grpc.generated.Post>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "UpdatePost"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.UpdatePostRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.Post.getDefaultInstance()))
              .setSchemaDescriptor(new PostServiceMethodDescriptorSupplier("UpdatePost"))
              .build();
        }
      }
    }
    return getUpdatePostMethod;
  }

  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.DeletePostRequest,
      com.blog.grpc.generated.DeleteResponse> getDeletePostMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "DeletePost",
      requestType = com.blog.grpc.generated.DeletePostRequest.class,
      responseType = com.blog.grpc.generated.DeleteResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.DeletePostRequest,
      com.blog.grpc.generated.DeleteResponse> getDeletePostMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.DeletePostRequest, com.blog.grpc.generated.DeleteResponse> getDeletePostMethod;
    if ((getDeletePostMethod = PostServiceGrpc.getDeletePostMethod) == null) {
      synchronized (PostServiceGrpc.class) {
        if ((getDeletePostMethod = PostServiceGrpc.getDeletePostMethod) == null) {
          PostServiceGrpc.getDeletePostMethod = getDeletePostMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.DeletePostRequest, com.blog.grpc.generated.DeleteResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "DeletePost"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.DeletePostRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.DeleteResponse.getDefaultInstance()))
              .setSchemaDescriptor(new PostServiceMethodDescriptorSupplier("DeletePost"))
              .build();
        }
      }
    }
    return getDeletePostMethod;
  }

  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.ListPostsRequest,
      com.blog.grpc.generated.Post> getListPostsMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "ListPosts",
      requestType = com.blog.grpc.generated.ListPostsRequest.class,
      responseType = com.blog.grpc.generated.Post.class,
      methodType = io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.ListPostsRequest,
      com.blog.grpc.generated.Post> getListPostsMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.ListPostsRequest, com.blog.grpc.generated.Post> getListPostsMethod;
    if ((getListPostsMethod = PostServiceGrpc.getListPostsMethod) == null) {
      synchronized (PostServiceGrpc.class) {
        if ((getListPostsMethod = PostServiceGrpc.getListPostsMethod) == null) {
          PostServiceGrpc.getListPostsMethod = getListPostsMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.ListPostsRequest, com.blog.grpc.generated.Post>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "ListPosts"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.ListPostsRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.Post.getDefaultInstance()))
              .setSchemaDescriptor(new PostServiceMethodDescriptorSupplier("ListPosts"))
              .build();
        }
      }
    }
    return getListPostsMethod;
  }

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static PostServiceStub newStub(io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<PostServiceStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<PostServiceStub>() {
        @java.lang.Override
        public PostServiceStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new PostServiceStub(channel, callOptions);
        }
      };
    return PostServiceStub.newStub(factory, channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static PostServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<PostServiceBlockingStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<PostServiceBlockingStub>() {
        @java.lang.Override
        public PostServiceBlockingStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new PostServiceBlockingStub(channel, callOptions);
        }
      };
    return PostServiceBlockingStub.newStub(factory, channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary calls on the service
   */
  public static PostServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<PostServiceFutureStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<PostServiceFutureStub>() {
        @java.lang.Override
        public PostServiceFutureStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new PostServiceFutureStub(channel, callOptions);
        }
      };
    return PostServiceFutureStub.newStub(factory, channel);
  }

  /**
   * <pre>
   * Serviço de gerenciamento de postagens
   * </pre>
   */
  public interface AsyncService {

    /**
     * <pre>
     * Cria uma nova postagem
     * </pre>
     */
    default void createPost(com.blog.grpc.generated.CreatePostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getCreatePostMethod(), responseObserver);
    }

    /**
     * <pre>
     * Busca postagem por ID — retorna NOT_FOUND se não existir
     * </pre>
     */
    default void getPost(com.blog.grpc.generated.GetPostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getGetPostMethod(), responseObserver);
    }

    /**
     * <pre>
     * Atualiza título e conteúdo de uma postagem
     * </pre>
     */
    default void updatePost(com.blog.grpc.generated.UpdatePostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getUpdatePostMethod(), responseObserver);
    }

    /**
     * <pre>
     * Remove uma postagem permanentemente
     * </pre>
     */
    default void deletePost(com.blog.grpc.generated.DeletePostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.DeleteResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getDeletePostMethod(), responseObserver);
    }

    /**
     * <pre>
     * ---- RPC COM STREAMING DO SERVIDOR ----
     * O servidor envia posts um a um enquanto o cliente lê com um iterador.
     * Vantagem: o servidor pode começar a enviar antes de buscar tudo no DB,
     * e o cliente processa cada item imediatamente (backpressure natural).
     * </pre>
     */
    default void listPosts(com.blog.grpc.generated.ListPostsRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getListPostsMethod(), responseObserver);
    }
  }

  /**
   * Base class for the server implementation of the service PostService.
   * <pre>
   * Serviço de gerenciamento de postagens
   * </pre>
   */
  public static abstract class PostServiceImplBase
      implements io.grpc.BindableService, AsyncService {

    @java.lang.Override public final io.grpc.ServerServiceDefinition bindService() {
      return PostServiceGrpc.bindService(this);
    }
  }

  /**
   * A stub to allow clients to do asynchronous rpc calls to service PostService.
   * <pre>
   * Serviço de gerenciamento de postagens
   * </pre>
   */
  public static final class PostServiceStub
      extends io.grpc.stub.AbstractAsyncStub<PostServiceStub> {
    private PostServiceStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PostServiceStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new PostServiceStub(channel, callOptions);
    }

    /**
     * <pre>
     * Cria uma nova postagem
     * </pre>
     */
    public void createPost(com.blog.grpc.generated.CreatePostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getCreatePostMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * Busca postagem por ID — retorna NOT_FOUND se não existir
     * </pre>
     */
    public void getPost(com.blog.grpc.generated.GetPostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getGetPostMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * Atualiza título e conteúdo de uma postagem
     * </pre>
     */
    public void updatePost(com.blog.grpc.generated.UpdatePostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getUpdatePostMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * Remove uma postagem permanentemente
     * </pre>
     */
    public void deletePost(com.blog.grpc.generated.DeletePostRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.DeleteResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getDeletePostMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * ---- RPC COM STREAMING DO SERVIDOR ----
     * O servidor envia posts um a um enquanto o cliente lê com um iterador.
     * Vantagem: o servidor pode começar a enviar antes de buscar tudo no DB,
     * e o cliente processa cada item imediatamente (backpressure natural).
     * </pre>
     */
    public void listPosts(com.blog.grpc.generated.ListPostsRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post> responseObserver) {
      io.grpc.stub.ClientCalls.asyncServerStreamingCall(
          getChannel().newCall(getListPostsMethod(), getCallOptions()), request, responseObserver);
    }
  }

  /**
   * A stub to allow clients to do synchronous rpc calls to service PostService.
   * <pre>
   * Serviço de gerenciamento de postagens
   * </pre>
   */
  public static final class PostServiceBlockingStub
      extends io.grpc.stub.AbstractBlockingStub<PostServiceBlockingStub> {
    private PostServiceBlockingStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PostServiceBlockingStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new PostServiceBlockingStub(channel, callOptions);
    }

    /**
     * <pre>
     * Cria uma nova postagem
     * </pre>
     */
    public com.blog.grpc.generated.Post createPost(com.blog.grpc.generated.CreatePostRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getCreatePostMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * Busca postagem por ID — retorna NOT_FOUND se não existir
     * </pre>
     */
    public com.blog.grpc.generated.Post getPost(com.blog.grpc.generated.GetPostRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getGetPostMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * Atualiza título e conteúdo de uma postagem
     * </pre>
     */
    public com.blog.grpc.generated.Post updatePost(com.blog.grpc.generated.UpdatePostRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getUpdatePostMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * Remove uma postagem permanentemente
     * </pre>
     */
    public com.blog.grpc.generated.DeleteResponse deletePost(com.blog.grpc.generated.DeletePostRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getDeletePostMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * ---- RPC COM STREAMING DO SERVIDOR ----
     * O servidor envia posts um a um enquanto o cliente lê com um iterador.
     * Vantagem: o servidor pode começar a enviar antes de buscar tudo no DB,
     * e o cliente processa cada item imediatamente (backpressure natural).
     * </pre>
     */
    public java.util.Iterator<com.blog.grpc.generated.Post> listPosts(
        com.blog.grpc.generated.ListPostsRequest request) {
      return io.grpc.stub.ClientCalls.blockingServerStreamingCall(
          getChannel(), getListPostsMethod(), getCallOptions(), request);
    }
  }

  /**
   * A stub to allow clients to do ListenableFuture-style rpc calls to service PostService.
   * <pre>
   * Serviço de gerenciamento de postagens
   * </pre>
   */
  public static final class PostServiceFutureStub
      extends io.grpc.stub.AbstractFutureStub<PostServiceFutureStub> {
    private PostServiceFutureStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PostServiceFutureStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new PostServiceFutureStub(channel, callOptions);
    }

    /**
     * <pre>
     * Cria uma nova postagem
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<com.blog.grpc.generated.Post> createPost(
        com.blog.grpc.generated.CreatePostRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getCreatePostMethod(), getCallOptions()), request);
    }

    /**
     * <pre>
     * Busca postagem por ID — retorna NOT_FOUND se não existir
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<com.blog.grpc.generated.Post> getPost(
        com.blog.grpc.generated.GetPostRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getGetPostMethod(), getCallOptions()), request);
    }

    /**
     * <pre>
     * Atualiza título e conteúdo de uma postagem
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<com.blog.grpc.generated.Post> updatePost(
        com.blog.grpc.generated.UpdatePostRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getUpdatePostMethod(), getCallOptions()), request);
    }

    /**
     * <pre>
     * Remove uma postagem permanentemente
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<com.blog.grpc.generated.DeleteResponse> deletePost(
        com.blog.grpc.generated.DeletePostRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getDeletePostMethod(), getCallOptions()), request);
    }
  }

  private static final int METHODID_CREATE_POST = 0;
  private static final int METHODID_GET_POST = 1;
  private static final int METHODID_UPDATE_POST = 2;
  private static final int METHODID_DELETE_POST = 3;
  private static final int METHODID_LIST_POSTS = 4;

  private static final class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final AsyncService serviceImpl;
    private final int methodId;

    MethodHandlers(AsyncService serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_CREATE_POST:
          serviceImpl.createPost((com.blog.grpc.generated.CreatePostRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post>) responseObserver);
          break;
        case METHODID_GET_POST:
          serviceImpl.getPost((com.blog.grpc.generated.GetPostRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post>) responseObserver);
          break;
        case METHODID_UPDATE_POST:
          serviceImpl.updatePost((com.blog.grpc.generated.UpdatePostRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post>) responseObserver);
          break;
        case METHODID_DELETE_POST:
          serviceImpl.deletePost((com.blog.grpc.generated.DeletePostRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.DeleteResponse>) responseObserver);
          break;
        case METHODID_LIST_POSTS:
          serviceImpl.listPosts((com.blog.grpc.generated.ListPostsRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.Post>) responseObserver);
          break;
        default:
          throw new AssertionError();
      }
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public io.grpc.stub.StreamObserver<Req> invoke(
        io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        default:
          throw new AssertionError();
      }
    }
  }

  public static final io.grpc.ServerServiceDefinition bindService(AsyncService service) {
    return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
        .addMethod(
          getCreatePostMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              com.blog.grpc.generated.CreatePostRequest,
              com.blog.grpc.generated.Post>(
                service, METHODID_CREATE_POST)))
        .addMethod(
          getGetPostMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              com.blog.grpc.generated.GetPostRequest,
              com.blog.grpc.generated.Post>(
                service, METHODID_GET_POST)))
        .addMethod(
          getUpdatePostMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              com.blog.grpc.generated.UpdatePostRequest,
              com.blog.grpc.generated.Post>(
                service, METHODID_UPDATE_POST)))
        .addMethod(
          getDeletePostMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              com.blog.grpc.generated.DeletePostRequest,
              com.blog.grpc.generated.DeleteResponse>(
                service, METHODID_DELETE_POST)))
        .addMethod(
          getListPostsMethod(),
          io.grpc.stub.ServerCalls.asyncServerStreamingCall(
            new MethodHandlers<
              com.blog.grpc.generated.ListPostsRequest,
              com.blog.grpc.generated.Post>(
                service, METHODID_LIST_POSTS)))
        .build();
  }

  private static abstract class PostServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoFileDescriptorSupplier, io.grpc.protobuf.ProtoServiceDescriptorSupplier {
    PostServiceBaseDescriptorSupplier() {}

    @java.lang.Override
    public com.google.protobuf.Descriptors.FileDescriptor getFileDescriptor() {
      return com.blog.grpc.generated.BlogProto.getDescriptor();
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.ServiceDescriptor getServiceDescriptor() {
      return getFileDescriptor().findServiceByName("PostService");
    }
  }

  private static final class PostServiceFileDescriptorSupplier
      extends PostServiceBaseDescriptorSupplier {
    PostServiceFileDescriptorSupplier() {}
  }

  private static final class PostServiceMethodDescriptorSupplier
      extends PostServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoMethodDescriptorSupplier {
    private final java.lang.String methodName;

    PostServiceMethodDescriptorSupplier(java.lang.String methodName) {
      this.methodName = methodName;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.MethodDescriptor getMethodDescriptor() {
      return getServiceDescriptor().findMethodByName(methodName);
    }
  }

  private static volatile io.grpc.ServiceDescriptor serviceDescriptor;

  public static io.grpc.ServiceDescriptor getServiceDescriptor() {
    io.grpc.ServiceDescriptor result = serviceDescriptor;
    if (result == null) {
      synchronized (PostServiceGrpc.class) {
        result = serviceDescriptor;
        if (result == null) {
          serviceDescriptor = result = io.grpc.ServiceDescriptor.newBuilder(SERVICE_NAME)
              .setSchemaDescriptor(new PostServiceFileDescriptorSupplier())
              .addMethod(getCreatePostMethod())
              .addMethod(getGetPostMethod())
              .addMethod(getUpdatePostMethod())
              .addMethod(getDeletePostMethod())
              .addMethod(getListPostsMethod())
              .build();
        }
      }
    }
    return result;
  }
}
