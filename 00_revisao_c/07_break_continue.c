/*
 * Exemplo 07: Break e Continue
 * Controlando o fluxo de loops
 */

#include <stdio.h>

int main() {
    // BREAK - interrompe o loop completamente
    printf("=== BREAK: Parar quando encontrar 5 ===\n");
    for (int i = 1; i <= 10; i++) {
        if (i == 5) {
            printf("Encontrou 5, parando!\n");
            break;  // Sai do loop
        }
        printf("%d ", i);
    }
    printf("\n");
    
    // CONTINUE - pula para próxima iteração
    printf("\n=== CONTINUE: Pular números pares ===\n");
    for (int i = 1; i <= 10; i++) {
        if (i % 2 == 0) {
            continue;  // Pula para próxima iteração
        }
        printf("%d ", i);
    }
    printf("\n");
    
    // BREAK em busca linear
    printf("\n=== BREAK: Busca em vetor ===\n");
    int numeros[] = {10, 25, 30, 45, 50, 65, 70};
    int tamanho = 7;
    int busca = 45;
    int encontrado = -1;
    
    for (int i = 0; i < tamanho; i++) {
        printf("Verificando posição %d: %d\n", i, numeros[i]);
        if (numeros[i] == busca) {
            encontrado = i;
            break;  // Não precisa continuar buscando
        }
    }
    
    if (encontrado != -1) {
        printf("Valor %d encontrado na posição %d\n", busca, encontrado);
    } else {
        printf("Valor %d não encontrado\n", busca);
    }
    
    // CONTINUE para filtrar dados
    printf("\n=== CONTINUE: Filtrar negativos ===\n");
    int valores[] = {5, -3, 8, -1, 10, -7, 3};
    int soma = 0;
    
    printf("Somando apenas positivos: ");
    for (int i = 0; i < 7; i++) {
        if (valores[i] < 0) {
            continue;  // Ignora negativos
        }
        printf("%d ", valores[i]);
        soma += valores[i];
    }
    printf("\nSoma = %d\n", soma);
    
    // BREAK em loops aninhados (afeta apenas o loop interno)
    printf("\n=== BREAK em loops aninhados ===\n");
    for (int i = 1; i <= 3; i++) {
        printf("Loop externo i = %d: ", i);
        for (int j = 1; j <= 5; j++) {
            if (j == 3) {
                break;  // Sai apenas do loop interno
            }
            printf("j=%d ", j);
        }
        printf("\n");
    }
    
    // WHILE com BREAK
    printf("\n=== WHILE com BREAK ===\n");
    int n = 1;
    while (1) {  // Loop "infinito"
        printf("%d ", n);
        if (n >= 5) {
            break;  // Condição de saída dentro do loop
        }
        n++;
    }
    printf("\nLoop encerrado\n");
    
    return 0;
}
