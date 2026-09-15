/*
 * pingpong_latencia.c
 *
 * Objetivo: medir a latência de comunicação entre dois processos MPI.
 *
 * Método ping-pong:
 *   - O processo 0 envia uma mensagem mínima (1 byte) ao processo 1.
 *   - O processo 1 devolve imediatamente a mesma mensagem (eco).
 *   - Repete-se ITERACOES vezes e mede-se o tempo total.
 *   - RTT (Round-Trip Time) = tempo_total / iterações.
 *   - Latência ≈ RTT / 2  (tempo de apenas um sentido).
 *
 * Por que usar mensagem mínima?
 *   Com 1 byte o tempo de transmissão dos dados é desprezível, então o que
 *   medimos é essencialmente o overhead de inicializar e entregar uma mensagem
 *   — ou seja, a latência da rede/software MPI.
 */

#include <mpi.h>
#include <stdio.h>
#include <string.h>

/* Número de repetições do ping-pong; mais iterações → média mais estável. */
#define ITERACOES 10000

int main(int argc, char **argv) {
    /*
     * MPI_Init(argc, argv)
     *   Inicializa o ambiente MPI. Deve ser a primeira chamada MPI.
     *   Parâmetros:
     *     &argc, &argv — ponteiros para argumentos da linha de comando; o MPI
     *                    pode consumir argumentos próprios desse vetor.
     */
    MPI_Init(&argc, &argv);

    int rank, size;

    /*
     * MPI_Comm_rank(communicator, &rank)
     *   Retorna o rank (índice) do processo atual no comunicador.
     *   Parâmetros:
     *     MPI_COMM_WORLD — comunicador global com todos os processos;
     *     &rank          — recebe o rank deste processo (0 a size-1).
     */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*
     * MPI_Comm_size(communicator, &size)
     *   Retorna o número total de processos no comunicador.
     *   Parâmetros:
     *     MPI_COMM_WORLD — comunicador global;
     *     &size          — recebe o total de processos.
     */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Este experimento requer exatamente 2 processos (remetente e receptor). */
    if (size != 2) {
        if (rank == 0) {
            fprintf(stderr, "Este programa exige exatamente 2 processos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    /*
     * MPI_Get_processor_name(name, &resultlen)
     *   Retorna o nome do nó onde este processo está rodando.
     *   Parâmetros:
     *     mpi_name   — buffer de saída (≥ MPI_MAX_PROCESSOR_NAME bytes);
     *     &name_len  — comprimento efetivo da string retornada.
     */
    char mpi_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(mpi_name, &name_len);
    printf("Rank %d em %s\n", rank, mpi_name);
    fflush(stdout);

    /* Payload mínimo: 1 byte. O conteúdo não importa para medir latência. */
    char buffer = 0;
    int parceiro = (rank == 0) ? 1 : 0;   /* cada processo sabe quem é o outro */

    /*
     * MPI_Barrier(communicator)
     *   Ponto de sincronização global: nenhum processo avança além desta linha
     *   até que TODOS os processos do comunicador tenham chegado aqui.
     *   Parâmetros:
     *     MPI_COMM_WORLD — comunicador no qual todos devem se sincronizar.
     *   Por que usar? Garante que o cronômetro comece ao mesmo tempo nos dois
     *   processos, evitando que um inicie antes do outro estar pronto.
     */
    MPI_Barrier(MPI_COMM_WORLD);

    /*
     * MPI_Wtime()
     *   Retorna o tempo decorrido em segundos (ponto flutuante de alta
     *   precisão) desde um instante de referência arbitrário do processo.
     *   Sem parâmetros. Usada em par (fim - inicio) para medir intervalos.
     */
    double inicio = MPI_Wtime();

    for (int i = 0; i < ITERACOES; i++) {
        if (rank == 0) {
            /*
             * MPI_Send(buf, count, datatype, dest, tag, communicator)
             *   Envio bloqueante: retorna apenas quando o buffer pode ser
             *   reutilizado (a mensagem foi copiada ou enviada).
             *   Parâmetros:
             *     &buffer        — endereço dos dados a enviar;
             *     1              — número de elementos;
             *     MPI_CHAR       — tipo de cada elemento (1 byte);
             *     parceiro       — rank do processo destino;
             *     0              — tag (rótulo) da mensagem; usada para
             *                      diferenciar mensagens no receptor;
             *     MPI_COMM_WORLD — comunicador utilizado.
             */
            MPI_Send(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);

            /*
             * MPI_Recv(buf, count, datatype, source, tag, communicator, status)
             *   Recepção bloqueante: aguarda até que uma mensagem compatível
             *   chegue e a copia para o buffer.
             *   Parâmetros:
             *     &buffer          — buffer onde a mensagem será armazenada;
             *     1                — capacidade máxima em elementos;
             *     MPI_CHAR         — tipo esperado;
             *     parceiro         — rank do remetente (ou MPI_ANY_SOURCE);
             *     0                — tag esperada (ou MPI_ANY_TAG);
             *     MPI_COMM_WORLD   — comunicador;
             *     MPI_STATUS_IGNORE — ignoramos metadados da mensagem recebida.
             */
            MPI_Recv(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        } else {
            /* Processo 1: primeiro recebe, depois devolve (eco). */
            MPI_Recv(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Send(&buffer, 1, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);
        }
    }

    double fim = MPI_Wtime();

    /* Apenas o processo 0 tem a visão completa do RTT (enviou e recebeu). */
    if (rank == 0) {
        double tempo_total = fim - inicio;
        double rtt_us = (tempo_total / ITERACOES) * 1e6;   /* RTT em microssegundos */
        double latencia_us = rtt_us / 2.0;                  /* um sentido = RTT / 2  */

        printf("\n--- Resultado ---\n");
        printf("Iteracoes:            %d\n", ITERACOES);
        printf("Tempo total:          %.3f s\n", tempo_total);
        printf("RTT medio:            %.3f us\n", rtt_us);
        printf("Latencia (RTT/2):     %.3f us\n", latencia_us);
    }

    /*
     * MPI_Finalize()
     *   Encerra o ambiente MPI e libera todos os recursos internos.
     *   Nenhuma função MPI pode ser chamada depois desta.
     */
    MPI_Finalize();
    return 0;
}
