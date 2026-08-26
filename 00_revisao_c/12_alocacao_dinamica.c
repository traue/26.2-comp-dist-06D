/*
 * Exemplo 12: Alocação Dinâmica de Memória
 * Criando vetores dinamicamente com malloc, calloc, realloc e free
 */

#include <stdio.h>
#include <stdlib.h>  // Para malloc, calloc, realloc, free

int main() {
    int *vetor;
    int n;
    
    // Alocação com malloc
    printf("=== malloc - Memory Allocation ===\n");
    n = 5;
    
    // malloc aloca n * sizeof(int) bytes
    // Retorna ponteiro void* que é convertido para int*
    vetor = (int*) malloc(n * sizeof(int));
    
    if (vetor == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return 1;
    }
    
    // Valores não são inicializados (lixo de memória)
    printf("Valores após malloc (não inicializados):\n");
    for (int i = 0; i < n; i++) {
        printf("vetor[%d] = %d (lixo)\n", i, vetor[i]);
    }
    
    // Preenchendo o vetor
    for (int i = 0; i < n; i++) {
        vetor[i] = (i + 1) * 10;
    }
    
    printf("\nApós inicialização:\n");
    for (int i = 0; i < n; i++) {
        printf("vetor[%d] = %d\n", i, vetor[i]);
    }
    
    // Liberando memória
    free(vetor);
    printf("\nMemória liberada com free()\n");
    
    // Alocação com calloc
    printf("\n=== calloc - Cleared Allocation ===\n");
    n = 5;
    
    // calloc aloca e inicializa com zeros
    // Sintaxe: calloc(quantidade, tamanho_de_cada)
    vetor = (int*) calloc(n, sizeof(int));
    
    if (vetor == NULL) {
        printf("Erro: Falha na alocação!\n");
        return 1;
    }
    
    printf("Valores após calloc (inicializados com 0):\n");
    for (int i = 0; i < n; i++) {
        printf("vetor[%d] = %d\n", i, vetor[i]);
    }
    
    // Preenchendo
    for (int i = 0; i < n; i++) {
        vetor[i] = i * i;
    }
    
    printf("\nApós atribuição (quadrados):\n");
    for (int i = 0; i < n; i++) {
        printf("vetor[%d] = %d\n", i, vetor[i]);
    }
    
    // Redimensionando com realloc
    printf("\n=== realloc - Reallocation ===\n");
    int novo_tamanho = 8;
    
    // realloc muda o tamanho mantendo os dados existentes
    vetor = (int*) realloc(vetor, novo_tamanho * sizeof(int));
    
    if (vetor == NULL) {
        printf("Erro: Falha na realocação!\n");
        return 1;
    }
    
    printf("Após realloc para %d elementos:\n", novo_tamanho);
    printf("(Elementos antigos preservados, novos não inicializados)\n");
    
    // Inicializando novos elementos
    for (int i = n; i < novo_tamanho; i++) {
        vetor[i] = 99;
    }
    
    for (int i = 0; i < novo_tamanho; i++) {
        printf("vetor[%d] = %d\n", i, vetor[i]);
    }
    
    // Liberando memória
    free(vetor);
    vetor = NULL;  // Boa prática: setar para NULL após free
    
    // Exemplo: Vetor de tamanho definido em tempo de execução
    printf("\n=== Vetor de tamanho dinâmico ===\n");
    int tamanho = 7;  // Simulando entrada do usuário
    
    int *dinamico = (int*) malloc(tamanho * sizeof(int));
    
    if (dinamico) {
        printf("Alocado vetor de %d elementos\n", tamanho);
        
        // Preenchendo
        for (int i = 0; i < tamanho; i++) {
            dinamico[i] = i + 1;
        }
        
        // Calculando soma
        int soma = 0;
        for (int i = 0; i < tamanho; i++) {
            soma += dinamico[i];
        }
        printf("Soma dos elementos: %d\n", soma);
        
        free(dinamico);
        dinamico = NULL;
    }
    
    printf("\n=== Resumo ===\n");
    printf("malloc(size): aloca 'size' bytes, não inicializa\n");
    printf("calloc(n, size): aloca n*size bytes, inicializa com 0\n");
    printf("realloc(ptr, size): redimensiona bloco existente\n");
    printf("free(ptr): libera memória alocada\n");
    
    return 0;
}
