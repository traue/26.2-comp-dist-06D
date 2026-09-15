/*
 * hello_hostname.c
 *
 * Objetivo: demonstrar como identificar em qual máquina física cada processo
 * MPI está rodando. Útil para verificar que o hostfile foi lido corretamente
 * e que os processos foram distribuídos entre os nós da rede.
 */

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    /*
     * MPI_Init(argc, argv)
     *   Inicializa o ambiente MPI. Deve ser a primeira chamada MPI do programa.
     *   Parâmetros:
     *     &argc, &argv  — ponteiros para os argumentos da linha de comando;
     *                     o MPI pode remover seus próprios argumentos desse vetor.
     */
    MPI_Init(&argc, &argv);

    int rank, size;

    /*
     * MPI_Comm_rank(communicator, &rank)
     *   Descobre o identificador (rank) do processo atual dentro do comunicador.
     *   Parâmetros:
     *     MPI_COMM_WORLD  — comunicador global que engloba todos os processos;
     *     &rank           — variável que receberá o rank (0, 1, 2, ..., size-1).
     */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*
     * MPI_Comm_size(communicator, &size)
     *   Descobre o número total de processos no comunicador.
     *   Parâmetros:
     *     MPI_COMM_WORLD  — comunicador global;
     *     &size           — variável que receberá o total de processos.
     */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* gethostname: função POSIX que retorna o nome do host do sistema operacional. */
    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    /*
     * MPI_Get_processor_name(name, &resultlen)
     *   Retorna o nome do processador/nó segundo a implementação MPI.
     *   Em geral é igual ao hostname, mas pode incluir informações extras.
     *   Parâmetros:
     *     mpi_name   — buffer de saída (tamanho mínimo: MPI_MAX_PROCESSOR_NAME);
     *     &name_len  — comprimento real da string retornada.
     */
    int name_len;
    char mpi_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(mpi_name, &name_len);

    /* Cada processo imprime suas próprias informações de localização. */
    printf("Processo %d de %d rodando em %s (MPI: %s)\n",
           rank, size, hostname, mpi_name);

    /*
     * MPI_Finalize()
     *   Encerra o ambiente MPI, libera recursos e sincroniza a saída.
     *   Deve ser a última chamada MPI do programa. Nenhuma função MPI pode
     *   ser chamada após MPI_Finalize().
     */
    MPI_Finalize();
    return 0;
}
