package br.mackenzie.grpc.server;

import io.grpc.Server;
import io.grpc.ServerBuilder;

public class ServidorGrpc {

    public static void main(String[] args) throws Exception {

        Server server = ServerBuilder
                .forPort(50051)
                .addService(new CalculadoraServiceImpl())
                .build();

        server.start();

        System.out.println("Servidor gRPC iniciado na porta 50051");

        // Encerramento limpo ao receber SIGINT/SIGTERM (Ctrl+C)
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("Encerrando servidor gRPC...");
            server.shutdown();
            System.out.println("Servidor encerrado.");
        }));

        server.awaitTermination();
    }
}
