#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Para usar sleep()

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);  // Inicializa o ambiente MPI

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtém o rank (ID) do processo atual

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtém o número total de processos

    if (size < 2) {
        // Garante que há pelo menos dois processos para a comunicação
        fprintf(stderr, "We must have at least 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int data_size = 10;  // Tamanho do vetor de dados
    int data[data_size];       // Vetor a ser enviado
    MPI_Request request;       // Representa a requisição de envio
    MPI_Status status;         // Para verificar o status da operação

    if (rank == 0) {
        // Inicializa o vetor com valores de 0 a 9
        for (int i = 0; i < data_size; ++i) {
            data[i] = i;
        }

        // Inicia o envio não bloqueante para o processo 1
        MPI_Isend(data, data_size, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        // Decide cancelar a operação de envio
        MPI_Cancel(&request);

        // Aguarda a finalização da tentativa de cancelamento
        MPI_Wait(&request, &status);

        // Verifica se o envio foi realmente cancelado
        int flag;
        MPI_Test_cancelled(&status, &flag);

        if (flag) {
            printf("Send operation was successfully cancelled.\n");
        } else {
            printf("Send operation was not cancelled.\n");
        }
    }

    else if (rank == 1) {
        sleep(1);  // Aguarda um tempo antes de tentar receber (para permitir o cancelamento)
        int recv_buff[data_size];     // Buffer de recepção
        MPI_Status recv_status;

        // Tenta receber a mensagem do processo 0
        int recv_result = MPI_Recv(recv_buff, data_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &recv_status);

        if (recv_result == MPI_SUCCESS) {
            printf("Process 1 received data.\n");
        } else {
            printf("Process 0 cancelled the data.\n");
        }
    }

    MPI_Finalize();  // Finaliza o ambiente MPI

    return 0;
}


// compilar: mpicc -o mpi_cancel mpi_cancel.c

// executar: mpirun -np 2 ./mpi_cancel