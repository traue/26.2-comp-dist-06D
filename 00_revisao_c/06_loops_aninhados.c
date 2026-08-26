/*
 * Exemplo 06: Loops Aninhados
 * Combinando múltiplas estruturas de repetição
 */

#include <stdio.h>

int main() {
    // Loops aninhados - padrão retangular
    printf("=== Padrão Retangular ===\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            printf("* ");
        }
        printf("\n");
    }
    
    // Triângulo crescente
    printf("\n=== Triângulo Crescente ===\n");
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
    
    // Triângulo decrescente
    printf("\n=== Triângulo Decrescente ===\n");
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
    
    // Preenchendo matriz com loops aninhados
    printf("\n=== Matriz de Multiplicação ===\n");
    int matriz[4][4];
    
    // Preenchendo
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matriz[i][j] = (i + 1) * (j + 1);
        }
    }
    
    // Imprimindo cabeçalho
    printf("    ");
    for (int j = 1; j <= 4; j++) {
        printf("%3d ", j);
    }
    printf("\n    ");
    for (int j = 0; j < 4; j++) {
        printf("----");
    }
    printf("\n");
    
    // Imprimindo matriz
    for (int i = 0; i < 4; i++) {
        printf("%d | ", i + 1);
        for (int j = 0; j < 4; j++) {
            printf("%3d ", matriz[i][j]);
        }
        printf("\n");
    }
    
    // Três níveis de aninhamento
    printf("\n=== Três Níveis de Loop ===\n");
    for (int i = 1; i <= 2; i++) {
        printf("i = %d\n", i);
        for (int j = 1; j <= 2; j++) {
            printf("  j = %d\n", j);
            for (int k = 1; k <= 2; k++) {
                printf("    k = %d\n", k);
            }
        }
    }
    
    return 0;
}
