package br.mackenzie.grpc.client;

import br.mackenzie.grpc.CalculadoraRequest;
import br.mackenzie.grpc.CalculadoraServiceGrpc;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.StatusRuntimeException;

import java.util.Scanner;
import java.util.concurrent.TimeUnit;

public class ClienteGrpc {

    public static void main(String[] args) throws InterruptedException {

        ManagedChannel channel = ManagedChannelBuilder
                .forAddress("localhost", 50051)
                .usePlaintext()
                .build();

        try {

            CalculadoraServiceGrpc.CalculadoraServiceBlockingStub stub =
                    CalculadoraServiceGrpc.newBlockingStub(channel);

            Scanner scanner = new Scanner(System.in);

            System.out.println("=== Calculadora gRPC ===");

            while (true) {
                System.out.println();
                System.out.println("Escolha a operação:");
                System.out.println("  1 - Somar");
                System.out.println("  2 - Subtrair");
                System.out.println("  3 - Multiplicar");
                System.out.println("  4 - Dividir");
                System.out.println("  0 - Sair");
                System.out.print("Opção: ");

                String opcao = scanner.nextLine().trim();

                if (opcao.equals("0")) {
                    System.out.println("Encerrando cliente.");
                    break;
                }

                if (!opcao.matches("[1-4]")) {
                    System.out.println("Opção inválida. Tente novamente.");
                    continue;
                }

                double numero1;
                double numero2;

                try {
                    System.out.print("Número 1: ");
                    numero1 = Double.parseDouble(scanner.nextLine().trim());

                    System.out.print("Número 2: ");
                    numero2 = Double.parseDouble(scanner.nextLine().trim());
                } catch (NumberFormatException e) {
                    System.out.println("Valor inválido. Digite um número.");
                    continue;
                }

                CalculadoraRequest request = CalculadoraRequest.newBuilder()
                        .setNumero1(numero1)
                        .setNumero2(numero2)
                        .build();

                try {
                    double resultado;
                    switch (opcao) {
                        case "1" -> {
                            resultado = stub.somar(request).getResultado();
                            System.out.printf("Resultado: %.2f + %.2f = %.2f%n",
                                    numero1, numero2, resultado);
                        }
                        case "2" -> {
                            resultado = stub.subtrair(request).getResultado();
                            System.out.printf("Resultado: %.2f - %.2f = %.2f%n",
                                    numero1, numero2, resultado);
                        }
                        case "3" -> {
                            resultado = stub.multiplicar(request).getResultado();
                            System.out.printf("Resultado: %.2f × %.2f = %.2f%n",
                                    numero1, numero2, resultado);
                        }
                        case "4" -> {
                            resultado = stub.dividir(request).getResultado();
                            System.out.printf("Resultado: %.2f ÷ %.2f = %.2f%n",
                                    numero1, numero2, resultado);
                        }
                    }
                } catch (StatusRuntimeException e) {
                    System.out.println("Erro: " + e.getStatus().getDescription());
                }
            }

            scanner.close();

        } finally {
            channel.shutdown().awaitTermination(5, TimeUnit.SECONDS);
        }
    }
}
