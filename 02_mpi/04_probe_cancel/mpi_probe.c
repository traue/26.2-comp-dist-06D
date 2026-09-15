#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);  // Inicializa o ambiente MPI

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtém o rank (ID) do processo atual

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtém o número total de processos

    if (size < 2) {
        fprintf(stderr, "We must have at least 2 processes\n");  // Garante no mínimo dois processos
        MPI_Abort(MPI_COMM_WORLD, 1);  // Aborta a execução se houver menos de 2 processos
    }

    int number_amount;  // Quantidade de números a serem enviados ou recebidos

    if (rank == 0) {
        srand(time(NULL) + rank);  // Inicializa a semente do gerador de números aleatórios com valor único
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];  // Cria um vetor de inteiros com capacidade máxima de 100

        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;  // Gera uma quantidade aleatória de números a enviar

        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);  // Envia os números ao processo de rank 1

        printf("Process 0 sent %d numbers to process 1\n", number_amount);  // Imprime o total enviado
    }

    else if (rank == 1) {
        MPI_Status status;

        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);  // Espia a mensagem vinda do processo 0 sem recebê-la ainda

        MPI_Get_count(&status, MPI_INT, &number_amount);  // Descobre quantos inteiros estão na mensagem

        int* numb_buffer = (int*)malloc(sizeof(int) * number_amount);  // Aloca memória suficiente para armazenar os dados

        MPI_Recv(numb_buffer, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Agora recebe a mensagem

        printf("Process 1 received %d numbers from process 0\n", number_amount);  // Imprime o total recebido

        free(numb_buffer);  // Libera a memória alocada
    }

    MPI_Finalize();  // Finaliza o ambiente MPI

    return 0;
}