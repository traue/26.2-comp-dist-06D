/*
 * Exemplo 05: Loop DO-WHILE
 * Estrutura de repetição com condição no final
 * Garante pelo menos uma execução do bloco
 */

#include <stdio.h>

int main() {
    // DO-WHILE básico
    printf("=== Contando de 1 a 5 ===\n");
    int i = 1;
    do {
        printf("%d ", i);
        i++;
    } while (i <= 5);
    printf("\n");
    
    // Diferença entre WHILE e DO-WHILE quando condição é falsa
    printf("\n=== Diferença WHILE vs DO-WHILE ===\n");
    
    int x = 10;
    printf("Valor de x = %d\n", x);
    
    printf("\nWHILE (x < 5) - não executa nada:\n");
    while (x < 5) {
        printf("Executou WHILE\n");
        x++;
    }
    printf("(nenhuma saída)\n");
    
    printf("\nDO-WHILE (x < 5) - executa pelo menos uma vez:\n");
    do {
        printf("Executou DO-WHILE (x = %d)\n", x);
        x++;
    } while (x < 5);
    
    // DO-WHILE para menu (simulado)
    printf("\n=== Simulação de Menu ===\n");
    int opcoes[] = {1, 2, 3, 0}; // Simulando escolhas do usuário
    int idx = 0;
    int opcao;
    
    do {
        printf("\n--- MENU ---\n");
        printf("1. Opção A\n");
        printf("2. Opção B\n");
        printf("3. Opção C\n");
        printf("0. Sair\n");
        
        opcao = opcoes[idx];
        printf("Opção escolhida: %d\n", opcao);
        
        switch (opcao) {
            case 1:
                printf("Você escolheu a Opção A\n");
                break;
            case 2:
                printf("Você escolheu a Opção B\n");
                break;
            case 3:
                printf("Você escolheu a Opção C\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        idx++;
    } while (opcao != 0);
    
    // DO-WHILE para calcular fatorial
    printf("\n=== Fatorial de 5 ===\n");
    int n = 5;
    int fatorial = 1;
    int contador = n;
    
    do {
        fatorial *= contador;
        printf("%d! parcial = %d\n", n - contador + 1, fatorial);
        contador--;
    } while (contador > 0);
    
    printf("Fatorial de %d = %d\n", n, fatorial);
    
    return 0;
}
