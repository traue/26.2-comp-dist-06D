/*
 * pi_montecarlo.c
 *
 * Objetivo: estimar o valor de π usando o método de Monte Carlo distribuído.
 *
 * Método:
 *   Sorteamos pontos aleatórios (x, y) dentro do quadrado [0,1] × [0,1].
 *   Se x² + y² ≤ 1, o ponto cai dentro do quarto de círculo de raio 1.
 *   A razão (pontos dentro / total) ≈ π/4  →  π ≈ 4 * (dentro / total).
 *
 * Distribuição do trabalho:
 *   Cada processo sorteia uma fatia do total de pontos de forma independente,
 *   depois MPI_Reduce soma os acertos no processo 0, que calcula o π final.
 *
 * Uso: mpirun -np <N> ./pi_montecarlo [total_de_pontos]
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    /*
     * MPI_Init(argc, argv)
     *   Inicializa o ambiente MPI. Deve ser a primeira chamada MPI.
     *   Parâmetros:
     *     &argc, &argv — ponteiros para os argumentos da linha de comando;
     *                    o MPI pode remover argumentos próprios desse vetor.
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

    /* Total de pontos: passado como argumento, ou padrão de 100 milhões. */
    long long total_pontos = 100000000LL;
    if (argc > 1) {
        total_pontos = atoll(argv[1]);
    }

    /*
     * MPI_Bcast(buf, count, datatype, root, communicator)
     *   Difunde (broadcast) dados do processo `root` para todos os demais.
     *   Parâmetros:
     *     &total_pontos  — buffer: no root contém o valor a enviar; nos demais
     *                      receberá o valor enviado pelo root;
     *     1              — número de elementos a transmitir;
     *     MPI_LONG_LONG  — tipo de cada elemento;
     *     0              — rank do processo raiz (quem envia);
     *     MPI_COMM_WORLD — comunicador.
     *   Por que usar? Garante que todos os processos usem o mesmo total_pontos,
     *   mesmo que o argumento só tenha sido lido pelo processo 0.
     */
    MPI_Bcast(&total_pontos, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    /* Divide os pontos igualmente; o rank 0 absorve a sobra da divisão inteira. */
    long long pontos_locais = total_pontos / size;
    if (rank == 0) {
        pontos_locais += total_pontos % size;
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
    printf("Rank %d em %s: sorteando %lld pontos\n",
           rank, mpi_name, pontos_locais);
    fflush(stdout);

    /*
     * Semente diferente por processo: se todos usassem time(NULL), gerariam
     * a mesma sequência aleatória, perdendo a independência estatística.
     * Multiplica o rank por um número primo para espaçar bem as sementes.
     */
    unsigned int seed = (unsigned int)(time(NULL) + rank * 7919);
    srand(seed);

    /*
     * MPI_Barrier(communicator)
     *   Ponto de sincronização: todos os processos esperam aqui até que o
     *   último chegue, garantindo que o cronômetro comece junto em todos.
     *   Parâmetros:
     *     MPI_COMM_WORLD — comunicador a sincronizar.
     */
    MPI_Barrier(MPI_COMM_WORLD);

    /*
     * MPI_Wtime()
     *   Retorna o tempo decorrido em segundos desde uma referência arbitrária.
     *   Sem parâmetros. Usada em par (fim - inicio) para medir intervalos.
     */
    double t_inicio = MPI_Wtime();

    /* Núcleo do Monte Carlo: sorteia pontos e conta os que caem no círculo. */
    long long dentro_local = 0;
    for (long long i = 0; i < pontos_locais; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        /* Verifica se o ponto (x, y) está dentro do quarto de círculo de raio 1. */
        if (x * x + y * y <= 1.0) {
            dentro_local++;
        }
    }

    double t_fim_calculo = MPI_Wtime();

    /*
     * MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, communicator)
     *   Aplica uma operação de redução em dados distribuídos pelos processos,
     *   coletando o resultado no processo `root`.
     *   Parâmetros:
     *     &dentro_local  — buffer de envio: valor local de cada processo;
     *     &dentro_total  — buffer de recepção: só é válido no root após a chamada;
     *     1              — número de elementos;
     *     MPI_LONG_LONG  — tipo de cada elemento;
     *     MPI_SUM        — operação a aplicar (soma todos os valores);
     *     0              — rank do processo que receberá o resultado;
     *     MPI_COMM_WORLD — comunicador.
     *   Resultado: dentro_total (no rank 0) = soma de dentro_local de todos os ranks.
     */
    long long dentro_total = 0;
    MPI_Reduce(&dentro_local, &dentro_total, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double t_fim_total = MPI_Wtime();

    /* Apenas o rank 0 possui dentro_total correto; os outros não imprimem. */
    if (rank == 0) {
        double pi_estimado = 4.0 * (double)dentro_total / (double)total_pontos;
        double erro = fabs(pi_estimado - M_PI);

        printf("\n--- Resultado ---\n");
        printf("Processos:            %d\n", size);
        printf("Pontos totais:        %lld\n", total_pontos);
        printf("Pontos dentro:        %lld\n", dentro_total);
        printf("Pi estimado:          %.10f\n", pi_estimado);
        printf("Pi real (M_PI):       %.10f\n", M_PI);
        printf("Erro absoluto:        %.2e\n", erro);
        printf("Tempo de calculo:     %.3f s\n", t_fim_calculo - t_inicio);
        printf("Tempo com reduce:     %.3f s\n", t_fim_total - t_inicio);
    }

    /*
     * MPI_Finalize()
     *   Encerra o ambiente MPI e libera todos os recursos.
     *   Nenhuma função MPI pode ser chamada depois desta.
     */
    MPI_Finalize();
    return 0;
}
