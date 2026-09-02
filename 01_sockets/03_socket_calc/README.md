# 🧮 Parte 3 — Calculadora Remota com Sockets em C

## Objetivo

Implementar uma **calculadora cliente-servidor** onde o cliente envia expressões matemáticas e o **servidor calcula e retorna o resultado**. O cliente funciona como um terminal interativo — envia operações em loop até decidir sair.

```
┌──────────────┐     "10 + 3"     ┌──────────────┐
│    CLIENTE    │ ──────────────▶  │   SERVIDOR   │
│  (terminal)  │                  │  (processa)  │
│              │  ◀────────────── │              │
│              │     "= 13"       │  10 + 3 = 13 │
└──────────────┘                  └──────────────┘
```

---

## Conceito-chave: Protocolo de Aplicação

Até agora, enviamos texto livre entre cliente e servidor. Neste projeto, definimos um **protocolo simples** — regras sobre o formato das mensagens:

### Protocolo da Calculadora

| Direção | Formato | Exemplo |
|---------|---------|---------|
| Cliente → Servidor | `<número> <operador> <número>` | `12.5 + 7.3` |
| Servidor → Cliente (sucesso) | `= <resultado>` | `= 19.8` |
| Servidor → Cliente (erro) | `ERRO: <descrição>` | `ERRO: divisão por zero!` |
| Cliente → Servidor (sair) | `sair` | `sair` |

Este é um conceito fundamental em redes: os **dados brutos** que trafegam pelo socket são apenas bytes. É o **protocolo** que dá significado a esses bytes. HTTP, FTP, SMTP — todos são protocolos de aplicação construídos sobre sockets TCP, assim como a nossa calculadora.

---

## Operadores suportados

| Operador | Operação          | Exemplo        | Resultado |
|----------|--------------------|----------------|-----------|
| `+`      | Soma               | `10 + 3`       | `= 13`    |
| `-`      | Subtração          | `10 - 3`       | `= 7`     |
| `*`      | Multiplicação      | `10 * 3`       | `= 30`    |
| `x`      | Multiplicação (alt)| `10 x 3`       | `= 30`    |
| `/`      | Divisão            | `10 / 3`       | `= 3.33333` |
| `^`      | Potência           | `2 ^ 10`       | `= 1024`  |
| `%`      | Módulo (resto)     | `10 % 3`       | `= 1`     |

Aceita números decimais: `3.14 * 2` → `= 6.28`

---

## Arquitetura

### Servidor (`calc_servidor.c`)

```
main()
  │
  ├── Cria socket, bind, listen
  │
  └── Loop (aceita clientes):
        │
        ├── accept() → novo cliente
        │
        └── atender_cliente():
              │
              └── Loop (operações):
                    ├── Envia prompt "calc> "
                    ├── recv() → expressão do cliente
                    ├── calcular() → parse e cálculo
                    ├── send() → resultado ou erro
                    └── Se "sair" → encerra sessão
```

### Cliente (`calc_cliente.c`)

```
main()
  │
  ├── Conecta ao servidor
  ├── Recebe banner
  │
  └── Loop (interativo):
        ├── Lê expressão do teclado
        ├── send() → envia ao servidor
        ├── recv() → recebe resultado
        └── Exibe na tela
```

---

## A função `sscanf()` — Parse simples

No servidor, usamos `sscanf()` para extrair os componentes da expressão:

```c
double a, b;
char operador;
sscanf(expressao, "%lf %c %lf", &a, &operador, &b);
// "12.5 + 7.3" → a=12.5, operador='+', b=7.3
```

É uma abordagem simples e eficaz para um protocolo com formato fixo. Em cenários mais complexos, usaríamos um parser dedicado.

---

## Compilação

```bash
make              # compila servidor e cliente
make clean        # remove binários
```

Ou manualmente:

```bash
gcc -Wall -Wextra -o calc_servidor calc_servidor.c -lm
gcc -Wall -Wextra -o calc_cliente calc_cliente.c
```

> **Nota**: o servidor usa `-lm` (link com `libm`) para as funções `pow()` e `fmod()`.

---

## Execução

```bash
# Terminal 1 — servidor
./calc_servidor

# Terminal 2 — cliente
./calc_cliente
```

### Exemplo de sessão

```
Conectando a 127.0.0.1:9090...
========================================
  Calculadora Remota - Conectado!
  Formato: <num> <op> <num>
  Operadores: + - * / ^ %
  Digite 'sair' para desconectar.
========================================
calc> 10 + 3
= 13
calc> 100 / 7
= 14.2857
calc> 2 ^ 16
= 65536
calc> 10 / 0
ERRO: divisão por zero!
calc> sair
Até mais!
```

---

## O que observar neste projeto

1. **Protocolo de aplicação** — formato de mensagem definido (número-operador-número)
2. **Validação no servidor** — o servidor verifica operadores inválidos, divisão por zero etc.
3. **Sessão contínua** — o cliente mantém a conexão aberta para múltiplas operações (diferente da Parte 1, onde era uma mensagem e encerrava)
4. **Servidor sequencial** — atende um cliente por vez; quando termina, aceita o próximo

---

## Arquivos desta pasta

| Arquivo            | Descrição                                         |
|--------------------|---------------------------------------------------|
| `calc_servidor.c`  | Servidor que recebe expressões, calcula e responde |
| `calc_cliente.c`   | Cliente interativo tipo terminal                   |
| `Makefile`         | Compilação dos exemplos                            |
