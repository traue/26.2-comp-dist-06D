# Probe e Cancel em MPI

## Introdução

As operações de comunicação ponto a ponto mais básicas do MPI — `MPI_Send` e `MPI_Recv` — pressupõem que o receptor já sabe o que está chegando: tipo, quantidade, origem e tag. Na prática, isso nem sempre é possível. Às vezes o remetente determina o tamanho da mensagem em tempo de execução, como resultado de algum cálculo, e o receptor precisa se adaptar dinamicamente. Em outros casos, uma operação de envio que foi iniciada precisa ser interrompida antes de completar.

Este diretório explora duas capacidades do MPI que tratam exatamente dessas situações: `MPI_Probe`, que permite inspecionar uma mensagem sem recebê-la, e `MPI_Cancel`, que tenta cancelar uma operação de comunicação pendente.

Para entender o cancelamento, é necessário primeiro entender as operações não bloqueantes.

---

## Operações não bloqueantes

As funções `MPI_Send` e `MPI_Recv` são bloqueantes: o processo fica parado esperando até que a operação conclua. As versões não bloqueantes, `MPI_Isend` e `MPI_Irecv`, iniciam a operação e retornam imediatamente, sem esperar pelo término. A operação continua em segundo plano, e o programa pode fazer outras coisas enquanto isso.

O controle sobre a operação é feito por meio de um objeto do tipo `MPI_Request`. Para verificar se terminou, usa-se `MPI_Test` (que retorna imediatamente com uma flag) ou `MPI_Wait` (que bloqueia até a conclusão).

```c
MPI_Request request;
MPI_Status status;

MPI_Isend(data, size, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

// ... pode fazer outras coisas aqui ...

MPI_Wait(&request, &status);  // espera o envio terminar
```

A motivação para operações não bloqueantes é performance: em vez de esperar passivamente, o processo pode adiantar cálculos, preparar os próximos dados ou realizar operações de I/O. Em algoritmos mais avançados, é comum intercalar comunicação e computação exatamente por esse motivo.

---

## MPI_Cancel — Cancelando operações pendentes

Uma vez iniciada uma operação não bloqueante, é possível tentar cancelá-la com `MPI_Cancel`. A palavra "tentar" é deliberada: o MPI **não garante** que o cancelamento vai funcionar. Se a mensagem já foi transmitida ou bufferizada internamente antes de o cancelamento ser processado, ela pode chegar normalmente ao destino.

O fluxo correto de uso é:

```c
MPI_Isend(data, data_size, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

MPI_Cancel(&request);          // solicita o cancelamento
MPI_Wait(&request, &status);   // aguarda o desfecho da operação

int flag;
MPI_Test_cancelled(&status, &flag);  // verifica se o cancelamento ocorreu

if (flag) {
    printf("Operação cancelada com sucesso.\n");
} else {
    printf("A mensagem já havia sido enviada.\n");
}
```

Note a separação de responsabilidades: `MPI_Cancel` apenas *solicita* o cancelamento; `MPI_Wait` espera a operação chegar a um desfecho (seja enviando normalmente, seja sendo cancelada); e `MPI_Test_cancelled` examina o status resultante para informar o que de fato aconteceu.

Na prática, `MPI_Cancel` é mais útil para recepções (`MPI_Irecv`) do que para envios. Cancelar um `MPI_Isend` frequentemente falha porque a implementação do MPI já bufferizou a mensagem internamente antes de o cancelamento chegar. O caso de uso mais robusto é cancelar um `MPI_Irecv` que está esperando por uma mensagem que nunca vai chegar — útil em situações de timeout ou mudança de fluxo de controle.

---

## MPI_Probe — Inspecionando sem receber

`MPI_Probe` resolve um problema diferente. Imagine que o processo 0 envia um vetor de tamanho variável — ele decide a quantidade de elementos em tempo de execução. O processo 1, para receber, precisa alocar um buffer do tamanho correto. Mas como saber o tamanho antes de receber a mensagem?

Uma solução ingênua seria usar dois envios: um com o tamanho e outro com os dados. Isso funciona, mas adiciona uma rodada de comunicação e complica o código. `MPI_Probe` é uma alternativa mais limpa: ele examina a mensagem que está na fila para ser recebida, sem extraí-la do canal. O processo receptor pode então consultar o tamanho real com `MPI_Get_count`, alocar o buffer com o tamanho exato, e só então chamar `MPI_Recv`.

```c
MPI_Status status;

// Examina a mensagem do processo 0, tag 0, sem recebê-la
MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

// Descobre quantos inteiros estão na mensagem
int number_amount;
MPI_Get_count(&status, MPI_INT, &number_amount);

// Aloca o buffer com o tamanho exato
int *buffer = (int*)malloc(sizeof(int) * number_amount);

// Agora recebe a mensagem
MPI_Recv(buffer, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
```

`MPI_Probe` é bloqueante: ele espera até que haja uma mensagem correspondente disponível, com a origem, tag e comunicador especificados. Existe também `MPI_Iprobe`, a versão não bloqueante, que retorna imediatamente com uma flag indicando se há ou não mensagem disponível — útil quando o processo quer continuar trabalhando enquanto aguarda.

Uma garantia importante: entre o `MPI_Probe` e o `MPI_Recv` subsequente, a mensagem não "desaparece" e nenhum outro processo pode roubá-la. Ela fica reservada para que o mesmo processo a receba em seguida.

---

## Os exemplos neste diretório

### mpi_probe.c

O processo 0 gera uma quantidade aleatória de inteiros — entre 0 e 100, decidida em tempo de execução — e os envia para o processo 1 com `MPI_Send`. O processo 1 não sabe quantos inteiros estão chegando. Ele usa `MPI_Probe` para inspecionar a mensagem e `MPI_Get_count` para extrair o tamanho real. Com esse valor em mãos, aloca o buffer com `malloc` e então chama `MPI_Recv`.

O ponto central do exemplo é a separação entre descobrir o tamanho e receber os dados — as duas chamadas ficam explicitamente separadas no código. É um padrão muito útil em aplicações reais onde o volume de dados depende do resultado de algum cálculo no remetente, como em simulações que produzem saídas de tamanho variável.

### mpi_cancel.c

O processo 0 inicializa um vetor com os valores de 0 a 9 e inicia um envio não bloqueante para o processo 1 com `MPI_Isend`. Logo em seguida, sem esperar nada, solicita o cancelamento com `MPI_Cancel`. Usa `MPI_Wait` para aguardar o desfecho e `MPI_Test_cancelled` para verificar se o cancelamento ocorreu.

Enquanto isso, o processo 1 aguarda um segundo com `sleep(1)` antes de tentar receber. Esse atraso dá ao processo 0 tempo para tentar cancelar antes de a recepção acontecer.

Ao executar, o resultado pode variar: dependendo da implementação do MPI e do estado interno dos buffers, o envio pode ou não ter sido cancelado quando o processo 1 tenta receber. Esse comportamento não determinístico é parte do que o exemplo quer mostrar — o cancelamento não é uma operação garantida e o código correto sempre verifica o resultado com `MPI_Test_cancelled`.

---

## Como compilar e executar

```bash
# Compilar
mpicc -o mpi_probe mpi_probe.c
mpicc -o mpi_cancel mpi_cancel.c

# Executar (ambos exigem exatamente 2 processos)
mpirun -np 2 ./mpi_probe
mpirun -np 2 ./mpi_cancel
```

Ao testar o `mpi_cancel`, execute algumas vezes seguidas e observe se o resultado muda entre execuções. Em algumas implementações do MPI, o cancelamento de um `MPI_Isend` raramente funciona porque a mensagem é bufferizada imediatamente. Em outras, com buffers menores, o cancelamento pode ter sucesso. Isso evidencia que código robusto jamais assume que o cancelamento funcionou — sempre verifica.

---

## Comparativo rápido

| Função               | O que faz                                              |
|----------------------|--------------------------------------------------------|
| `MPI_Isend`          | Inicia envio não bloqueante, retorna imediatamente     |
| `MPI_Irecv`          | Inicia recepção não bloqueante, retorna imediatamente  |
| `MPI_Wait`           | Bloqueia até a operação associada ao request terminar  |
| `MPI_Test`           | Verifica sem bloquear se a operação terminou           |
| `MPI_Cancel`         | Solicita cancelamento de uma operação pendente         |
| `MPI_Test_cancelled` | Verifica se o cancelamento de fato ocorreu             |
| `MPI_Probe`          | Inspeciona mensagem disponível sem recebê-la           |
| `MPI_Iprobe`         | Versão não bloqueante do Probe                         |
| `MPI_Get_count`      | Extrai a quantidade de elementos do status de uma mensagem |
