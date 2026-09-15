#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int i;
    int total_size = 100;          // Tamanho total do vetor (1 a 100)
    int process_rank, num_processes;
    int *data = NULL;              // Ponteiro para o vetor completo (só usado pelo root)
    int *local_data;               // Vetor local com parte dos dados em cada processo
    int local_sum = 0;             // Soma parcial de cada processo
    int *partial_sums = NULL;      // Vetor para coletar somas parciais no processo root
    int total_sum = 0;             // Soma final (calculada pelo processo root)

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank (identificador) do processo atual
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    // Obtém o número total de processos em execução
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    // Cada processo irá receber uma parte igual do vetor
    int chunk_size = total_size / num_processes;

    // Apenas o processo 0 (root) aloca e preenche o vetor com valores de 1 a 100
    if (process_rank == 0) {
        data = (int *)malloc(total_size * sizeof(int));
        for (i = 0; i < total_size; i++) {
            data[i] = i + 1;
        }
    }

    // Todos os processos alocam memória para seu pedaço do vetor
    local_data = (int *)malloc(chunk_size * sizeof(int));

    // Scatter: distribui pedaços do vetor do root para todos os processos
    MPI_Scatter(data, chunk_size, MPI_INT, local_data, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo calcula a soma parcial de seu pedaço
    for (i = 0; i < chunk_size; i++) {
        local_sum += local_data[i];
    }

    // O processo root aloca memória para receber todas as somas parciais
    if (process_rank == 0) {
        partial_sums = (int *)malloc(num_processes * sizeof(int));
    }

    // Gather: envia a soma parcial de cada processo para o processo root
    MPI_Gather(&local_sum, 1, MPI_INT, partial_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // O processo root soma todas as somas parciais para obter o resultado final
    if (process_rank == 0) {
        for (i = 0; i < num_processes; i++) {
            total_sum += partial_sums[i];
        }

        printf("Soma total: %d\n", total_sum); // Deve imprimir 5050
        free(data);
        free(partial_sums);
    }

    // Libera a memória do vetor local
    free(local_data);

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}