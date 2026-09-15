#include "mpi.h"     // Biblioteca para programação paralela com MPI
#include <stdio.h>   // Para entrada e saída
#include <math.h>    // Para função fabs (valor absoluto)

int main(int argc, char* argv[]) {

    int id, num_processes;       // id: rank do processo atual | num_processes: total de processos
    long n, i;                    // n: número de intervalos | i: contador de loop
    double PI25DT = 3.141592653589793238462643; // Valor conhecido de pi para cálculo do erro
    double h, mypi, pi, sum, x; // h: tamanho do passo | mypi: π local | pi: π final | sum: soma parcial | x: ponto médio

    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &id);            // Obtém o rank (id) do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes); // Obtém o número total de processos

    // Processo 0 lê o número de intervalos (n) a partir do teclado
    if (id == 0) {
        printf("Enter the number of intervals n: ");
        scanf("%ld", &n);
    }

    // O valor de n é compartilhado com todos os processos usando broadcast
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h = 1.0 / (double) n; // Tamanho de cada subintervalo
    sum = 0.0;

    // Cada processo calcula sua soma parcial usando a regra do ponto médio
    for (i = id + 1; i <= n; i += num_processes) {
        x = h * ((double)i - 0.5); // Calcula o ponto médio do intervalo
        sum += 4.0 / (1.0 + x * x); // Soma o valor da função f(x) = 4 / (1 + x²)
    }

    mypi = h * sum; // Calcula o valor local de π multiplicando a soma pelo tamanho do passo

    // Reduz (soma) os resultados de todos os processos no processo 0
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Apenas o processo 0 imprime o resultado final e o erro
    if (id == 0) {
        printf("PI is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}

// compilar com mpicc -o pi_mpi pi_mpi.c
// exructar com mpirun -np 4 ./pi_mpi 