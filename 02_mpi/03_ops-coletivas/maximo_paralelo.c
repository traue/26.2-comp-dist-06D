/**
 * O programa tem como objetivo:
	•	Criar um vetor global de 42 números.
	•	Dividir igualmente esse vetor entre todos os processos MPI.
	•	Cada processo gera números aleatórios, encontra o máximo local do seu pedaço.
	•	Com MPI_Allgather, todos os processos trocam seus máximos locais entre si.
	•	Cada processo calcula, por conta própria, o máximo global a partir dessa troca.
 */


#include <stdio.h>      // Biblioteca padrão de entrada e saída (printf, etc.)
#include <stdlib.h>     // Para alocação dinâmica e geração de números aleatórios
#include <mpi.h>        // Biblioteca MPI para comunicação entre processos

int main() {

    int numberP; // Número total de processos MPI em execução
    int rank;    // Identificador (rank) do processo atual

    MPI_Init(NULL, NULL); // Inicializa o ambiente MPI

    MPI_Comm_size(MPI_COMM_WORLD, &numberP); // Obtém o número total de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    // Obtém o ID (rank) do processo atual

    int N = 42; // Tamanho total do vetor global a ser distribuído

    // Aloca o vetor local de cada processo com tamanho proporcional (N / numberP)
    int* local_array = (int *)malloc(N / numberP * sizeof(int));

    // Inicializa a semente do gerador de números aleatórios com base no rank
    unsigned int seed = rank + 1;
    srand(seed);

    // Preenche o vetor local com valores aleatórios entre 0 e 99
    for (int i = 0; i < N / numberP; i++) {
        local_array[i] = rand() % 100;
    }

    // Exibe o vetor local gerado por este processo
    printf("Processo %d Vetor Local:: ", rank);
    for (int i = 0; i < N / numberP; ++i) {
        printf("%d ", local_array[i]);
    }
    printf("\n");

    // Calcula o máximo local dentro do vetor de cada processo
    int local_max = local_array[0];
    for (int i = 1; i < N / numberP; ++i) {
        if (local_array[i] > local_max) {
            local_max = local_array[i];
        }
    }

    // Aloca vetor para armazenar os máximos locais de todos os processos
    int *all_max_values = (int *)malloc(numberP * sizeof(int));

    // Usa MPI_Allgather para compartilhar os máximos locais entre todos os processos
    MPI_Allgather(&local_max, 1, MPI_INT,
                  all_max_values, 1, MPI_INT,
                  MPI_COMM_WORLD);

    // Exibe os resultados de cada processo
    printf("Processo %d Máximo local: %d, Todos valores máximos: ", rank, local_max);
    for (int i = 0; i < numberP; ++i) {
        printf("%d ", all_max_values[i]);
        if (i == numberP - 1) printf("\n");
    }

    // Calcula o máximo global com base nos máximos locais
    int global_max = all_max_values[0];
    for (int i = 1; i < numberP; ++i) {
        if (all_max_values[i] > global_max) {
            global_max = all_max_values[i];
        }
    }

    // Imprime o máximo global (todos os processos podem imprimir, pois todos têm o valor)
    printf("Processo %d Máximo Global: %d\n", rank, global_max);

    // Libera a memória alocada dinamicamente
    free(local_array);
    free(all_max_values);

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}