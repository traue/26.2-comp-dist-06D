/*
 * Exemplo 02: Vetores Multidimensionais (Matrizes)
 * Declaração e manipulação de matrizes em C
 */

#include <stdio.h>

int main() {
    // Declaração de uma matriz 3x3
    int matriz[3][3];
    
    // Preenchendo a matriz
    int valor = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matriz[i][j] = valor;
            valor++;
        }
    }
    
    // Imprimindo a matriz
    printf("=== Matriz 3x3 ===\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    
    // Matriz inicializada diretamente
    int tabela[2][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    };
    
    printf("\n=== Matriz 2x4 Inicializada ===\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", tabela[i][j]);
        }
        printf("\n");
    }
    
    // Soma dos elementos da diagonal principal
    printf("\n=== Diagonal Principal da Matriz 3x3 ===\n");
    int soma = 0;
    for (int i = 0; i < 3; i++) {
        printf("matriz[%d][%d] = %d\n", i, i, matriz[i][i]);
        soma += matriz[i][i];
    }
    printf("Soma da diagonal: %d\n", soma);
    
    return 0;
}
