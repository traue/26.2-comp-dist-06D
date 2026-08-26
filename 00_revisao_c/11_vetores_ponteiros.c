/*
 * Exemplo 11: Vetores e Ponteiros
 * Relação entre vetores e ponteiros em C
 */

#include <stdio.h>

int main() {
    int numeros[] = {10, 20, 30, 40, 50};
    int *ptr;
    
    // Nome do vetor é um ponteiro para o primeiro elemento
    printf("=== Vetor é um ponteiro constante ===\n");
    printf("numeros (endereço do vetor): %p\n", (void*)numeros);
    printf("&numeros[0] (endereço do primeiro elemento): %p\n", (void*)&numeros[0]);
    printf("São iguais? %s\n", numeros == &numeros[0] ? "Sim" : "Não");
    
    // Atribuindo vetor a ponteiro
    ptr = numeros;  // Equivalente a: ptr = &numeros[0]
    
    printf("\n=== Diferentes formas de acessar elementos ===\n");
    printf("Índice | numeros[i] | *(numeros+i) | ptr[i] | *(ptr+i)\n");
    printf("-------|------------|--------------|--------|--------\n");
    for (int i = 0; i < 5; i++) {
        printf("   %d   |     %2d     |      %2d      |   %2d   |   %2d\n",
               i, numeros[i], *(numeros + i), ptr[i], *(ptr + i));
    }
    
    // Modificando vetor através de ponteiro
    printf("\n=== Modificando através de ponteiro ===\n");
    printf("Antes: ");
    for (int i = 0; i < 5; i++) printf("%d ", numeros[i]);
    printf("\n");
    
    *(ptr + 2) = 300;    // Muda numeros[2]
    ptr[4] = 500;        // Muda numeros[4]
    
    printf("Depois: ");
    for (int i = 0; i < 5; i++) printf("%d ", numeros[i]);
    printf("\n");
    
    // Percorrendo com ponteiro
    printf("\n=== Percorrendo com incremento de ponteiro ===\n");
    ptr = numeros;
    int *fim = numeros + 5;
    
    printf("Elementos: ");
    while (ptr < fim) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    // Notação de índice vs notação de ponteiro
    printf("\n=== Equivalências ===\n");
    printf("numeros[0] é equivalente a *numeros = %d\n", *numeros);
    printf("numeros[1] é equivalente a *(numeros+1) = %d\n", *(numeros+1));
    printf("&numeros[2] é equivalente a numeros+2 = %p\n", (void*)(numeros+2));
    
    // IMPORTANTE: Diferença entre vetor e ponteiro
    printf("\n=== Diferença importante ===\n");
    printf("sizeof(numeros) = %lu (tamanho total do vetor)\n", sizeof(numeros));
    
    int *p = numeros;
    printf("sizeof(p) = %lu (tamanho do ponteiro)\n", sizeof(p));
    
    printf("Número de elementos = sizeof(numeros)/sizeof(int) = %lu\n",
           sizeof(numeros) / sizeof(int));
    
    // Vetor de caracteres e ponteiros
    printf("\n=== String como ponteiro ===\n");
    char texto[] = "Hello";
    char *str = texto;
    
    printf("String: %s\n", str);
    printf("Caracteres: ");
    while (*str != '\0') {
        printf("'%c' ", *str);
        str++;
    }
    printf("\n");
    
    return 0;
}
