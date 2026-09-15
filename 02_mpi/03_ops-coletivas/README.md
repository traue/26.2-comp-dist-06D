# Operações Coletivas em MPI

## Introdução

Em programação paralela com MPI, a maior parte dos tutoriais começa mostrando como dois processos trocam mensagens com `MPI_Send` e `MPI_Recv`. Isso funciona bem quando a comunicação é pontual entre dois participantes específicos. Mas, na prática, muitos algoritmos paralelos precisam que todos os processos participem de uma mesma operação ao mesmo tempo: distribuir dados, coletar resultados parciais, ou sincronizar o estado compartilhado. É para isso que existem as operações coletivas.

Uma operação coletiva envolve **todos os processos de um comunicador**. Quando um processo chama `MPI_Bcast`, por exemplo, todos os outros também precisam chamar `MPI_Bcast` — caso contrário, o programa trava indefinidamente. Essa é a regra central: toda operação coletiva exige participação de todos, sem exceção.

Vale destacar uma consequência prática disso: operações coletivas implicam sincronização implícita entre os processos. Eles precisam chegar ao mesmo ponto da execução antes de a operação completar. Isso não é necessariamente ruim — em muitos casos é exatamente o comportamento desejado — mas é importante ter consciência ao projetar o programa.

---

## As operações abordadas

### MPI_Bcast — Broadcast

O broadcast é a operação coletiva mais simples. Um processo (chamado de *root*) possui um dado e quer enviá-lo para todos os demais. Com `MPI_Send` e `MPI_Recv`, seria necessário um loop enviando para cada processo individualmente. Com `MPI_Bcast`, o root envia uma vez e o MPI cuida de distribuir para todos.

```c
MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
```

Nessa chamada: `&n` é o buffer, `1` é a quantidade de elementos, `MPI_INT` é o tipo, `0` é o rank do root, e `MPI_COMM_WORLD` é o comunicador. Todos os processos executam exatamente essa linha — o root preenche o buffer antes de chamá-la, os demais a chamam para receber o valor.

### MPI_Reduce — Redução

A redução é o oposto lógico do broadcast. Em vez de distribuir dados de um para todos, ela coleta. Cada processo tem um valor local e contribui para uma operação — soma, máximo, produto, etc. — cujo resultado vai para o root.

```c
MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
```

Os parâmetros: `&mypi` é o valor que este processo contribui, `&pi` é onde o resultado vai (relevante apenas no root), `1` é a quantidade de elementos, `MPI_DOUBLE` é o tipo, `MPI_SUM` é a operação, e `0` é o root.

O MPI fornece várias operações prontas: `MPI_SUM`, `MPI_MAX`, `MPI_MIN`, `MPI_PROD`, `MPI_LAND`, entre outras. É possível também definir operações personalizadas com `MPI_Op_create`.

### MPI_Scatter e MPI_Gather — Distribuição e Coleta

`MPI_Scatter` e `MPI_Gather` são complementares. O scatter divide um vetor que está no root e distribui partes iguais para todos os processos. O gather faz o caminho inverso: coleta dados de cada processo de volta para o root.

A diferença em relação ao `MPI_Bcast` é que o scatter fragmenta os dados — cada processo recebe uma fatia diferente, não uma cópia do mesmo dado.

```c
// Distribui partes do vetor 'data' para cada processo
MPI_Scatter(data, chunk_size, MPI_INT,
            local_data, chunk_size, MPI_INT,
            0, MPI_COMM_WORLD);
```

O primeiro trio de argumentos descreve o buffer de envio, que só é relevante no root. O segundo trio descreve o buffer de recepção em cada processo. `0` é o root.

Depois do processamento local, o gather reúne os resultados:

```c
MPI_Gather(&local_sum, 1, MPI_INT,
           partial_sums, 1, MPI_INT,
           0, MPI_COMM_WORLD);
```

O padrão `Scatter → Processamento local → Gather` é um dos mais recorrentes em computação paralela. É a forma natural de dividir um problema, resolver cada parte independentemente, e reunir o resultado.

### MPI_Allgather — Gather para Todos

O `MPI_Allgather` funciona como o `MPI_Gather`, mas em vez de entregar o resultado apenas ao root, entrega para **todos os processos**. Cada processo contribui com um valor e, ao final da operação, todos têm o vetor completo com os valores de cada um.

```c
MPI_Allgather(&local_max, 1, MPI_INT,
              all_max_values, 1, MPI_INT,
              MPI_COMM_WORLD);
```

Note que não há parâmetro de root — não faz sentido tê-lo, já que o resultado vai para todos.

A diferença em relação ao `MPI_Reduce` com `MPI_MAX` é sutil mas importante: com `MPI_Allgather` todos os processos têm acesso a todos os valores individuais, não apenas ao resultado final da operação. Isso é útil quando o processamento subsequente depende de conhecer os valores de cada processo, e não só o extremo ou a soma.

---

## Os exemplos neste diretório

### soma_reduce.c

O ponto de partida. Cada processo define um valor local igual a `rank + 1`. O `MPI_Reduce` soma todos esses valores no processo 0. Com quatro processos, o resultado é 1 + 2 + 3 + 4 = 10. É o exemplo mínimo para entender como a redução funciona antes de complicar com algoritmos maiores.

### pi.c e pi_mpi.c

Um par de programas que calcula π usando integração numérica pela regra do ponto médio. A ideia é aproximar a integral de `4 / (1 + x²)` de 0 a 1, que converge exatamente para π.

`pi.c` é a versão sequencial — roda com 9 bilhões de intervalos num único processo e serve como referência de resultado e tempo. `pi_mpi.c` paraleliza esse cálculo: o processo 0 lê o número de intervalos via `scanf` e usa `MPI_Bcast` para distribuir o valor para todos. Cada processo então calcula sua fatia da soma (usando o rank como deslocamento no loop), e `MPI_Reduce` com `MPI_SUM` agrega tudo no processo 0, que imprime o resultado e o erro em relação ao valor real.

O padrão aqui — broadcast do parâmetro de entrada, cálculo paralelo independente, reduce para agregar — é um dos mais recorrentes em programação paralela numérica.

### soma_parelela.c

Demonstra o ciclo completo scatter → processamento → gather. O processo 0 cria um vetor com os valores de 1 a 100. O `MPI_Scatter` distribui fatias iguais para todos os processos. Cada processo soma seus elementos localmente. O `MPI_Gather` coleta as somas parciais no processo 0, que as soma para obter o total. Com qualquer número de processos que divida 100 igualmente, o resultado deve ser sempre 5050 — o que serve como verificação natural da correção do programa.

### maximo_paralelo.c

Neste exemplo, cada processo gera seu próprio vetor de números aleatórios (usando o rank como semente do gerador, para que os valores sejam diferentes entre processos) e encontra o máximo local. O `MPI_Allgather` compartilha esses máximos locais entre todos, de modo que cada processo pode calcular o máximo global de forma independente.

O vetor tem 42 elementos divididos igualmente entre os processos. O número de processos deve, portanto, ser um divisor de 42.

---

## Como compilar e executar

Todos os programas usam o compilador `mpicc`, que é o wrapper do GCC para MPI:

```bash
# Compilar
mpicc -o soma_reduce soma_reduce.c
mpicc -o pi_mpi pi_mpi.c -lm
mpicc -o soma_parelela soma_parelela.c
mpicc -o maximo_paralelo maximo_paralelo.c

# Executar
mpirun -np 4 ./soma_reduce
mpirun -np 4 ./pi_mpi       # solicita o número de intervalos como entrada
mpirun -np 4 ./soma_parelela
mpirun -np 6 ./maximo_paralelo
```

Para o `pi_mpi`, um valor de `1000000` já é suficiente para obter uma aproximação bem próxima de π. Aumentar esse número melhora a precisão, mas o ganho diminui rapidamente.

Para o `maximo_paralelo`, use um número de processos que divida 42 exatamente: 1, 2, 3, 6, 7, 14, 21 ou 42.

---

## Resumo das operações

| Operação        | Sentido                    | Resultado disponível em |
|-----------------|----------------------------|--------------------------|
| `MPI_Bcast`     | Root para todos            | Todos                    |
| `MPI_Scatter`   | Root para todos (dividido) | Todos (partes diferentes)|
| `MPI_Gather`    | Todos para root            | Root                     |
| `MPI_Allgather` | Todos para todos           | Todos                    |
| `MPI_Reduce`    | Todos para root (operação) | Root                     |
