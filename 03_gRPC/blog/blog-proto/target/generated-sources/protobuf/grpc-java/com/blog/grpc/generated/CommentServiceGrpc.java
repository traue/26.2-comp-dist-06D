package com.blog.grpc.generated;

import static io.grpc.MethodDescriptor.generateFullMethodName;

/**
 * <pre>
 * Serviço de gerenciamento de comentários
 * </pre>
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.65.0)",
    comments = "Source: blog.proto")
@io.grpc.stub.annotations.GrpcGenerated
public final class CommentServiceGrpc {

  private CommentServiceGrpc() {}

  public static final java.lang.String SERVICE_NAME = "blog.CommentService";

  // Static method descriptors that strictly reflect the proto.
  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.AddCommentRequest,
      com.blog.grpc.generated.Comment> getAddCommentMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "AddComment",
      requestType = com.blog.grpc.generated.AddCommentRequest.class,
      responseType = com.blog.grpc.generated.Comment.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.AddCommentRequest,
      com.blog.grpc.generated.Comment> getAddCommentMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.AddCommentRequest, com.blog.grpc.generated.Comment> getAddCommentMethod;
    if ((getAddCommentMethod = CommentServiceGrpc.getAddCommentMethod) == null) {
      synchronized (CommentServiceGrpc.class) {
        if ((getAddCommentMethod = CommentServiceGrpc.getAddCommentMethod) == null) {
          CommentServiceGrpc.getAddCommentMethod = getAddCommentMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.AddCommentRequest, com.blog.grpc.generated.Comment>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "AddComment"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.AddCommentRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.Comment.getDefaultInstance()))
              .setSchemaDescriptor(new CommentServiceMethodDescriptorSupplier("AddComment"))
              .build();
        }
      }
    }
    return getAddCommentMethod;
  }

  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.DeleteCommentRequest,
      com.blog.grpc.generated.DeleteResponse> getDeleteCommentMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "DeleteComment",
      requestType = com.blog.grpc.generated.DeleteCommentRequest.class,
      responseType = com.blog.grpc.generated.DeleteResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.DeleteCommentRequest,
      com.blog.grpc.generated.DeleteResponse> getDeleteCommentMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.DeleteCommentRequest, com.blog.grpc.generated.DeleteResponse> getDeleteCommentMethod;
    if ((getDeleteCommentMethod = CommentServiceGrpc.getDeleteCommentMethod) == null) {
      synchronized (CommentServiceGrpc.class) {
        if ((getDeleteCommentMethod = CommentServiceGrpc.getDeleteCommentMethod) == null) {
          CommentServiceGrpc.getDeleteCommentMethod = getDeleteCommentMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.DeleteCommentRequest, com.blog.grpc.generated.DeleteResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "DeleteComment"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.DeleteCommentRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.DeleteResponse.getDefaultInstance()))
              .setSchemaDescriptor(new CommentServiceMethodDescriptorSupplier("DeleteComment"))
              .build();
        }
      }
    }
    return getDeleteCommentMethod;
  }

  private static volatile io.grpc.MethodDescriptor<com.blog.grpc.generated.GetCommentsRequest,
      com.blog.grpc.generated.Comment> getGetCommentsMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "GetComments",
      requestType = com.blog.grpc.generated.GetCommentsRequest.class,
      responseType = com.blog.grpc.generated.Comment.class,
      methodType = io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
  public static io.grpc.MethodDescriptor<com.blog.grpc.generated.GetCommentsRequest,
      com.blog.grpc.generated.Comment> getGetCommentsMethod() {
    io.grpc.MethodDescriptor<com.blog.grpc.generated.GetCommentsRequest, com.blog.grpc.generated.Comment> getGetCommentsMethod;
    if ((getGetCommentsMethod = CommentServiceGrpc.getGetCommentsMethod) == null) {
      synchronized (CommentServiceGrpc.class) {
        if ((getGetCommentsMethod = CommentServiceGrpc.getGetCommentsMethod) == null) {
          CommentServiceGrpc.getGetCommentsMethod = getGetCommentsMethod =
              io.grpc.MethodDescriptor.<com.blog.grpc.generated.GetCommentsRequest, com.blog.grpc.generated.Comment>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "GetComments"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.GetCommentsRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  com.blog.grpc.generated.Comment.getDefaultInstance()))
              .setSchemaDescriptor(new CommentServiceMethodDescriptorSupplier("GetComments"))
              .build();
        }
      }
    }
    return getGetCommentsMethod;
  }

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static CommentServiceStub newStub(io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<CommentServiceStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<CommentServiceStub>() {
        @java.lang.Override
        public CommentServiceStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new CommentServiceStub(channel, callOptions);
        }
      };
    return CommentServiceStub.newStub(factory, channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static CommentServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<CommentServiceBlockingStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<CommentServiceBlockingStub>() {
        @java.lang.Override
        public CommentServiceBlockingStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new CommentServiceBlockingStub(channel, callOptions);
        }
      };
    return CommentServiceBlockingStub.newStub(factory, channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary calls on the service
   */
  public static CommentServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<CommentServiceFutureStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<CommentServiceFutureStub>() {
        @java.lang.Override
        public CommentServiceFutureStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new CommentServiceFutureStub(channel, callOptions);
        }
      };
    return CommentServiceFutureStub.newStub(factory, channel);
  }

  /**
   * <pre>
   * Serviço de gerenciamento de comentários
   * </pre>
   */
  public interface AsyncService {

    /**
     * <pre>
     * Adiciona um comentário a uma postagem existente
     * </pre>
     */
    default void addComment(com.blog.grpc.generated.AddCommentRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Comment> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getAddCommentMethod(), responseObserver);
    }

    /**
     * <pre>
     * Remove um comentário por ID
     * </pre>
     */
    default void deleteComment(com.blog.grpc.generated.DeleteCommentRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.DeleteResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getDeleteCommentMethod(), responseObserver);
    }

    /**
     * <pre>
     * Retorna todos os comentários de um post como stream
     * Demonstra streaming com filtro (por post_id)
     * </pre>
     */
    default void getComments(com.blog.grpc.generated.GetCommentsRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Comment> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getGetCommentsMethod(), responseObserver);
    }
  }

  /**
   * Base class for the server implementation of the service CommentService.
   * <pre>
   * Serviço de gerenciamento de comentários
   * </pre>
   */
  public static abstract class CommentServiceImplBase
      implements io.grpc.BindableService, AsyncService {

    @java.lang.Override public final io.grpc.ServerServiceDefinition bindService() {
      return CommentServiceGrpc.bindService(this);
    }
  }

  /**
   * A stub to allow clients to do asynchronous rpc calls to service CommentService.
   * <pre>
   * Serviço de gerenciamento de comentários
   * </pre>
   */
  public static final class CommentServiceStub
      extends io.grpc.stub.AbstractAsyncStub<CommentServiceStub> {
    private CommentServiceStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected CommentServiceStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new CommentServiceStub(channel, callOptions);
    }

    /**
     * <pre>
     * Adiciona um comentário a uma postagem existente
     * </pre>
     */
    public void addComment(com.blog.grpc.generated.AddCommentRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Comment> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getAddCommentMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * Remove um comentário por ID
     * </pre>
     */
    public void deleteComment(com.blog.grpc.generated.DeleteCommentRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.DeleteResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getDeleteCommentMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     * <pre>
     * Retorna todos os comentários de um post como stream
     * Demonstra streaming com filtro (por post_id)
     * </pre>
     */
    public void getComments(com.blog.grpc.generated.GetCommentsRequest request,
        io.grpc.stub.StreamObserver<com.blog.grpc.generated.Comment> responseObserver) {
      io.grpc.stub.ClientCalls.asyncServerStreamingCall(
          getChannel().newCall(getGetCommentsMethod(), getCallOptions()), request, responseObserver);
    }
  }

  /**
   * A stub to allow clients to do synchronous rpc calls to service CommentService.
   * <pre>
   * Serviço de gerenciamento de comentários
   * </pre>
   */
  public static final class CommentServiceBlockingStub
      extends io.grpc.stub.AbstractBlockingStub<CommentServiceBlockingStub> {
    private CommentServiceBlockingStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected CommentServiceBlockingStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new CommentServiceBlockingStub(channel, callOptions);
    }

    /**
     * <pre>
     * Adiciona um comentário a uma postagem existente
     * </pre>
     */
    public com.blog.grpc.generated.Comment addComment(com.blog.grpc.generated.AddCommentRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getAddCommentMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * Remove um comentário por ID
     * </pre>
     */
    public com.blog.grpc.generated.DeleteResponse deleteComment(com.blog.grpc.generated.DeleteCommentRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getDeleteCommentMethod(), getCallOptions(), request);
    }

    /**
     * <pre>
     * Retorna todos os comentários de um post como stream
     * Demonstra streaming com filtro (por post_id)
     * </pre>
     */
    public java.util.Iterator<com.blog.grpc.generated.Comment> getComments(
        com.blog.grpc.generated.GetCommentsRequest request) {
      return io.grpc.stub.ClientCalls.blockingServerStreamingCall(
          getChannel(), getGetCommentsMethod(), getCallOptions(), request);
    }
  }

  /**
   * A stub to allow clients to do ListenableFuture-style rpc calls to service CommentService.
   * <pre>
   * Serviço de gerenciamento de comentários
   * </pre>
   */
  public static final class CommentServiceFutureStub
      extends io.grpc.stub.AbstractFutureStub<CommentServiceFutureStub> {
    private CommentServiceFutureStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected CommentServiceFutureStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new CommentServiceFutureStub(channel, callOptions);
    }

    /**
     * <pre>
     * Adiciona um comentário a uma postagem existente
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<com.blog.grpc.generated.Comment> addComment(
        com.blog.grpc.generated.AddCommentRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getAddCommentMethod(), getCallOptions()), request);
    }

    /**
     * <pre>
     * Remove um comentário por ID
     * </pre>
     */
    public com.google.common.util.concurrent.ListenableFuture<com.blog.grpc.generated.DeleteResponse> deleteComment(
        com.blog.grpc.generated.DeleteCommentRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getDeleteCommentMethod(), getCallOptions()), request);
    }
  }

  private static final int METHODID_ADD_COMMENT = 0;
  private static final int METHODID_DELETE_COMMENT = 1;
  private static final int METHODID_GET_COMMENTS = 2;

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
        case METHODID_ADD_COMMENT:
          serviceImpl.addComment((com.blog.grpc.generated.AddCommentRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.Comment>) responseObserver);
          break;
        case METHODID_DELETE_COMMENT:
          serviceImpl.deleteComment((com.blog.grpc.generated.DeleteCommentRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.DeleteResponse>) responseObserver);
          break;
        case METHODID_GET_COMMENTS:
          serviceImpl.getComments((com.blog.grpc.generated.GetCommentsRequest) request,
              (io.grpc.stub.StreamObserver<com.blog.grpc.generated.Comment>) responseObserver);
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
          getAddCommentMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              com.blog.grpc.generated.AddCommentRequest,
              com.blog.grpc.generated.Comment>(
                service, METHODID_ADD_COMMENT)))
        .addMethod(
          getDeleteCommentMethod(),
          io.grpc.stub.ServerCalls.asyncUnaryCall(
            new MethodHandlers<
              com.blog.grpc.generated.DeleteCommentRequest,
              com.blog.grpc.generated.DeleteResponse>(
                service, METHODID_DELETE_COMMENT)))
        .addMethod(
          getGetCommentsMethod(),
          io.grpc.stub.ServerCalls.asyncServerStreamingCall(
            new MethodHandlers<
              com.blog.grpc.generated.GetCommentsRequest,
              com.blog.grpc.generated.Comment>(
                service, METHODID_GET_COMMENTS)))
        .build();
  }

  private static abstract class CommentServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoFileDescriptorSupplier, io.grpc.protobuf.ProtoServiceDescriptorSupplier {
    CommentServiceBaseDescriptorSupplier() {}

    @java.lang.Override
    public com.google.protobuf.Descriptors.FileDescriptor getFileDescriptor() {
      return com.blog.grpc.generated.BlogProto.getDescriptor();
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.ServiceDescriptor getServiceDescriptor() {
      return getFileDescriptor().findServiceByName("CommentService");
    }
  }

  private static final class CommentServiceFileDescriptorSupplier
      extends CommentServiceBaseDescriptorSupplier {
    CommentServiceFileDescriptorSupplier() {}
  }

  private static final class CommentServiceMethodDescriptorSupplier
      extends CommentServiceBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoMethodDescriptorSupplier {
    private final java.lang.String methodName;

    CommentServiceMethodDescriptorSupplier(java.lang.String methodName) {
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
      synchronized (CommentServiceGrpc.class) {
        result = serviceDescriptor;
        if (result == null) {
          serviceDescriptor = result = io.grpc.ServiceDescriptor.newBuilder(SERVICE_NAME)
              .setSchemaDescriptor(new CommentServiceFileDescriptorSupplier())
              .addMethod(getAddCommentMethod())
              .addMethod(getDeleteCommentMethod())
              .addMethod(getGetCommentsMethod())
              .build();
        }
      }
    }
    return result;
  }
}
