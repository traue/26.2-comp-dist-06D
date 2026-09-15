Os códigos apresentados desta aula são exemplos simples de comunicação entre dois processos paralelos usando a biblioteca MPI em C.


----

### O que este exemplo faz?


1. Inicialização MPI

```c
MPI_Init(&argc, &argv);
```

Inicializa o ambiente MPI, preparando-o para a execução paralela.


2. Obtenção do Rank do Processo


```c
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
```

Cada processo MPI possui um  identificador único chamado rank, que é obtido nesta linha. Aqui, há dois processos: um com rank 0 e outro com rank 1.


3. Envio da mensagem (Processo 0)


```c
if (rank == 0) {
    sent_result = MPI_Send(msg, strlen(msg)+1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
```

- O processo com rank 0 envia uma mensagem (msg) para o processo com rank 1
- A mensagem enviada é a string "Gold Coast".
- A mensagem é enviada usando a função MPI_Send.
- Parâmetros da função MPI_Send:
    - msg: buffer contendo a mensagem a enviar.
    - strlen(msg)+1: comprimento da mensagem incluindo o caractere nulo final ('\0').
	- MPI_CHAR: tipo de dado sendo enviado (caractere).
	- 1: rank do processo destino.
	- tag: identificador da mensagem (24 neste caso).
	- MPI_COMM_WORLD: comunicador MPI padrão, envolvendo todos os processos.


4. Recebimento da mensagem (Processo 1)

```c
else if (rank == 1) {
    received_result = MPI_Recv(msg, sizeof(msg), MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
```

- O processo com rank 1 recebe a mensagem do processo com rank 0.
- Parâmetros da função MPI_Recv:
	- msg: buffer para armazenar a mensagem recebida.
	- sizeof(msg): tamanho máximo do buffer (20 caracteres).
	- MPI_CHAR: tipo de dado esperado.
	- 0: rank do processo que enviou a mensagem.
	- tag: identificador da mensagem (deve coincidir com o do envio).
	- MPI_COMM_WORLD: comunicador padrão.
	- status: variável que armazena informações sobre a mensagem recebida.


5. Verificação do status das operações

O código verifica se as operações de envio e recebimento ocorreram com sucesso (MPI_SUCCESS). Caso contrário, imprime um erro e aborta a execução com MPI_Abort


6. Informação sobre a mensagem recebida

```c
MPI_Get_count(&status, MPI_CHAR, &received_size);
```

A função acima é usada para determinar o tamanho real da mensagem recebida pelo processo 1. O resultado é impresso junto com o conteúdo da mensagem.


7.Finalização MPI

```c
MPI_Finalize();
```

Finaliza o ambiente MPI e encerra o programa paralelo adequadamente.

----

#### Compilação:

Para compilar:

```sh
mpicc -o 01_ponto_a_ponto 01_ponto_a_ponto.c
```

Execução:

```sh
mpirun -np 2 ./01_ponto_a_ponto 
```

Ao executar o programa com dois processos paralelos, você receberá algo como:

Processo 0 anviou a mensagem - Java é mais legal - ao Processo 1
Processo 1 recebeu a mensagem - Java é mais legal - com tamanho 19 do Processo 0



#### Resumo:

Esse código faz uma comunicação simples ponto-a-ponto onde um processo (rank 0) envia uma mensagem e o outro (rank 1) a recebe. É um exemplo básico para ilustrar a comunicação entre processos utilizando MPI


