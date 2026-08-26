# Atividade - Lab 01 - Mini Sistema de Cadastro de Produtos

**Disciplina:** Computação Distribuída   
**Entrega:** Individual, em duplas ou em trios
**Valor:** Entra nas atividades da N1

---

## Objetivo

Desenvolver um **mini sistema de cadastro de produtos** em linguagem C que utilize os principais conceitos estudados: **alocação dinâmica de memória**, **ponteiros**, **passagem por referência** e **manipulação de vetores dinâmicos**.

Esta atividade visa consolidar seu entendimento sobre gerenciamento de memória em C, um conhecimento fundamental para trabalhar com sistemas distribuídos e de baixo nível.

---

## Descrição do Sistema

Você deve implementar um programa em C que permita ao usuário gerenciar uma lista de produtos. O sistema deve funcionar através de um **menu interativo** que permita realizar as operações descritas abaixo.

### Estrutura do Produto

Cada produto deve conter os seguintes campos:

| Campo       | Tipo           | Descrição                                      |
|-------------|----------------|------------------------------------------------|
| `codigo`    | `int`          | Código único do produto (gerado automaticamente) |
| `nome`      | `char*`        | Nome do produto (alocado dinamicamente)        |
| `preco`     | `float`        | Preço unitário do produto                      |
| `quantidade`| `int`          | Quantidade em estoque                          |

**Importante:** O campo `nome` deve ser um ponteiro `char*` com memória alocada dinamicamente usando `malloc`, não um vetor de tamanho fixo.

---

## Funcionalidades Obrigatórias

O programa deve implementar as seguintes funcionalidades:

### 1. Adicionar Produto

- O usuário informa: nome, preço e quantidade.
- O sistema gera automaticamente um código único (incremental).
- O vetor de produtos deve ser **realocado dinamicamente** para acomodar o novo produto (use `realloc`).
- O nome do produto deve ter memória alocada individualmente.

### 2. Listar Todos os Produtos

- Exibe todos os produtos cadastrados em formato tabular.
- Mostra: código, nome, preço, quantidade e **valor em estoque** (preço × quantidade).
- Ao final, exibe o **valor total do estoque** (soma de todos os valores em estoque).

### 3. Buscar Produto por Código

- O usuário informa um código.
- O sistema busca e exibe as informações do produto, ou informa que não foi encontrado.
- A busca deve ser implementada em uma **função separada** que retorne um ponteiro para o produto encontrado (ou `NULL`).

### 4. Atualizar Estoque

- O usuário informa o código do produto e a nova quantidade.
- O sistema deve usar a função de busca (item 3) para localizar o produto.
- Utilize **passagem por referência** para modificar a quantidade.

### 5. Remover Produto

- O usuário informa o código do produto a ser removido.
- O sistema deve:
  1. Localizar o produto no vetor
  2. Liberar a memória do nome (`free`)
  3. Reorganizar o vetor (deslocar elementos)
  4. Realocar o vetor para o novo tamanho (`realloc`)
- **Atenção especial** à ordem de liberação de memória!

### 6. Sair do Programa

- Antes de encerrar, o programa **deve liberar toda a memória alocada**:
  - Primeiro: liberar cada `nome` individualmente
  - Depois: liberar o vetor de produtos
- Exiba uma mensagem confirmando que a memória foi liberada.

---

## Estrutura Sugerida do Código

Seu programa deve conter, **no mínimo**, as seguintes funções:

```c
// Definição da struct
typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

// Funções obrigatórias (você define os parâmetros)
void adicionar_produto(...);
void listar_produtos(...);
Produto* buscar_produto(...);
void atualizar_estoque(...);
void remover_produto(...);
void liberar_memoria(...);
```

**Observação:** Você deve definir quais parâmetros cada função receberá. Pense em:
- Quais dados precisam ser lidos/modificados?
- Quando usar passagem por valor vs. por referência?
- Quando usar ponteiros simples vs. ponteiros de ponteiros?

---

## Requisitos Técnicos

1. **Alocação Dinâmica:** Todo o gerenciamento de produtos deve usar alocação dinâmica (`malloc`, `realloc`, `free`). Não use vetores de tamanho fixo.

2. **Verificação de Erros:** Toda alocação de memória deve verificar se foi bem-sucedida (ponteiro != NULL).

3. **Liberação de Memória:** O programa não deve ter vazamentos de memória (memory leaks).

4. **Modularização:** O código deve estar organizado em funções. A função `main` deve conter apenas o menu e chamadas às funções.

5. **Comentários:** Comente as partes mais importantes do código, especialmente as operações com ponteiros.

---

## Exemplo de Execução

```
========================================
    SISTEMA DE CADASTRO DE PRODUTOS
========================================

Menu:
1. Adicionar produto
2. Listar produtos
3. Buscar produto
4. Atualizar estoque
5. Remover produto
6. Sair

Opção: 1

--- Adicionar Produto ---
Nome: Notebook Dell
Preço: 4500.00
Quantidade: 10
Produto adicionado com código 1!

Opção: 1

--- Adicionar Produto ---
Nome: Mouse Logitech
Preço: 150.00
Quantidade: 50
Produto adicionado com código 2!

Opção: 2

--- Lista de Produtos ---
+--------+------------------+----------+------+---------------+
| Código | Nome             | Preço    | Qtd  | Valor Estoque |
+--------+------------------+----------+------+---------------+
|      1 | Notebook Dell    | 4500.00  |   10 |     45000.00  |
|      2 | Mouse Logitech   |  150.00  |   50 |      7500.00  |
+--------+------------------+----------+------+---------------+
Valor total do estoque: R$ 52500.00

Opção: 4

--- Atualizar Estoque ---
Código do produto: 2
Nova quantidade: 45
Estoque atualizado com sucesso!

Opção: 5

--- Remover Produto ---
Código do produto: 1
Produto "Notebook Dell" removido com sucesso!

Opção: 6

Liberando memória...
Memória do produto "Mouse Logitech" liberada.
Vetor de produtos liberado.
Programa encerrado.
```

---

## Perguntas:

Incluir no documento da entrega:

1. **Por que usamos `char*` para o nome em vez de `char nome[50]`?**  
   Qual a vantagem e desvantagem de cada abordagem?

2. **Explique a diferença entre `Produto*` e `Produto**` no contexto do seu código.**  
   Em que situações você usaria cada um?

3. **Na função de adicionar produto, por que precisamos de `realloc`?**  
   O que aconteceria se usássemos apenas `malloc`?

4. **Na função de remover produto, por que a ordem de liberação de memória importa?**  
   O que aconteceria se liberássemos o vetor de produtos antes de liberar os nomes?

5. **Desenhe o diagrama de memória** mostrando como os produtos estão organizados após adicionar 3 produtos.

6. **O que é um memory leak?** Aponte no seu código onde isso poderia ocorrer se você não tivesse cuidado.

7. **Por que a função de busca retorna um ponteiro (`Produto*`) em vez de retornar o produto por valor (`Produto`)?**

8. **Na atualização de estoque, por que usar passagem por referência?**  
   O que aconteceria se você passasse o produto por valor?

---

## Critérios de Avaliação

| Critério                                    | Peso  |
|---------------------------------------------|-------|
| Funcionamento correto das 6 funcionalidades | 40%   |
| Uso correto de alocação dinâmica            | 20%   |
| Uso correto de ponteiros e referências      | 15%   |
| Liberação adequada de memória               | 10%   |
| Organização e modularização do código       | 10%   |
| Capacidade de explicar o código (defesa)    | 5%    |

---

## Dicas Importantes

1. **Comece simples:** Implemente primeiro a adição e listagem. Depois adicione as outras funcionalidades.

2. **Teste frequentemente:** Compile e teste após cada funcionalidade implementada.

3. **Use o Valgrind:** Se disponível, execute seu programa com Valgrind para detectar vazamentos de memória:
   ```bash
   valgrind --leak-check=full ./seu_programa
   ```

4. **Cuidado com `scanf` e strings:** Para ler strings com espaços, use `fgets` ou `scanf(" %[^\n]", buffer)`.

5. **Desenhe antes de codificar:** Faça um diagrama de como a memória estará organizada. Isso ajuda muito a entender os ponteiros.

---

## Forma de Entrega

- Entregar um aquivo PDF via Moodle:
    - Inlcuir o nome dos integrantes do grupo em ordem afabética e RAs
    - Inlcuir o código-fonte no documento ou link para o repositório
    - Incluir prints de testes realizados
    - Icnluir as respostas das perguntas
- Não é preciso entregar o código-fonte, a não ser que use um repositório (recomendado) para a equipe
- Seu código deve compilar sem erros com, exemplo: `gcc -Wall -o atividade atividade_01_nome_ra.c` se o professor quiser rodá-lo integralmente.

---

**Boa sorte e bom trabalho!**

*Lembre-se: entender o que você está fazendo é mais importante do que apenas fazer funcionar.*
