/*
 * Exemplo 10: Passagem por Referência
 * Usando ponteiros para modificar variáveis em funções
 */

#include <stdio.h>

// Função que NÃO consegue trocar valores (passagem por valor)
void trocar_errado(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    printf("Dentro de trocar_errado: a = %d, b = %d\n", a, b);
}

// Função que CONSEGUE trocar valores (passagem por referência)
void trocar_certo(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    printf("Dentro de trocar_certo: *a = %d, *b = %d\n", *a, *b);
}

// Função que modifica um valor
void dobrar(int *num) {
    *num = *num * 2;
}

// Função que retorna múltiplos valores via ponteiros
void dividir(int dividendo, int divisor, int *quociente, int *resto) {
    *quociente = dividendo / divisor;
    *resto = dividendo % divisor;
}

// Função que encontra min e max de um vetor
void encontrar_min_max(int *vetor, int tamanho, int *min, int *max) {
    *min = vetor[0];
    *max = vetor[0];
    
    for (int i = 1; i < tamanho; i++) {
        if (vetor[i] < *min) {
            *min = vetor[i];
        }
        if (vetor[i] > *max) {
            *max = vetor[i];
        }
    }
}

// Função que incrementa cada elemento do vetor
void incrementar_vetor(int *vetor, int tamanho, int valor) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] += valor;
    }
}

int main() {
    // Demonstração de passagem por valor vs referência
    printf("=== Passagem por Valor (não funciona) ===\n");
    int x = 5, y = 10;
    printf("Antes: x = %d, y = %d\n", x, y);
    trocar_errado(x, y);
    printf("Depois: x = %d, y = %d (não trocou!)\n", x, y);
    
    printf("\n=== Passagem por Referência (funciona) ===\n");
    x = 5; y = 10;
    printf("Antes: x = %d, y = %d\n", x, y);
    trocar_certo(&x, &y);
    printf("Depois: x = %d, y = %d (trocou!)\n", x, y);
    
    // Modificando valor via ponteiro
    printf("\n=== Dobrar valor ===\n");
    int numero = 7;
    printf("Antes: numero = %d\n", numero);
    dobrar(&numero);
    printf("Depois: numero = %d\n", numero);
    
    // Retornando múltiplos valores
    printf("\n=== Múltiplos valores de retorno ===\n");
    int q, r;
    dividir(17, 5, &q, &r);
    printf("17 / 5 = %d, resto = %d\n", q, r);
    
    dividir(100, 7, &q, &r);
    printf("100 / 7 = %d, resto = %d\n", q, r);
    
    // Min e Max de um vetor
    printf("\n=== Encontrar Min e Max ===\n");
    int valores[] = {45, 12, 78, 34, 89, 23, 56};
    int minimo, maximo;
    
    printf("Vetor: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", valores[i]);
    }
    printf("\n");
    
    encontrar_min_max(valores, 7, &minimo, &maximo);
    printf("Mínimo: %d, Máximo: %d\n", minimo, maximo);
    
    // Modificando vetor via ponteiro
    printf("\n=== Incrementar vetor ===\n");
    int nums[] = {1, 2, 3, 4, 5};
    
    printf("Antes: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    
    incrementar_vetor(nums, 5, 10);
    
    printf("Depois (+10): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    
    return 0;
}
