#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int valor_local;       // Valor local de cada processo
    int soma_global = 0;   // Variável onde o processo root armazenará a soma final

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Pega o número total de processos em execução
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Pega o identificador (rank) do processo atual
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Cada processo define um valor local (aqui, só para fins de teste, será igual ao seu rank + 1)
    valor_local = rank + 1;

    // Mostra o valor local de cada processo
    printf("Processo %d: valor local = %d\n", rank, valor_local);

    // Usa MPI_Reduce para somar todos os valores locais e enviar o resultado para o processo 0
    MPI_Reduce(&valor_local,     // endereço do valor a ser enviado
               &soma_global,     // onde armazenar o resultado (no processo root)
               1,                // número de elementos (aqui, 1 inteiro)
               MPI_INT,          // tipo dos dados
               MPI_SUM,          // operação a ser realizada (soma)
               0,                // rank do processo root (que receberá o resultado)
               MPI_COMM_WORLD);  // comunicador

    // Apenas o processo root (0) imprime a soma final
    if (rank == 0) {
        printf("Processo %d: soma global = %d\n", rank, soma_global);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}