/*
 * Exemplo 14: Matriz Dinâmica com Ponteiros de Ponteiros
 * Alocação de matriz 2D usando **
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
    int **matriz;
    int linhas = 3;
    int colunas = 4;
    
    printf("=== Alocação de Matriz Dinâmica %dx%d ===\n", linhas, colunas);
    
    // Passo 1: Alocar vetor de ponteiros (linhas)
    matriz = (int**) malloc(linhas * sizeof(int*));
    
    if (matriz == NULL) {
        printf("Erro na alocação!\n");
        return 1;
    }
    
    // Passo 2: Alocar cada linha
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int*) malloc(colunas * sizeof(int));
        
        if (matriz[i] == NULL) {
            printf("Erro na alocação da linha %d!\n", i);
            return 1;
        }
    }
    
    printf("Memória alocada com sucesso!\n\n");
    
    // Estrutura na memória
    printf("=== Estrutura na Memória ===\n");
    printf("matriz (%p) --> [linha0 | linha1 | linha2]\n", (void*)matriz);
    for (int i = 0; i < linhas; i++) {
        printf("  matriz[%d] = %p --> [col0 | col1 | col2 | col3]\n", 
               i, (void*)matriz[i]);
    }
    
    // Preenchendo a matriz
    printf("\n=== Preenchendo a Matriz ===\n");
    int contador = 1;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = contador++;
        }
    }
    
    // Imprimindo a matriz
    printf("Conteúdo da matriz:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%3d ", matriz[i][j]);
        }
        printf("\n");
    }
    
    // Acessando com notação de ponteiro
    printf("\n=== Acesso com Notação de Ponteiro ===\n");
    printf("matriz[1][2] = %d\n", matriz[1][2]);
    printf("*(*(matriz + 1) + 2) = %d\n", *(*(matriz + 1) + 2));
    printf("(São equivalentes)\n");
    
    // Soma dos elementos
    printf("\n=== Soma dos Elementos ===\n");
    int soma = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            soma += matriz[i][j];
        }
    }
    printf("Soma total: %d\n", soma);
    
    // IMPORTANTE: Liberando memória na ordem correta
    printf("\n=== Liberando Memória ===\n");
    
    // Primeiro: liberar cada linha
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
        printf("Liberado matriz[%d]\n", i);
    }
    
    // Depois: liberar o vetor de ponteiros
    free(matriz);
    printf("Liberado matriz\n");
    
    matriz = NULL;  // Boa prática
    
    printf("\nMemória liberada corretamente!\n");
    
    // Exemplo com matriz de tamanho variável
    printf("\n=== Matriz com Linhas de Tamanhos Diferentes ===\n");
    
    int num_linhas = 4;
    int tamanhos[] = {2, 4, 3, 5};  // Cada linha tem tamanho diferente
    
    int **triangular = (int**) malloc(num_linhas * sizeof(int*));
    
    for (int i = 0; i < num_linhas; i++) {
        triangular[i] = (int*) malloc(tamanhos[i] * sizeof(int));
        
        // Preencher com índice da linha
        for (int j = 0; j < tamanhos[i]; j++) {
            triangular[i][j] = i + 1;
        }
    }
    
    printf("Matriz com linhas de tamanhos diferentes:\n");
    for (int i = 0; i < num_linhas; i++) {
        printf("Linha %d (%d elementos): ", i, tamanhos[i]);
        for (int j = 0; j < tamanhos[i]; j++) {
            printf("%d ", triangular[i][j]);
        }
        printf("\n");
    }
    
    // Liberar
    for (int i = 0; i < num_linhas; i++) {
        free(triangular[i]);
    }
    free(triangular);
    
    return 0;
}
