/*
 * Exemplo 13: Ponteiros de Ponteiros
 * Conceito de ponteiro que aponta para outro ponteiro
 */

#include <stdio.h>

int main() {
    int valor = 100;
    int *ptr;       // Ponteiro para int
    int **pptr;     // Ponteiro para ponteiro de int
    
    ptr = &valor;   // ptr armazena endereço de valor
    pptr = &ptr;    // pptr armazena endereço de ptr
    
    printf("=== Conceito Básico ===\n");
    printf("valor = %d\n", valor);
    printf("&valor (endereço de valor) = %p\n", (void*)&valor);
    printf("\n");
    
    printf("ptr = %p (endereço armazenado em ptr)\n", (void*)ptr);
    printf("*ptr = %d (valor apontado por ptr)\n", *ptr);
    printf("&ptr (endereço de ptr) = %p\n", (void*)&ptr);
    printf("\n");
    
    printf("pptr = %p (endereço armazenado em pptr = endereço de ptr)\n", (void*)pptr);
    printf("*pptr = %p (valor apontado por pptr = ptr)\n", (void*)*pptr);
    printf("**pptr = %d (valor final = valor)\n", **pptr);
    
    // Diagrama visual
    printf("\n=== Diagrama de Memória ===\n");
    printf("+---------+     +--------+     +--------+\n");
    printf("|  pptr   | --> |  ptr   | --> | valor  |\n");
    printf("|   %p  |     |  %p  |     |  %d   |\n", 
           (void*)pptr, (void*)ptr, valor);
    printf("+---------+     +--------+     +--------+\n");
    
    // Modificando valor através de pptr
    printf("\n=== Modificando através de ** ===\n");
    printf("Antes: valor = %d\n", valor);
    **pptr = 200;
    printf("Após **pptr = 200: valor = %d\n", valor);
    
    // Modificando ptr através de pptr
    int outro = 999;
    printf("\n=== Redirecionando ponteiro via ** ===\n");
    printf("outro = %d (em %p)\n", outro, (void*)&outro);
    
    *pptr = &outro;  // Agora ptr aponta para 'outro'
    
    printf("Após *pptr = &outro:\n");
    printf("*ptr = %d (agora aponta para 'outro')\n", *ptr);
    printf("**pptr = %d\n", **pptr);
    
    // Ponteiro triplo
    printf("\n=== Ponteiro Triplo ***ppptr ===\n");
    int numero = 42;
    int *p1 = &numero;
    int **p2 = &p1;
    int ***p3 = &p2;
    
    printf("numero = %d\n", numero);
    printf("*p1 = %d\n", *p1);
    printf("**p2 = %d\n", **p2);
    printf("***p3 = %d\n", ***p3);
    
    printf("\nNíveis de indireção:\n");
    printf("p3 --> p2 --> p1 --> numero\n");
    printf("%p --> %p --> %p --> %d\n", 
           (void*)p3, (void*)p2, (void*)p1, numero);
    
    return 0;
}
