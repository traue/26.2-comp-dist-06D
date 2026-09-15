#include <mpi.h>     // Inclui a biblioteca do MPI, necessária para funções de comunicação paralela
#include <stdio.h>   // Inclui a biblioteca padrão de entrada e saída, para usar printf

int main (int argc, char **argv) {
    int rank, size;  // Declara duas variáveis inteiras:
                     // 'rank' será o identificador único do processo (ID)
                     // 'size' será o número total de processos em execução

    MPI_Init(&argc, &argv);  
    // Inicializa o ambiente MPI.
    // Deve ser chamada antes de qualquer outra função MPI.

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Obtém o identificador (rank) do processo atual dentro do comunicador global MPI_COMM_WORLD
    // Ex: se há 4 processos, os ranks serão 0, 1, 2 e 3

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Obtém o número total de processos que participam do comunicador

    printf("Hello, I am process %d of %d\n", rank, size);
    // Imprime uma mensagem indicando qual o rank (ID) do processo atual
    // e quantos processos existem no total

    MPI_Finalize();
    // Finaliza o ambiente MPI. Deve ser a última função MPI chamada.
    // Libera os recursos alocados pela biblioteca

    return 0;
    // Encerra o programa com sucesso
}