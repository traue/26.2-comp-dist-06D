# Calculadora gRPC com Java 17 + Maven

Projeto simples utilizando gRPC com Java 17 e Maven

---

# Tecnologias utilizadas

- Java 17
- Maven 3.8+
- gRPC 1.68.1
- Protocol Buffers 3.25.5
- IntelliJ IDEA (recomendado)

---

# Estrutura do Projeto

```text
grpc-calculadora/
│
├── pom.xml
│
└── src/
    └── main/
        ├── java/
        │   └── br/
        │       └── mackenzie/
        │           └── grpc/
        │               ├── server/
        │               │   ├── CalculadoraServiceImpl.java
        │               │   └── ServidorGrpc.java
        │               │
        │               └── client/
        │                   └── ClienteGrpc.java
        │
        └── proto/
            └── calculadora.proto
```

> ⚠️ **Atenção:** o arquivo `calculadora.proto` **precisa** estar exatamente em `src/main/proto/`. Em outro lugar, o plugin não enxerga e não gera as classes.

---

# 1. Criando o projeto Maven

```bash
mvn archetype:generate \
  -DgroupId=br.mackenzie.grpc \
  -DartifactId=grpc-calculadora \
  -DarchetypeArtifactId=maven-archetype-quickstart \
  -DinteractiveMode=false
```

```bash
cd grpc-calculadora
```

Após gerar, **delete** os arquivos `App.java` e `AppTest.java` (não serão usados):

```bash
rm src/main/java/br/mackenzie/grpc/App.java
rm src/test/java/br/mackenzie/grpc/AppTest.java
```

Alternativamente, você pode criar o projeto manualmente no IntelliJ, mas certifique-se de configurar o `pom.xml` corretamente (próximo passo).

---

# 2. Configurando o pom.xml

Substitua TODO o conteúdo do `pom.xml` por:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
         http://maven.apache.org/xsd/maven-4.0.0.xsd">

    <modelVersion>4.0.0</modelVersion>

    <groupId>br.mackenzie.grpc</groupId>
    <artifactId>grpc-calculadora</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>jar</packaging>

    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.compiler.release>17</maven.compiler.release>

        <grpc.version>1.68.1</grpc.version>
        <protobuf.version>3.25.5</protobuf.version>
    </properties>

    <dependencies>

        <dependency>
            <groupId>io.grpc</groupId>
            <artifactId>grpc-netty-shaded</artifactId>
            <version>${grpc.version}</version>
        </dependency>

        <dependency>
            <groupId>io.grpc</groupId>
            <artifactId>grpc-protobuf</artifactId>
            <version>${grpc.version}</version>
        </dependency>

        <dependency>
            <groupId>io.grpc</groupId>
            <artifactId>grpc-stub</artifactId>
            <version>${grpc.version}</version>
        </dependency>

        <!--
            Necessária para @javax.annotation.Generated nas classes geradas.
            O Java 17 removeu esse pacote do JDK, então precisamos incluir
            manualmente para o código gerado pelo gRPC compilar.
        -->
        <dependency>
            <groupId>javax.annotation</groupId>
            <artifactId>javax.annotation-api</artifactId>
            <version>1.3.2</version>
        </dependency>

    </dependencies>

    <build>

        <extensions>
            <!-- Detecta o SO/arquitetura para baixar o protoc correto -->
            <extension>
                <groupId>kr.motd.maven</groupId>
                <artifactId>os-maven-plugin</artifactId>
                <version>1.7.1</version>
            </extension>
        </extensions>

        <plugins>

            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.13.0</version>
                <configuration>
                    <release>${maven.compiler.release}</release>
                </configuration>
            </plugin>

            <!-- Gera classes Java a partir do .proto -->
            <plugin>
                <groupId>org.xolstice.maven.plugins</groupId>
                <artifactId>protobuf-maven-plugin</artifactId>
                <version>0.6.1</version>

                <configuration>
                    <protocArtifact>
                        com.google.protobuf:protoc:${protobuf.version}:exe:${os.detected.classifier}
                    </protocArtifact>
                    <pluginId>grpc-java</pluginId>
                    <pluginArtifact>
                        io.grpc:protoc-gen-grpc-java:${grpc.version}:exe:${os.detected.classifier}
                    </pluginArtifact>
                </configuration>

                <executions>
                    <execution>
                        <goals>
                            <goal>compile</goal>
                            <goal>compile-custom</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>

            <!-- Necessário para `mvn exec:java` funcionar -->
            <plugin>
                <groupId>org.codehaus.mojo</groupId>
                <artifactId>exec-maven-plugin</artifactId>
                <version>3.5.0</version>
            </plugin>

        </plugins>

    </build>

</project>
```

---

# 3. Arquivo calculadora.proto

Crie:

```text
src/main/proto/calculadora.proto
```

Conteúdo:

```proto
syntax = "proto3";

option java_multiple_files = true;
option java_package = "br.mackenzie.grpc";
option java_outer_classname = "CalculadoraProto";

service CalculadoraService {

  rpc somar (CalculadoraRequest)
      returns (CalculadoraResponse);

  rpc subtrair (CalculadoraRequest)
      returns (CalculadoraResponse);

  rpc multiplicar (CalculadoraRequest)
      returns (CalculadoraResponse);

  rpc dividir (CalculadoraRequest)
      returns (CalculadoraResponse);
}

message CalculadoraRequest {
  double numero1 = 1;
  double numero2 = 2;
}

message CalculadoraResponse {
  double resultado = 1;
}
```

---

# 4. Gerando as classes automaticamente

```bash
mvn clean compile
```

Isso baixa o `protoc`, gera as classes em `target/generated-sources/protobuf/` e compila o projeto.

**Verificação:** confira se as classes foram geradas:

```bash
find target/generated-sources -name "*.java"
```

Você deve ver:
```text
target/generated-sources/protobuf/java/br/mackenzie/grpc/CalculadoraRequest.java
target/generated-sources/protobuf/java/br/mackenzie/grpc/CalculadoraResponse.java
target/generated-sources/protobuf/grpc-java/br/mackenzie/grpc/CalculadoraServiceGrpc.java
```

---

# 5. Implementando o servidor

Arquivo:

```text
src/main/java/br/mackenzie/grpc/server/CalculadoraServiceImpl.java
```

```java
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
```

---

# 6. Servidor principal

Arquivo:

```text
src/main/java/br/mackenzie/grpc/server/ServidorGrpc.java
```

```java
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
```

---

# 7. Cliente gRPC

Arquivo:

```text
src/main/java/br/mackenzie/grpc/client/ClienteGrpc.java
```

```java
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
```

---

# 8. Compilando

```bash
mvn clean package
```

---

# 9. Executando o servidor

Em um terminal:

```bash
cd grpc-calculadora
mvn exec:java -Dexec.mainClass="br.mackenzie.grpc.server.ServidorGrpc"
```

Ou, no IntelliJ, clique direito em `ServidorGrpc.java` → **Run 'ServidorGrpc.main()'**.

---

# 10. Executando o cliente

Em **outro** terminal (com o servidor rodando):

```bash
cd grpc-calculadora
mvn exec:java -Dexec.mainClass="br.mackenzie.grpc.client.ClienteGrpc"
```

Ou, no IntelliJ, clique direito em `ClienteGrpc.java` → **Run 'ClienteGrpc.main()'**.

---

# Saída esperada

**Servidor (com logs de cada chamada):**
```text
Servidor gRPC iniciado na porta 50051
[somar]      20,00 + 10,00 = 30,00
[subtrair]   20,00 - 10,00 = 10,00
[multiplicar] 20,00 × 10,00 = 200,00
[dividir]    20,00 ÷ 10,00 = 2,00
[dividir]    10,00 ÷ 0,00 -> ERRO: divisão por zero
```

**Cliente (interativo):**
```text
=== Calculadora gRPC ===

Escolha a operação:
  1 - Somar
  2 - Subtrair
  3 - Multiplicar
  4 - Dividir
  0 - Sair
Opção: 1
Número 1: 20
Número 2: 10
Resultado: 20,00 + 10,00 = 30,00

Opção: 4
Número 1: 10
Número 2: 0
Erro: Divisão por zero não permitida

Opção: 0
Encerrando cliente.
```

> O cliente exibe um menu em loop. Digite `0` para sair. Entradas inválidas (letras no lugar de números, opção fora do menu) são tratadas sem encerrar o programa.

---

# Conceitos importantes

## Protocol Buffers
Formato binário eficiente criado pelo Google para serialização de dados estruturados.

## Stub
Cliente gerado automaticamente pelo gRPC a partir do `.proto`.

## RPC
Remote Procedure Call. Permite executar métodos remotos como se fossem locais.

## Status
Forma idiomática do gRPC sinalizar erros (`INVALID_ARGUMENT`, `NOT_FOUND`, etc.), em vez de exceções Java genéricas.

---

# Vantagens do gRPC

- Alta performance
- HTTP/2 (multiplexação, streaming bidirecional)
- Serialização binária compacta
- Forte tipagem via `.proto`
- Excelente para microsserviços
- Geração automática de código em múltiplas linguagens

---

# Troubleshooting

| Problema | Causa provável | Solução |
|---|---|---|
| `mvn clean compile` não gera nada do `.proto` | Arquivo `.proto` fora de `src/main/proto/` | Mover para o local correto |
| `cannot find symbol: package javax.annotation` | Java 17 removeu `javax.annotation` do JDK | Adicionar `javax.annotation-api 1.3.2` no `pom.xml` (já incluído) |
| `cannot find symbol: CalculadoraServiceGrpc` (Maven) | `mvn compile` não rodou | Rodar `mvn clean compile` |
| Imports em vermelho na IDE, mas Maven compila | IDE não enxerga `target/generated-sources` | **IntelliJ:** Maven → Reload Project. **VS Code:** `Java: Clean Java Language Server Workspace` |
| `Address already in use` | Porta 50051 ocupada | Trocar porta em `ServidorGrpc.java` |
| `Goal requires a project to execute but there is no POM` | Rodando `mvn` fora da pasta do projeto | Entrar em `cd grpc-calculadora` antes |
| `UNAVAILABLE: io exception` no cliente | Servidor não está rodando | Subir o servidor primeiro |
| `protoc` falha ao baixar | Sem internet / firewall corporativo | Verificar conexão e proxy do Maven em `~/.m2/settings.xml` |

---

# Notas sobre IDE

**IntelliJ IDEA** é a recomendação para este projeto — ele reconhece automaticamente as pastas `target/generated-sources/protobuf/java` e `target/generated-sources/protobuf/grpc-java` após `Maven → Reload Project`.

No **VS Code**, pode ser necessário rodar manualmente:
- `Ctrl+Shift+P` → **Java: Clean Java Language Server Workspace** → **Restart and Delete**.