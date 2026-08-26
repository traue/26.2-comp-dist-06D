/*
 * Exemplo 08: Ponteiros Básico
 * Introdução a ponteiros: declaração, operadores & e *
 */

#include <stdio.h>

int main() {
    // Variável normal
    int numero = 42;
    
    // Declaração de um ponteiro para int
    // O * na declaração indica que é um ponteiro
    int *ptr;
    
    // Atribuindo o endereço de 'numero' ao ponteiro
    // O operador & retorna o endereço de memória
    ptr = &numero;
    
    printf("=== Conceitos Básicos ===\n");
    printf("Valor de numero: %d\n", numero);
    printf("Endereço de numero (&numero): %p\n", (void*)&numero);
    printf("Valor de ptr (endereço armazenado): %p\n", (void*)ptr);
    printf("Valor apontado por ptr (*ptr): %d\n", *ptr);
    
    // Modificando o valor através do ponteiro
    printf("\n=== Modificando através do ponteiro ===\n");
    printf("Antes: numero = %d\n", numero);
    *ptr = 100;  // Muda o valor na posição de memória
    printf("Depois de *ptr = 100: numero = %d\n", numero);
    
    // Ponteiros para diferentes tipos
    printf("\n=== Ponteiros para diferentes tipos ===\n");
    
    float decimal = 3.14;
    float *ptr_float = &decimal;
    printf("decimal = %.2f, *ptr_float = %.2f\n", decimal, *ptr_float);
    
    char letra = 'A';
    char *ptr_char = &letra;
    printf("letra = %c, *ptr_char = %c\n", letra, *ptr_char);
    
    double grande = 123456.789;
    double *ptr_double = &grande;
    printf("grande = %.3f, *ptr_double = %.3f\n", grande, *ptr_double);
    
    // Tamanho dos ponteiros
    printf("\n=== Tamanho dos ponteiros ===\n");
    printf("sizeof(int*) = %lu bytes\n", sizeof(int*));
    printf("sizeof(float*) = %lu bytes\n", sizeof(float*));
    printf("sizeof(char*) = %lu bytes\n", sizeof(char*));
    printf("sizeof(double*) = %lu bytes\n", sizeof(double*));
    printf("(Todos têm o mesmo tamanho - armazenam endereços)\n");
    
    // Ponteiro NULL
    printf("\n=== Ponteiro NULL ===\n");
    int *ptr_nulo = NULL;
    printf("ptr_nulo = %p\n", (void*)ptr_nulo);
    
    if (ptr_nulo == NULL) {
        printf("Ponteiro é NULL - não aponta para nada válido\n");
    }
    
    // Verificação antes de usar
    if (ptr != NULL) {
        printf("ptr é válido, valor = %d\n", *ptr);
    }
    
    return 0;
}
