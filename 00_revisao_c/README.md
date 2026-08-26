# 📚 Revisão de Linguagem C

Este material é uma revisão didática dos principais conceitos da linguagem C, desde vetores e loops até ponteiros avançados.

---

## 📑 Índice

1. [Vetores (Arrays)](#1-vetores-arrays)
2. [Matrizes (Vetores Multidimensionais)](#2-matrizes-vetores-multidimensionais)
3. [Loop FOR](#3-loop-for)
4. [Loop WHILE](#4-loop-while)
5. [Loop DO-WHILE](#5-loop-do-while)
6. [Loops Aninhados](#6-loops-aninhados)
7. [Break e Continue](#7-break-e-continue)
8. [Ponteiros - Conceitos Básicos](#8-ponteiros---conceitos-básicos)
9. [Aritmética de Ponteiros](#9-aritmética-de-ponteiros)
10. [Passagem por Referência](#10-passagem-por-referência)
11. [Vetores e Ponteiros](#11-vetores-e-ponteiros)
12. [Alocação Dinâmica de Memória](#12-alocação-dinâmica-de-memória)
13. [Ponteiros de Ponteiros](#13-ponteiros-de-ponteiros)
14. [Matriz Dinâmica com **](#14-matriz-dinâmica-com-)
15. [Vetor de Strings](#15-vetor-de-strings)

**ATIVIDADE**: [Disponível aqui](atividade_01.md)

---

## 1. Vetores (Arrays)

**Arquivo completo:** [01_vetores_basico.c](01_vetores_basico.c)

### O que é um vetor?
Um vetor é uma coleção de elementos **do mesmo tipo**, armazenados em posições **contíguas** de memória.

### Declaração

```c
// Declaração simples
int numeros[5];  // Vetor de 5 inteiros

// Declaração com inicialização
int notas[4] = {7, 8, 9, 10};

// Tamanho automático
int valores[] = {1, 2, 3};  // Tamanho = 3
```

### Acesso aos elementos

Os índices começam em **0** e vão até **n-1**:

```c
int numeros[5];

numeros[0] = 10;  // Primeiro elemento
numeros[1] = 20;  // Segundo elemento
numeros[4] = 50;  // Último elemento (índice 4, não 5!)

printf("%d", numeros[0]);  // Imprime 10
```

### Strings (vetores de char)

```c
char nome[6] = {'M', 'a', 'r', 'i', 'a', '\0'};
// Ou mais simples:
char sobrenome[] = "Silva";

printf("%s", nome);  // Imprime: Maria
```

> ⚠️ **Importante:** Strings em C terminam com `\0` (caractere nulo).

---

## 2. Matrizes (Vetores Multidimensionais)

**Arquivo completo:** [02_vetores_matrizes.c](02_vetores_matrizes.c)

### Declaração de matriz

```c
// Matriz 3x3
int matriz[3][3];

// Com inicialização
int tabela[2][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8}
};
```

### Acesso aos elementos

```c
matriz[0][0] = 1;  // Linha 0, Coluna 0
matriz[1][2] = 5;  // Linha 1, Coluna 2

printf("%d", tabela[1][3]);  // Imprime: 8
```

### Percorrendo uma matriz

```c
for (int i = 0; i < 3; i++) {        // Linhas
    for (int j = 0; j < 3; j++) {    // Colunas
        printf("%d ", matriz[i][j]);
    }
    printf("\n");
}
```

---

## 3. Loop FOR

**Arquivo completo:** [03_loop_for.c](03_loop_for.c)

### Sintaxe

```c
for (inicialização; condição; incremento) {
    // código a ser repetido
}
```

### Exemplos

```c
// Contando de 1 a 10
for (int i = 1; i <= 10; i++) {
    printf("%d ", i);
}
// Saída: 1 2 3 4 5 6 7 8 9 10

// Contagem regressiva
for (int i = 10; i >= 1; i--) {
    printf("%d ", i);
}
// Saída: 10 9 8 7 6 5 4 3 2 1

// Números pares
for (int i = 0; i <= 20; i += 2) {
    printf("%d ", i);
}
// Saída: 0 2 4 6 8 10 12 14 16 18 20
```

### Percorrendo vetor com FOR

```c
int valores[5] = {100, 200, 300, 400, 500};

for (int i = 0; i < 5; i++) {
    printf("valores[%d] = %d\n", i, valores[i]);
}
```

---

## 4. Loop WHILE

**Arquivo completo:** [04_loop_while.c](04_loop_while.c)

### Sintaxe

```c
while (condição) {
    // código a ser repetido
}
```

A condição é verificada **antes** de cada iteração.

### Exemplos

```c
// Contando de 1 a 5
int i = 1;
while (i <= 5) {
    printf("%d ", i);
    i++;
}
// Saída: 1 2 3 4 5

// Soma de 1 a 100
int soma = 0;
int n = 1;
while (n <= 100) {
    soma += n;
    n++;
}
printf("Soma = %d", soma);  // Soma = 5050
```

### Extraindo dígitos de um número

```c
int numero = 12345;
while (numero > 0) {
    int digito = numero % 10;   // Último dígito
    printf("%d ", digito);
    numero = numero / 10;       // Remove último dígito
}
// Saída: 5 4 3 2 1
```

---

## 5. Loop DO-WHILE

**Arquivo completo:** [05_loop_do_while.c](05_loop_do_while.c)

### Sintaxe

```c
do {
    // código a ser repetido
} while (condição);
```

A condição é verificada **depois** de cada iteração. **Garante pelo menos uma execução.**

### Diferença para WHILE

```c
int x = 10;

// WHILE: não executa nada (condição falsa desde o início)
while (x < 5) {
    printf("WHILE executou\n");
}

// DO-WHILE: executa pelo menos uma vez
do {
    printf("DO-WHILE executou\n");
} while (x < 5);
```

### Uso típico: Menu

```c
int opcao;
do {
    printf("1. Opção A\n");
    printf("2. Opção B\n");
    printf("0. Sair\n");
    scanf("%d", &opcao);
    
    // processar opção...
    
} while (opcao != 0);
```

---

## 6. Loops Aninhados

**Arquivo completo:** [06_loops_aninhados.c](06_loops_aninhados.c)

### Conceito

Um loop dentro de outro. O loop **interno** executa completamente para cada iteração do **externo**.

### Padrões com asteriscos

```c
// Retângulo 3x5
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) {
        printf("* ");
    }
    printf("\n");
}
/* Saída:
* * * * * 
* * * * * 
* * * * * 
*/

// Triângulo
for (int i = 1; i <= 5; i++) {
    for (int j = 1; j <= i; j++) {
        printf("* ");
    }
    printf("\n");
}
/* Saída:
* 
* * 
* * * 
* * * * 
* * * * * 
*/
```

### Preenchendo matriz

```c
int matriz[3][3];

for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        matriz[i][j] = (i + 1) * (j + 1);
    }
}
```

---

## 7. Break e Continue

**Arquivo completo:** [07_break_continue.c](07_break_continue.c)

### BREAK

**Interrompe** o loop completamente.

```c
for (int i = 1; i <= 10; i++) {
    if (i == 5) {
        break;  // Sai do loop
    }
    printf("%d ", i);
}
// Saída: 1 2 3 4
```

### CONTINUE

**Pula** para a próxima iteração.

```c
for (int i = 1; i <= 10; i++) {
    if (i % 2 == 0) {
        continue;  // Pula números pares
    }
    printf("%d ", i);
}
// Saída: 1 3 5 7 9
```

### Busca em vetor com BREAK

```c
int numeros[] = {10, 25, 30, 45, 50};
int busca = 30;
int encontrado = -1;

for (int i = 0; i < 5; i++) {
    if (numeros[i] == busca) {
        encontrado = i;
        break;  // Não precisa continuar
    }
}
```

---

## 8. Ponteiros - Conceitos Básicos

**Arquivo completo:** [08_ponteiros_basico.c](08_ponteiros_basico.c)

### O que é um ponteiro?

Um ponteiro é uma variável que **armazena um endereço de memória**.

### Operadores importantes

| Operador | Significado |
|----------|-------------|
| `*` (na declaração) | Indica que é um ponteiro |
| `*` (uso) | Acessa o valor no endereço (dereferência) |
| `&` | Retorna o endereço de uma variável |

### Declaração e uso

```c
int numero = 42;
int *ptr;         // Declaração do ponteiro

ptr = &numero;    // ptr recebe o endereço de numero

printf("%d", numero);   // 42 (valor)
printf("%p", &numero);  // endereço de numero
printf("%p", ptr);      // mesmo endereço (armazenado em ptr)
printf("%d", *ptr);     // 42 (valor no endereço)
```

### Diagrama

```
+--------+                 +---------+
|  ptr   | --------------> | numero  |
| 0x1000 |                 |   42    |
+--------+     (aponta)    +---------+
                           endereço: 0x1000
```

### Modificando valor via ponteiro

```c
int numero = 42;
int *ptr = &numero;

*ptr = 100;  // Modifica o valor de 'numero'

printf("%d", numero);  // 100
```

### Ponteiro NULL

```c
int *ptr = NULL;  // Não aponta para nada

if (ptr == NULL) {
    printf("Ponteiro inválido!\n");
}
```

---

## 9. Aritmética de Ponteiros

**Arquivo completo:** [09_aritmetica_ponteiros.c](09_aritmetica_ponteiros.c)

### Conceito

Quando você soma/subtrai de um ponteiro, ele avança/retrocede pelo **tamanho do tipo**.

```c
int numeros[] = {10, 20, 30, 40, 50};
int *ptr = numeros;

printf("%d", *ptr);        // 10 (elemento 0)
printf("%d", *(ptr + 1));  // 20 (elemento 1)
printf("%d", *(ptr + 2));  // 30 (elemento 2)
```

### Incremento de ponteiro

```c
int numeros[] = {10, 20, 30, 40, 50};
int *ptr = numeros;

ptr++;  // Avança sizeof(int) bytes
printf("%d", *ptr);  // 20
```

### Percorrendo vetor com ponteiro

```c
int numeros[] = {10, 20, 30, 40, 50};
int *inicio = numeros;
int *fim = numeros + 5;

while (inicio < fim) {
    printf("%d ", *inicio);
    inicio++;
}
// Saída: 10 20 30 40 50
```

### Diferença entre ponteiros

```c
int *p1 = &numeros[1];
int *p2 = &numeros[4];

printf("%ld", p2 - p1);  // 3 (elementos de diferença)
```

---

## 10. Passagem por Referência

**Arquivo completo:** [10_passagem_referencia.c](10_passagem_referencia.c)

### Problema: Passagem por valor

```c
void trocar(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

int x = 5, y = 10;
trocar(x, y);
// x ainda é 5, y ainda é 10 - NÃO FUNCIONOU!
```

### Solução: Passagem por referência

```c
void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int x = 5, y = 10;
trocar(&x, &y);  // Passa os endereços
// x é 10, y é 5 - FUNCIONOU!
```

### Múltiplos valores de retorno

```c
void dividir(int dividendo, int divisor, int *quociente, int *resto) {
    *quociente = dividendo / divisor;
    *resto = dividendo % divisor;
}

int q, r;
dividir(17, 5, &q, &r);
// q = 3, r = 2
```

---

## 11. Vetores e Ponteiros

**Arquivo completo:** [11_vetores_ponteiros.c](11_vetores_ponteiros.c)

### Relação fundamental

O nome de um vetor é um **ponteiro constante** para seu primeiro elemento.

```c
int numeros[] = {10, 20, 30, 40, 50};
int *ptr = numeros;  // Equivalente a: ptr = &numeros[0]

// Formas equivalentes de acesso:
numeros[2]      // 30
*(numeros + 2)  // 30
ptr[2]          // 30
*(ptr + 2)      // 30
```

### Tabela de equivalências

| Notação de índice | Notação de ponteiro |
|-------------------|---------------------|
| `numeros[i]` | `*(numeros + i)` |
| `&numeros[i]` | `numeros + i` |

### Diferença importante

```c
int numeros[5];
int *ptr = numeros;

sizeof(numeros);  // 20 (5 * 4 bytes) - tamanho total
sizeof(ptr);      // 8 (tamanho do ponteiro)
```

---

## 12. Alocação Dinâmica de Memória

**Arquivo completo:** [12_alocacao_dinamica.c](12_alocacao_dinamica.c)

### Funções principais

| Função | Descrição |
|--------|-----------|
| `malloc(size)` | Aloca `size` bytes, **não inicializa** |
| `calloc(n, size)` | Aloca `n * size` bytes, **inicializa com 0** |
| `realloc(ptr, size)` | **Redimensiona** bloco existente |
| `free(ptr)` | **Libera** memória alocada |

### malloc

```c
#include <stdlib.h>

int *vetor = (int*) malloc(5 * sizeof(int));

if (vetor == NULL) {
    printf("Erro na alocação!\n");
    return 1;
}

// Usar o vetor...
vetor[0] = 10;
vetor[1] = 20;

free(vetor);  // Liberar quando não precisar mais
```

### calloc

```c
// Aloca E inicializa com zeros
int *vetor = (int*) calloc(5, sizeof(int));

// vetor[0] até vetor[4] são todos 0
```

### realloc

```c
int *vetor = (int*) malloc(5 * sizeof(int));

// Redimensionar para 10 elementos
vetor = (int*) realloc(vetor, 10 * sizeof(int));
```

> ⚠️ **Sempre libere a memória com `free()` quando não precisar mais!**

---

## 13. Ponteiros de Ponteiros

**Arquivo completo:** [13_ponteiros_de_ponteiros.c](13_ponteiros_de_ponteiros.c)

### Conceito

Um ponteiro que armazena o endereço de **outro ponteiro**.

```c
int valor = 100;
int *ptr = &valor;      // Ponteiro para int
int **pptr = &ptr;      // Ponteiro para ponteiro de int

printf("%d", valor);    // 100
printf("%d", *ptr);     // 100
printf("%d", **pptr);   // 100
```

### Diagrama

```
+--------+     +--------+     +--------+
|  pptr  | --> |  ptr   | --> | valor  |
| 0x3000 |     | 0x2000 |     |  100   |
+--------+     +--------+     +--------+
```

### Acessos

```c
pptr    // Endereço de ptr
*pptr   // Valor de ptr (= endereço de valor)
**pptr  // Valor de valor (= 100)
```

### Modificando via **

```c
**pptr = 200;  // Muda 'valor' para 200
```

---

## 14. Matriz Dinâmica com **

**Arquivo completo:** [14_matriz_dinamica.c](14_matriz_dinamica.c)

### Conceito

Uma matriz 2D dinâmica é um **vetor de ponteiros**, onde cada ponteiro aponta para uma **linha**.

### Alocação

```c
int linhas = 3, colunas = 4;
int **matriz;

// 1. Alocar vetor de ponteiros (linhas)
matriz = (int**) malloc(linhas * sizeof(int*));

// 2. Alocar cada linha
for (int i = 0; i < linhas; i++) {
    matriz[i] = (int*) malloc(colunas * sizeof(int));
}
```

### Estrutura na memória

```
matriz --> [ ptr0 | ptr1 | ptr2 ]
              |      |      |
              v      v      v
           [....] [....] [....]
           linha0 linha1 linha2
```

### Uso

```c
matriz[1][2] = 10;  // Linha 1, Coluna 2

// Equivalente:
*(*(matriz + 1) + 2) = 10;
```

### Liberação (ordem correta!)

```c
// Primeiro: liberar cada linha
for (int i = 0; i < linhas; i++) {
    free(matriz[i]);
}

// Depois: liberar o vetor de ponteiros
free(matriz);
```

---

## 15. Vetor de Strings

**Arquivo completo:** [15_vetor_strings.c](15_vetor_strings.c)

### Conceito

`char*` é uma string. `char**` é um vetor de strings.

### Declaração estática

```c
char *frutas[] = {"Maçã", "Banana", "Laranja"};

printf("%s", frutas[0]);  // Maçã
printf("%s", frutas[1]);  // Banana
```

### Alocação dinâmica

```c
int n = 3;
char **nomes = (char**) malloc(n * sizeof(char*));

// Alocar cada string
nomes[0] = (char*) malloc(10 * sizeof(char));
strcpy(nomes[0], "Alice");

nomes[1] = (char*) malloc(10 * sizeof(char));
strcpy(nomes[1], "Bob");
```

### main() com argumentos

```c
int main(int argc, char **argv) {
    // argc: número de argumentos
    // argv: vetor de strings
    // argv[0]: nome do programa
    // argv[1], argv[2], ...: argumentos
    
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    
    return 0;
}
```

---

## 📝 Resumo de Operadores de Ponteiro

| Símbolo | Contexto | Significado |
|---------|----------|-------------|
| `*` | Declaração: `int *p` | p é um ponteiro para int |
| `*` | Uso: `*p` | Valor no endereço apontado por p |
| `&` | `&x` | Endereço de x |
| `**` | Declaração: `int **pp` | pp é ponteiro para ponteiro |
| `**` | Uso: `**pp` | Valor final (dois níveis de indireção) |

---

## 🔧 Como Compilar e Executar

### Usando o Makefile (recomendado)

```bash
# Compilar TODOS os exemplos (gera executáveis em ./bin/)
make

# Compilar e executar um exemplo específico
make run-01_vetores_basico
make run-08_ponteiros_basico

# Listar todos os exemplos disponíveis
make list

# Limpar executáveis compilados
make clean

# Recompilar tudo do zero
make rebuild
```

### Compilação manual

```bash
# Compilar um arquivo específico
gcc -Wall -o bin/01_vetores_basico 01_vetores_basico.c

# Executar
./bin/01_vetores_basico
```

### Estrutura de diretórios

```
revisao_c/
├── bin/                  # Executáveis compilados (gerado pelo make)
├── *.c                   # Códigos-fonte
├── Makefile              # Automação de compilação
├── .gitignore            # Ignora bin/ no git
└── README.md             # Esta documentação
```

---

## 📚 Arquivos de Exemplo

| # | Arquivo | Tópico |
|---|---------|--------|
| 01 | [01_vetores_basico.c](01_vetores_basico.c) | Vetores básicos |
| 02 | [02_vetores_matrizes.c](02_vetores_matrizes.c) | Matrizes |
| 03 | [03_loop_for.c](03_loop_for.c) | Loop FOR |
| 04 | [04_loop_while.c](04_loop_while.c) | Loop WHILE |
| 05 | [05_loop_do_while.c](05_loop_do_while.c) | Loop DO-WHILE |
| 06 | [06_loops_aninhados.c](06_loops_aninhados.c) | Loops aninhados |
| 07 | [07_break_continue.c](07_break_continue.c) | Break e Continue |
| 08 | [08_ponteiros_basico.c](08_ponteiros_basico.c) | Ponteiros básicos |
| 09 | [09_aritmetica_ponteiros.c](09_aritmetica_ponteiros.c) | Aritmética de ponteiros |
| 10 | [10_passagem_referencia.c](10_passagem_referencia.c) | Passagem por referência |
| 11 | [11_vetores_ponteiros.c](11_vetores_ponteiros.c) | Vetores e ponteiros |
| 12 | [12_alocacao_dinamica.c](12_alocacao_dinamica.c) | Alocação dinâmica |
| 13 | [13_ponteiros_de_ponteiros.c](13_ponteiros_de_ponteiros.c) | Ponteiros de ponteiros |
| 14 | [14_matriz_dinamica.c](14_matriz_dinamica.c) | Matriz dinâmica |
| 15 | [15_vetor_strings.c](15_vetor_strings.c) | Vetor de strings |

---

**Bons estudos!** 🎓
