/*
 * pingpong_banda.c
 *
 * Objetivo: medir a largura de banda (throughput) de comunicação entre dois
 * processos MPI em função do tamanho da mensagem.
 *
 * Método:
 *   Para cada tamanho de mensagem (de 1 B até 4 MB, dobrando a cada passo):
 *     1. O processo 0 envia o buffer ao processo 1.
 *     2. O processo 1 devolve o mesmo buffer (eco).
 *     3. Repete-se ITERACOES vezes e mede-se o tempo total.
 *     4. Banda = bytes trafegados / tempo total.
 *
 * Interpretação dos resultados:
 *   - Para mensagens pequenas, a latência domina → banda aparente é baixa.
 *   - Para mensagens grandes, o canal é utilizado plenamente → banda se aproxima
 *     da capacidade física do enlace (ex.: 1 Gbps ≈ 125 MB/s).
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Número de ping-pongs por tamanho de mensagem. */
#define ITERACOES   1000
/* Tamanho inicial do buffer (bytes). */
#define TAMANHO_MIN 1
/* Tamanho máximo do buffer: 4 MB. */
#define TAMANHO_MAX (4 * 1024 * 1024)

int main(int argc, char **argv) {
    /*
     * MPI_Init(argc, argv)
     *   Inicializa o ambiente MPI. Deve ser a primeira chamada MPI.
     *   Parâmetros:
     *     &argc, &argv — ponteiros para argumentos da linha de comando.
     */
    MPI_Init(&argc, &argv);

    int rank, size;

    /*
     * MPI_Comm_rank(communicator, &rank)
     *   Retorna o rank do processo atual dentro do comunicador.
     *   Parâmetros:
     *     MPI_COMM_WORLD — comunicador global;
     *     &rank          — recebe o rank deste processo.
     */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*
     * MPI_Comm_size(communicator, &size)
     *   Retorna o total de processos no comunicador.
     *   Parâmetros:
     *     MPI_COMM_WORLD — comunicador global;
     *     &size          — recebe o total de processos.
     */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Experimento requer exatamente 2 processos: remetente e receptor. */
    if (size != 2) {
        if (rank == 0) {
            fprintf(stderr, "Este programa exige exatamente 2 processos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    /* Aloca o maior buffer possível; os testes menores usam apenas os primeiros bytes. */
    char *buffer = (char *)malloc(TAMANHO_MAX);
    if (!buffer) {
        fprintf(stderr, "Falha ao alocar buffer.\n");
        /*
         * MPI_Abort(communicator, errorcode)
         *   Termina todos os processos do comunicador com um código de erro.
         *   Parâmetros:
         *     MPI_COMM_WORLD — comunicador cujos processos serão encerrados;
         *     1              — código de erro devolvido ao sistema operacional.
         *   Diferente de MPI_Finalize: aborta imediatamente sem sincronização.
         */
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    memset(buffer, 0, TAMANHO_MAX);

    int parceiro = (rank == 0) ? 1 : 0;

    /* Cabeçalho da tabela de resultados (impresso apenas pelo rank 0). */
    if (rank == 0) {
        printf("%12s %12s %15s %15s\n",
               "Tamanho (B)", "Iter", "Tempo (s)", "Banda (MB/s)");
        printf("--------------------------------------------------------\n");
    }

    /* Varre tamanhos de mensagem em progressão geométrica de razão 2. */
    for (int tam = TAMANHO_MIN; tam <= TAMANHO_MAX; tam *= 2) {
        /*
         * MPI_Barrier(communicator)
         *   Sincroniza todos os processos antes de cada medição, garantindo
         *   que o cronômetro inicie junto nos dois lados.
         *   Parâmetros:
         *     MPI_COMM_WORLD — comunicador a sincronizar.
         */
        MPI_Barrier(MPI_COMM_WORLD);

        /*
         * MPI_Wtime()
         *   Retorna o tempo atual em segundos (alta precisão).
         *   Sem parâmetros. Usada em par (fim - inicio) para medir intervalos.
         */
        double inicio = MPI_Wtime();

        for (int i = 0; i < ITERACOES; i++) {
            if (rank == 0) {
                /*
                 * MPI_Send(buf, count, datatype, dest, tag, communicator)
                 *   Envio bloqueante de `count` elementos do tipo `datatype`.
                 *   Parâmetros:
                 *     buffer         — dados a enviar;
                 *     tam            — número de bytes (elementos MPI_CHAR);
                 *     MPI_CHAR       — tipo de cada elemento (1 byte);
                 *     parceiro       — rank do destino;
                 *     0              — tag para identificar a mensagem;
                 *     MPI_COMM_WORLD — comunicador.
                 */
                MPI_Send(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);

                /*
                 * MPI_Recv(buf, count, datatype, source, tag, communicator, status)
                 *   Recepção bloqueante: aguarda mensagem compatível e a armazena.
                 *   Parâmetros:
                 *     buffer           — onde armazenar os dados recebidos;
                 *     tam              — capacidade máxima em elementos;
                 *     MPI_CHAR         — tipo esperado;
                 *     parceiro         — rank do remetente;
                 *     0                — tag esperada;
                 *     MPI_COMM_WORLD   — comunicador;
                 *     MPI_STATUS_IGNORE — metadados da mensagem não nos interessam.
                 */
                MPI_Recv(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
            } else {
                /* Processo 1 atua como espelho: recebe e devolve. */
                MPI_Recv(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
                MPI_Send(buffer, tam, MPI_CHAR, parceiro, 0, MPI_COMM_WORLD);
            }
        }

        double fim = MPI_Wtime();

        if (rank == 0) {
            double tempo_total = fim - inicio;
            /* Cada iteração transfere `tam` bytes em cada sentido → 2 * tam por iteração. */
            double bytes_totais = 2.0 * ITERACOES * tam;
            double banda_mbs = (bytes_totais / tempo_total) / (1024.0 * 1024.0);
            printf("%12d %12d %15.6f %15.2f\n",
                   tam, ITERACOES, tempo_total, banda_mbs);
        }
    }

    free(buffer);

    /*
     * MPI_Finalize()
     *   Encerra o ambiente MPI e libera recursos. Última chamada MPI obrigatória.
     */
    MPI_Finalize();
    return 0;
}
