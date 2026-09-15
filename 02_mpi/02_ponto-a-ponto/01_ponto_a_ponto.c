#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {

    // Variável que armazena o identificador único do processo dentro do comunicador
    int rank;

    // Tag que identifica a mensagem, pode ser qualquer valor inteiro
    int tag = 24;

    // Buffer para armazenar a mensagem que será enviada/recebida
    char msg[20];

    // Variáveis para armazenar o resultado das operações MPI (envio e recebimento)
    int sent_result, received_result;

    // Objeto que guarda informações de status de mensagens recebidas
    MPI_Status status;

    // Inicializa o conteúdo da mensagem que será enviada
    strcpy(msg, "Java é mais legal");

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o identificador único do processo atual dentro do comunicador global (MPI_COMM_WORLD)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Processo com rank 0 executa esta parte do código
    if (rank == 0) {

        // Processo 0 envia a mensagem para o Processo 1
        sent_result = MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);

        // Verifica se houve erro no envio
        if (sent_result != MPI_SUCCESS) {
            fprintf(stderr, "MPI_Send erro. Código %d\n", sent_result);
            MPI_Abort(MPI_COMM_WORLD, 1);  // Aborta todos os processos MPI
        } else {
            printf("Processo 0 enviou a mensagem - %s - ao processo 1\n", msg);
        }
    }

    // Processo com rank 1 executa esta parte do código
    else if (rank == 1) {

        // Processo 1 recebe a mensagem enviada pelo Processo 0
        received_result = MPI_Recv(msg, sizeof(msg), MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

        // Verifica se houve erro no recebimento
        if (received_result != MPI_SUCCESS) {
            fprintf(stderr, "MPI_Recv erro. Código: %d\n", received_result);
            MPI_Abort(MPI_COMM_WORLD, 1);  // Aborta todos os processos MPI
        } else {
            // Verifica o tamanho exato da mensagem recebida
            int received_size;
            MPI_Get_count(&status, MPI_CHAR, &received_size);

            printf("Processo 1 recebeu a mensagem - %s - de tamanho %d do processo 0\n", msg, received_size);
        }
    }

    // Finaliza o ambiente MPI, encerrando a comunicação
    MPI_Finalize();

    return 0;
}
