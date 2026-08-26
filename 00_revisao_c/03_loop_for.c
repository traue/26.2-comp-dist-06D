/*
 * Exemplo 03: Loop FOR
 * Estrutura de repetição com contador
 */

#include <stdio.h>

int main() {
    // Loop FOR básico - contando de 1 a 10
    printf("=== Contando de 1 a 10 ===\n");
    for (int i = 1; i <= 10; i++) {
        printf("%d ", i);
    }
    printf("\n");
    
    // Loop FOR decrescente
    printf("\n=== Contagem regressiva ===\n");
    for (int i = 10; i >= 1; i--) {
        printf("%d ", i);
    }
    printf("\n");
    
    // Loop FOR com incremento diferente
    printf("\n=== Números pares de 0 a 20 ===\n");
    for (int i = 0; i <= 20; i += 2) {
        printf("%d ", i);
    }
    printf("\n");
    
    // Loop FOR para percorrer vetor
    int valores[5] = {100, 200, 300, 400, 500};
    printf("\n=== Percorrendo vetor com FOR ===\n");
    for (int i = 0; i < 5; i++) {
        printf("valores[%d] = %d\n", i, valores[i]);
    }
    
    // Loop FOR com múltiplas variáveis
    printf("\n=== FOR com múltiplas variáveis ===\n");
    for (int i = 0, j = 10; i < j; i++, j--) {
        printf("i = %d, j = %d\n", i, j);
    }
    
    // Tabuada usando FOR
    int numero = 7;
    printf("\n=== Tabuada do %d ===\n", numero);
    for (int i = 1; i <= 10; i++) {
        printf("%d x %d = %d\n", numero, i, numero * i);
    }
    
    return 0;
}
