/*
 * Exemplo 01: Vetores Básico
 * Declaração, inicialização e acesso a elementos de um vetor
 */

#include <stdio.h>

int main() {
    // Declaração de um vetor de inteiros com 5 elementos
    int numeros[5];
    
    // Atribuindo valores ao vetor
    numeros[0] = 10;
    numeros[1] = 20;
    numeros[2] = 30;
    numeros[3] = 40;
    numeros[4] = 50;
    
    // Acessando e imprimindo os valores
    printf("=== Vetor de Inteiros ===\n");
    printf("numeros[0] = %d\n", numeros[0]);
    printf("numeros[1] = %d\n", numeros[1]);
    printf("numeros[2] = %d\n", numeros[2]);
    printf("numeros[3] = %d\n", numeros[3]);
    printf("numeros[4] = %d\n", numeros[4]);
    
    // Declaração com inicialização direta
    int notas[4] = {7, 8, 9, 10};
    
    printf("\n=== Vetor Inicializado Diretamente ===\n");
    printf("notas[0] = %d\n", notas[0]);
    printf("notas[1] = %d\n", notas[1]);
    printf("notas[2] = %d\n", notas[2]);
    printf("notas[3] = %d\n", notas[3]);
    
    // Vetor de caracteres (string)
    char nome[6] = {'M', 'a', 'r', 'i', 'a', '\0'};
    printf("\n=== Vetor de Caracteres ===\n");
    printf("Nome: %s\n", nome);
    
    // Forma mais simples de declarar string
    char sobrenome[] = "Silva";
    printf("Sobrenome: %s\n", sobrenome);
    
    return 0;
}
