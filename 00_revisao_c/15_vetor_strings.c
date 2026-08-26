/*
 * Exemplo 15: Vetor de Strings com Ponteiros
 * Uso de char** para manipular múltiplas strings
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Vetor de strings estático
    printf("=== Vetor de Strings Estático ===\n");
    
    // char* é ponteiro para char (string)
    // char*[] é vetor de ponteiros para char (vetor de strings)
    char *frutas[] = {"Maçã", "Banana", "Laranja", "Uva", "Manga"};
    int num_frutas = 5;
    
    printf("Frutas:\n");
    for (int i = 0; i < num_frutas; i++) {
        printf("  %d. %s\n", i + 1, frutas[i]);
    }
    
    // Usando char** para apontar para o vetor
    printf("\n=== Usando char** ===\n");
    char **ptr_frutas = frutas;
    
    printf("Acessando via char**:\n");
    for (int i = 0; i < num_frutas; i++) {
        printf("  ptr_frutas[%d] = %s\n", i, ptr_frutas[i]);
    }
    
    // Alocação dinâmica de vetor de strings
    printf("\n=== Vetor de Strings Dinâmico ===\n");
    
    int quantidade = 4;
    char **nomes;
    
    // Alocar vetor de ponteiros
    nomes = (char**) malloc(quantidade * sizeof(char*));
    
    if (nomes == NULL) {
        printf("Erro na alocação!\n");
        return 1;
    }
    
    // Alocar cada string
    char *nomes_temp[] = {"Alice", "Bob", "Carlos", "Diana"};
    
    for (int i = 0; i < quantidade; i++) {
        // +1 para o '\0'
        nomes[i] = (char*) malloc((strlen(nomes_temp[i]) + 1) * sizeof(char));
        
        if (nomes[i] == NULL) {
            printf("Erro na alocação da string %d!\n", i);
            return 1;
        }
        
        strcpy(nomes[i], nomes_temp[i]);
    }
    
    printf("Nomes alocados dinamicamente:\n");
    for (int i = 0; i < quantidade; i++) {
        printf("  nomes[%d] = %s (endereço: %p)\n", i, nomes[i], (void*)nomes[i]);
    }
    
    // Modificando uma string
    printf("\n=== Modificando String ===\n");
    
    // Liberar a string antiga
    free(nomes[1]);
    
    // Alocar nova string
    char *novo_nome = "Roberto";
    nomes[1] = (char*) malloc((strlen(novo_nome) + 1) * sizeof(char));
    strcpy(nomes[1], novo_nome);
    
    printf("Após modificar nomes[1]:\n");
    for (int i = 0; i < quantidade; i++) {
        printf("  nomes[%d] = %s\n", i, nomes[i]);
    }
    
    // Ordenação simples (Bubble Sort)
    printf("\n=== Ordenação Alfabética ===\n");
    
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (strcmp(nomes[j], nomes[j + 1]) > 0) {
                // Trocar ponteiros (não as strings)
                char *temp = nomes[j];
                nomes[j] = nomes[j + 1];
                nomes[j + 1] = temp;
            }
        }
    }
    
    printf("Nomes ordenados:\n");
    for (int i = 0; i < quantidade; i++) {
        printf("  %d. %s\n", i + 1, nomes[i]);
    }
    
    // Liberando memória
    printf("\n=== Liberando Memória ===\n");
    for (int i = 0; i < quantidade; i++) {
        free(nomes[i]);
    }
    free(nomes);
    printf("Memória liberada!\n");
    
    // Argumentos de linha de comando (simulação)
    printf("\n=== Simulação de argc/argv ===\n");
    printf("Em main(int argc, char **argv):\n");
    printf("  - argc: número de argumentos\n");
    printf("  - argv: vetor de strings (char**)\n");
    printf("  - argv[0]: nome do programa\n");
    printf("  - argv[1], argv[2], ...: argumentos\n");
    
    // Simulando argv
    char *argv_simulado[] = {"./programa", "-v", "--output", "arquivo.txt"};
    int argc_simulado = 4;
    
    printf("\nSimulação:\n");
    for (int i = 0; i < argc_simulado; i++) {
        printf("  argv[%d] = %s\n", i, argv_simulado[i]);
    }
    
    return 0;
}
