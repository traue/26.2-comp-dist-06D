# Latência e Largura de Banda em MPI

## Introdução

Até aqui, trocamos mensagens entre processos sem nos perguntar quanto custa essa comunicação. No mundo real — especialmente quando os processos estão distribuídos em máquinas diferentes conectadas por rede — esse custo é uma das variáveis mais importantes do projeto de um programa paralelo. Um algoritmo que ignora a rede pode passar muito mais tempo trocando mensagens do que calculando.

Este diretório aborda esse custo de forma empírica: dois programas medem diretamente a **latência** e a **largura de banda** da comunicação MPI entre dois processos, usando o padrão clássico *ping-pong*. Os números resultantes mudam radicalmente dependendo de onde os dois processos estão rodando — na mesma máquina, em máquinas conectadas por Ethernet, por Wi-Fi, ou em uma rede dedicada de alto desempenho. Comparar essas configurações é o ponto pedagógico do exemplo.

---

## O modelo de custo de uma mensagem

Uma aproximação simples, mas muito usada, descreve o tempo para enviar uma mensagem de `n` bytes como:

```
T(n) = α + β · n
```

onde:

- **α** (alpha) é a **latência** — o custo fixo de enviar qualquer mensagem, independentemente do tamanho. Inclui o tempo de preparar cabeçalhos, entrar no kernel, atravessar pilhas de rede, propagar pelo meio físico, ser recebido do outro lado, sair do kernel e ser entregue à aplicação. Mesmo uma mensagem de 1 byte paga esse custo integralmente.
- **β** (beta) é o **inverso da largura de banda** — o custo por byte transmitido. A largura de banda efetiva (bytes por segundo) é `1/β`.

Para mensagens pequenas, `T ≈ α`: o custo é dominado pela latência, e duplicar o tamanho da mensagem não dobra o tempo. Para mensagens grandes, `T ≈ β · n`: o custo é dominado pelo tempo de transmissão, e a latência some no ruído. O ponto de transição entre os dois regimes é chamado de **message crossover** e depende da rede.

Esse modelo é o motivo pelo qual *agrupar* mensagens pequenas em uma mensagem grande quase sempre vale a pena em MPI: pagar a latência uma vez em vez de mil.

---

## O padrão ping-pong

Como medir α e β na prática? O truque é o ping-pong:

1. O processo 0 envia uma mensagem ao processo 1.
2. O processo 1 recebe e imediatamente devolve ao processo 0.
3. O processo 0 mede o tempo total dessa ida e volta (RTT, *round-trip time*).

Faz-se isso muitas vezes e tira-se a média, para diluir ruído de medição. Duas vantagens desse padrão:

- **Não precisa de relógios sincronizados**: o processo 0 mede antes do envio e depois da recepção, usando seu próprio relógio local. O cálculo da latência em um sentido é simplesmente RTT/2.
- **Funciona entre quaisquer duas máquinas**: é o teste mais elementar e portátil para caracterizar um canal de comunicação MPI.

Para medir **latência**, usa-se uma mensagem minúscula (1 byte) — assim o custo é dominado por α.

Para medir **largura de banda**, varia-se o tamanho da mensagem progressivamente (1B, 2B, 4B, …, até alguns MB) e observa-se quando o tempo passa a crescer linearmente com o tamanho. O regime linear revela β.

---

## Os exemplos neste diretório

### pingpong_latencia.c

Faz 10.000 iterações de ping-pong com mensagens de 1 byte entre os processos 0 e 1. Mede o tempo total, calcula o RTT médio e divide por 2 para obter a latência aproximada em um sentido.

O `MPI_Barrier` antes do laço garante que ambos os processos começam ao mesmo tempo — sem isso, um processo poderia começar a enviar antes do outro estar pronto para receber, e o primeiro `MPI_Recv` da medição incluiria o tempo de partida do processo mais lento, poluindo o número.

### pingpong_banda.c

Faz ping-pong repetido com mensagens de tamanhos crescentes, de 1 byte a 4 MB, sempre dobrando. Para cada tamanho, mede o tempo total de 1.000 iterações e calcula a largura de banda efetiva em MB/s.

Observe que o cálculo é `2 · ITERACOES · tam` bytes no numerador: cada iteração envolve uma ida **e** uma volta, então o volume total de dados trafegados é o dobro do tamanho de uma mensagem.

---

## Como compilar e executar

```bash
mpicc -O2 -o pingpong_latencia pingpong_latencia.c
mpicc -O2 -o pingpong_banda pingpong_banda.c
```

A flag `-O2` importa aqui: ela evita que o custo de cópias de buffer injete ruído na medição.

### Na mesma máquina (baseline local)

```bash
mpirun -np 2 ./pingpong_latencia
mpirun -np 2 ./pingpong_banda
```

Os dois processos vão se comunicar via memória compartilhada (transporte `sm` ou `vader` em Open MPI). Os números serão excelentes — latência tipicamente < 1 µs e banda > 5 GB/s — e refletem o "melhor caso": não há rede envolvida.

### Entre duas máquinas (caso realmente distribuído)

Configure o hostfile como explicado no diretório `05_rede_hostfile` e force um processo em cada máquina com `--map-by node`:

```bash
mpirun --hostfile hostfile --map-by node -np 2 ./pingpong_latencia
mpirun --hostfile hostfile --map-by node -np 2 ./pingpong_banda
```

Agora os números refletem a rede real: em Ethernet de 1 Gbps, espere latência de 50–200 µs e banda próxima de 110 MB/s (o teto teórico é 125 MB/s). Em Wi-Fi, a latência pode ser ordens de grandeza maior e muito mais instável entre execuções.

---

## O que observar nos resultados

Alguns comportamentos que o exemplo quase sempre deixa evidentes:

**A latência local é dramaticamente menor que a latência em rede.** Tipicamente duas ou três ordens de grandeza. Esse é o argumento central para minimizar comunicação entre máquinas sempre que possível, e para preferir algoritmos que se comunicam em volume em vez de em frequência.

**A largura de banda tem um "joelho" em algum tamanho de mensagem.** Abaixo desse tamanho, a curva `banda × tamanho` cresce — você está pagando muita latência por pouco byte. Acima dele, a banda estabiliza próxima do teto físico da rede. Esse joelho costuma ficar entre 4 KB e 64 KB, e marca o tamanho a partir do qual a rede está sendo usada eficientemente.

**A banda local tem seu próprio teto — e ele não é infinito.** Mesmo em memória compartilhada, a largura de banda é limitada pela velocidade da memória e do cache. Ainda assim, costuma ser 10× a 100× superior à de uma rede Gigabit.

**Há mudança de protocolo interna ao MPI.** Implementações como Open MPI usam protocolos diferentes para mensagens pequenas (*eager* — envia direto, assumindo que cabe no buffer do receptor) e grandes (*rendezvous* — faz handshake antes de transferir). A troca entre protocolos costuma aparecer como uma "quebra" visível na curva de banda, normalmente em torno de 64 KB.

---

## Exercícios sugeridos

Esses programas são ótimos pontos de partida para explorar mais:

1. **Compare configurações**: rode os dois programas local, entre máquinas cabeadas e entre máquinas no Wi-Fi. Tabule os números e discuta as diferenças.
2. **Varie o número de iterações**: reduza `ITERACOES` para 100 e repita. Observe se os números ficam instáveis — isso mostra a importância de médias longas.
3. **Force protocolo eager ou rendezvous**: com Open MPI, `--mca btl_tcp_eager_limit 1000000` empurra o crossover para cima. Rode antes e depois e veja a curva mudar de forma.
4. **Compare com mensagens não bloqueantes**: substitua `MPI_Send/Recv` por `MPI_Isend/Irecv` + `MPI_Waitall` e veja se há diferença mensurável. Normalmente não há ganho em ping-pong puro, porque não há computação para sobrepor — mas é instrutivo confirmar experimentalmente.
5. **Meça variância**: faça o programa rodar 10 vezes seguidas e reporte mínimo, máximo e desvio padrão. A rede raramente entrega números iguais entre execuções.

---

## Resumo

| Grandeza           | Símbolo | O que significa                                | Como medimos                  |
|--------------------|---------|------------------------------------------------|-------------------------------|
| Latência           | α       | Custo fixo por mensagem, independente do tamanho | Ping-pong com msg de 1 byte   |
| Largura de banda   | 1/β     | Bytes por segundo no regime assintótico         | Ping-pong variando o tamanho  |
| RTT                | —       | Tempo total de ida e volta                      | Medido diretamente            |
| Crossover          | —       | Tamanho onde α e β·n se igualam                 | Onde a curva de banda "joelha" |

Entender α e β de um canal é o que permite decidir racionalmente entre algoritmos paralelos alternativos: um algoritmo com menos mensagens maiores geralmente vence um com mais mensagens menores, e esses programas mostram exatamente por quê.
