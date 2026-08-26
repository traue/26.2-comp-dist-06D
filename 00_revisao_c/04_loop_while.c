/*
 * Exemplo 04: Loop WHILE
 * Estrutura de repetição com condição no início
 */

#include <stdio.h>

int main() {
    // WHILE básico - contando de 1 a 5
    printf("=== Contando de 1 a 5 ===\n");
    int contador = 1;
    while (contador <= 5) {
        printf("%d ", contador);
        contador++;
    }
    printf("\n");
    
    // WHILE com decremento
    printf("\n=== Contagem regressiva ===\n");
    int num = 10;
    while (num > 0) {
        printf("%d ", num);
        num--;
    }
    printf("\n");
    
    // WHILE para soma de números
    printf("\n=== Soma de números de 1 a 100 ===\n");
    int soma = 0;
    int i = 1;
    while (i <= 100) {
        soma += i;
        i++;
    }
    printf("Soma = %d\n", soma);
    
    // WHILE para encontrar dígitos de um número
    printf("\n=== Dígitos do número 12345 ===\n");
    int numero = 12345;
    printf("Número original: %d\n", numero);
    printf("Dígitos (ordem inversa): ");
    while (numero > 0) {
        int digito = numero % 10;  // Pega o último dígito
        printf("%d ", digito);
        numero = numero / 10;      // Remove o último dígito
    }
    printf("\n");
    
    // WHILE para validar entrada (simulado)
    printf("\n=== Simulação de validação ===\n");
    int tentativas = 0;
    int senha_correta = 1234;
    int senha_digitada = 0000;
    
    // Simulando tentativas
    int senhas_teste[] = {1111, 2222, 1234};
    int idx = 0;
    
    while (senha_digitada != senha_correta && tentativas < 3) {
        senha_digitada = senhas_teste[idx];
        printf("Tentativa %d: %d\n", tentativas + 1, senha_digitada);
        tentativas++;
        idx++;
    }
    
    if (senha_digitada == senha_correta) {
        printf("Senha correta!\n");
    } else {
        printf("Número máximo de tentativas excedido.\n");
    }
    
    return 0;
}
