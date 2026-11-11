package br.mackenzie.grpc.server;

import br.mackenzie.grpc.CalculadoraRequest;
import br.mackenzie.grpc.CalculadoraResponse;
import br.mackenzie.grpc.CalculadoraServiceGrpc;

import io.grpc.Status;
import io.grpc.stub.StreamObserver;

public class CalculadoraServiceImpl
        extends CalculadoraServiceGrpc.CalculadoraServiceImplBase {

    @Override
    public void somar(
            CalculadoraRequest request,
            StreamObserver<CalculadoraResponse> responseObserver) {

        double a = request.getNumero1();
        double b = request.getNumero2();
        double resultado = a + b;

        System.out.printf("[somar]      %.2f + %.2f = %.2f%n", a, b, resultado);

        responseObserver.onNext(CalculadoraResponse.newBuilder()
                .setResultado(resultado).build());
        responseObserver.onCompleted();
    }

    @Override
    public void subtrair(
            CalculadoraRequest request,
            StreamObserver<CalculadoraResponse> responseObserver) {

        double a = request.getNumero1();
        double b = request.getNumero2();
        double resultado = a - b;

        System.out.printf("[subtrair]   %.2f - %.2f = %.2f%n", a, b, resultado);

        responseObserver.onNext(CalculadoraResponse.newBuilder()
                .setResultado(resultado).build());
        responseObserver.onCompleted();
    }

    @Override
    public void multiplicar(
            CalculadoraRequest request,
            StreamObserver<CalculadoraResponse> responseObserver) {

        double a = request.getNumero1();
        double b = request.getNumero2();
        double resultado = a * b;

        System.out.printf("[multiplicar] %.2f × %.2f = %.2f%n", a, b, resultado);

        responseObserver.onNext(CalculadoraResponse.newBuilder()
                .setResultado(resultado).build());
        responseObserver.onCompleted();
    }

    @Override
    public void dividir(
            CalculadoraRequest request,
            StreamObserver<CalculadoraResponse> responseObserver) {

        double a = request.getNumero1();
        double b = request.getNumero2();

        if (b == 0) {
            System.out.printf("[dividir]    %.2f ÷ %.2f -> ERRO: divisão por zero%n", a, b);
            responseObserver.onError(
                    Status.INVALID_ARGUMENT
                            .withDescription("Divisão por zero não permitida")
                            .asRuntimeException()
            );
            return;
        }

        double resultado = a / b;

        System.out.printf("[dividir]    %.2f ÷ %.2f = %.2f%n", a, b, resultado);

        responseObserver.onNext(CalculadoraResponse.newBuilder()
                .setResultado(resultado).build());
        responseObserver.onCompleted();
    }
}
