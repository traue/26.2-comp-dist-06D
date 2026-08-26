/*
 * Exemplo 09: Aritmética de Ponteiros
 * Operações matemáticas com ponteiros
 */

#include <stdio.h>

int main() {
    // Vetor para demonstração
    int numeros[] = {10, 20, 30, 40, 50};
    int *ptr = numeros;  // ptr aponta para o primeiro elemento
    
    printf("=== Acessando elementos com aritmética de ponteiros ===\n");
    printf("Endereço base (ptr): %p\n", (void*)ptr);
    printf("\n");
    
    // Acessando elementos
    printf("*ptr       = %d (elemento 0)\n", *ptr);
    printf("*(ptr + 1) = %d (elemento 1)\n", *(ptr + 1));
    printf("*(ptr + 2) = %d (elemento 2)\n", *(ptr + 2));
    printf("*(ptr + 3) = %d (elemento 3)\n", *(ptr + 3));
    printf("*(ptr + 4) = %d (elemento 4)\n", *(ptr + 4));
    
    // Endereços
    printf("\n=== Endereços dos elementos ===\n");
    for (int i = 0; i < 5; i++) {
        printf("ptr + %d = %p (diferença: %ld bytes)\n", 
               i, (void*)(ptr + i), (char*)(ptr + i) - (char*)ptr);
    }
    printf("(Cada int ocupa %lu bytes)\n", sizeof(int));
    
    // Incremento do ponteiro
    printf("\n=== Incrementando o ponteiro ===\n");
    int *p = numeros;
    printf("Inicial: p = %p, *p = %d\n", (void*)p, *p);
    
    p++;  // Avança para o próximo int
    printf("Após p++: p = %p, *p = %d\n", (void*)p, *p);
    
    p++;
    printf("Após p++: p = %p, *p = %d\n", (void*)p, *p);
    
    p--;  // Volta um int
    printf("Após p--: p = %p, *p = %d\n", (void*)p, *p);
    
    // Percorrendo vetor com ponteiro
    printf("\n=== Percorrendo vetor com ponteiro ===\n");
    int *inicio = numeros;
    int *fim = numeros + 5;  // Um após o último elemento
    
    printf("Elementos: ");
    while (inicio < fim) {
        printf("%d ", *inicio);
        inicio++;
    }
    printf("\n");
    
    // Diferença entre ponteiros
    printf("\n=== Diferença entre ponteiros ===\n");
    int *p1 = &numeros[1];
    int *p2 = &numeros[4];
    
    printf("p1 aponta para numeros[1] = %d\n", *p1);
    printf("p2 aponta para numeros[4] = %d\n", *p2);
    printf("p2 - p1 = %ld elementos\n", p2 - p1);
    
    // Aritmética com caracteres
    printf("\n=== Aritmética com char* ===\n");
    char texto[] = "ABCDE";
    char *pc = texto;
    
    printf("String: %s\n", texto);
    for (int i = 0; i < 5; i++) {
        printf("*(pc + %d) = '%c'\n", i, *(pc + i));
    }
    
    // Comparação de ponteiros
    printf("\n=== Comparação de ponteiros ===\n");
    int *pa = &numeros[2];
    int *pb = &numeros[2];
    int *pc_int = &numeros[3];
    
    printf("pa e pb apontam para o mesmo lugar? %s\n", 
           pa == pb ? "Sim" : "Não");
    printf("pa e pc apontam para o mesmo lugar? %s\n", 
           pa == pc_int ? "Sim" : "Não");
    printf("pa < pc? %s (pa vem antes na memória)\n",
           pa < pc_int ? "Sim" : "Não");
    
    return 0;
}
