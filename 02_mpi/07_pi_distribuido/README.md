# Cálculo Distribuído de π por Monte Carlo

## Introdução

Este diretório junta tudo que foi construído até agora — operações ponto a ponto, operações coletivas, e execução em rede — em um único programa útil: o cálculo de π distribuído entre vários nós conectados por rede.

A abordagem é o **método de Monte Carlo**, diferente da integração numérica vista no diretório `03_ops-coletivas`. Em vez de somar áreas de retângulos sob uma curva, jogamos dardos aleatórios em um quadrado e contamos quantos caem dentro de um círculo. É um algoritmo mais intuitivo, mais fácil de paralelizar, e — o mais importante para este exemplo — *embaraçosamente paralelo*: cada processo trabalha de forma totalmente independente, só falando com os outros uma única vez no final para consolidar o resultado.

Essa última propriedade é o que faz desse programa um candidato ideal para rede: a comunicação é mínima, então o custo adicional de distribuir em várias máquinas é pequeno, e o ganho de desempenho costuma ser próximo do ideal.

---

## O algoritmo de Monte Carlo para π

A ideia geométrica é simples. Considere o quadrado de lado 1, com vértices em (0,0) e (1,1). Dentro dele, cabe um quarto de círculo de raio 1 centrado em (0,0). As áreas são:

- Quadrado: 1 × 1 = 1
- Quarto de círculo: π · 1² / 4 = π/4

Se sortearmos pontos uniformemente dentro do quadrado, a probabilidade de um ponto cair dentro do quarto de círculo é exatamente π/4. Com N pontos sorteados e D deles caindo dentro:

```
π ≈ 4 · D / N
```

Quanto maior N, melhor a aproximação. A taxa de convergência do Monte Carlo é `O(1/√N)`, que é lenta — para ganhar um dígito decimal de precisão, é preciso multiplicar N por 100. Essa é justamente a razão pela qual paralelizar faz diferença: para chegar a boa precisão precisamos de muitos pontos, e jogá-los em paralelo é fácil.

### Por que é embaraçosamente paralelo

Cada ponto é sorteado e testado de forma totalmente independente dos outros. Não há ordem, não há dependência, não há estado compartilhado. Para dividir o trabalho entre P processos, basta cada um sortear N/P pontos localmente. Ao final, um único `MPI_Reduce` soma os acertos.

Compare com a integração numérica de π: lá também cada processo faz sua fatia independente, mas o particionamento do domínio precisa ser explícito (cada processo cuida de um intervalo específico de x). Aqui nem isso — os domínios são iguais e o que varia é só o sorteio.

---

## Como o programa funciona

O fluxo é:

1. O processo 0 lê o total de pontos da linha de comando (ou usa o padrão de 100 milhões).
2. `MPI_Bcast` distribui esse total para todos os processos.
3. Cada processo calcula sua fatia: `pontos_locais = total / size`. O processo 0 absorve qualquer resto da divisão, para que a soma das fatias seja exatamente igual ao total pedido.
4. Cada processo **semeia seu gerador de números aleatórios de forma diferente** — se todos usassem a mesma semente, todos sorteariam exatamente os mesmos pontos, o que destruiria completamente a utilidade do paralelismo. A semente combina `time(NULL)` com o rank multiplicado por um primo.
5. Cada processo sorteia seus pontos, conta quantos caíram dentro do quarto de círculo, e guarda o resultado local.
6. `MPI_Reduce` com `MPI_SUM` agrega a contagem no processo 0.
7. O processo 0 calcula `π ≈ 4 · dentro / total` e imprime o resultado, o erro e os tempos.

Observe que o tempo é medido em dois pontos: logo após o laço de cálculo (`t_fim_calculo`) e logo após o `MPI_Reduce` (`t_fim_total`). Em um programa embaraçosamente paralelo como este, os dois valores devem ser muito próximos — se não forem, algo está consumindo tempo na comunicação. É um bom diagnóstico para detectar problemas de rede.

### Detalhe importante: o gargalo do rand()

A função `rand()` da libc não é a mais rápida nem a de melhor qualidade estatística, mas é suficiente para este exemplo didático. Em código de produção, geradores como `xoshiro256**` ou `pcg` são ordens de grandeza melhores em ambos os aspectos. Outra armadilha: `rand()` usa um estado global por *thread*, então, em programas com threads, o acesso a `rand()` precisa ser serializado. Em MPI puro (sem OpenMP, sem pthreads) isso não é problema porque cada processo tem seu próprio espaço de endereçamento e, portanto, seu próprio estado de `rand()`.

---

## Como executar

### Modo local (uma única máquina)

Começar localmente serve para verificar que o programa funciona antes de envolver a rede:

```bash
mpicc -O2 -o pi_montecarlo pi_montecarlo.c -lm

# Padrao: 100 milhoes de pontos
mpirun -np 4 ./pi_montecarlo

# Especificando outro total
mpirun -np 4 ./pi_montecarlo 1000000000
```

Os quatro processos serão processos do sistema operacional local se comunicando por memória compartilhada. A saída mostra que todos rodaram na mesma máquina (mesmo hostname em todas as linhas).

### Modo distribuído em rede

Com o hostfile configurado (conforme o diretório `05_rede_hostfile`):

```bash
# Usa o mapeamento padrao: preenche slots de uma maquina antes da proxima
mpirun --hostfile hostfile -np 8 ./pi_montecarlo 1000000000

# Forca um processo por maquina — util com poucos processos para
# garantir que o calculo realmente se espalhe em rede
mpirun --hostfile hostfile --map-by node -np 4 ./pi_montecarlo 1000000000
```

A saída deixará claro que processos diferentes rodaram em máquinas diferentes:

```
Rank 0 em no01.local: sorteando 250000000 pontos
Rank 1 em no02.local: sorteando 250000000 pontos
Rank 2 em no03.local: sorteando 250000000 pontos
Rank 3 em no01.local: sorteando 250000000 pontos
...
```

---

## Executando o programa em cada nó — o que o MPI faz por trás

Quando você dispara o `mpirun` a partir do nó frontend, não é você que precisa abrir terminal em cada máquina e iniciar processos. O MPI faz isso automaticamente por SSH. Ainda assim, é instrutivo entender *o que* está sendo feito, porque é exatamente o que acontece nos bastidores:

Para cada entrada do hostfile, o `mpirun` executa algo equivalente a:

```bash
ssh no02.local "cd /caminho/do/programa && ./pi_montecarlo"
```

O processo local em cada máquina é filho desse SSH. Depois que todos estão vivos, eles abrem conexões TCP diretas entre si (não passando mais pelo `mpirun`) e trocam mensagens por essas conexões. O `mpirun` na máquina frontend só coleta a saída padrão de cada processo e fica observando a conclusão.

### Para ver isso acontecendo ao vivo

Em uma das máquinas do cluster, antes de disparar o `mpirun` do frontend, abra dois terminais:

```bash
# Terminal A — monitora processos MPI
watch -n 0.5 'pgrep -af pi_montecarlo'

# Terminal B — monitora conexoes de rede do programa
watch -n 0.5 'ss -tnp | grep pi_montecarlo'
```

Ao disparar o `mpirun` do frontend, o terminal A mostrará o processo aparecendo, o terminal B mostrará conexões TCP estabelecidas com as outras máquinas, e ambos desaparecerão quando o cálculo terminar. Esse é o momento em que "MPI em rede" deixa de ser abstrato.

### Executando manualmente (raríssimo, mas ilustrativo)

Existe um modo raramente usado em que você lança os processos manualmente em cada máquina e os conecta via um servidor de nomes. Isso é conhecido como *singleton init* com *connect-accept*, e fica fora do escopo deste exemplo. O ponto pedagógico é apenas: **não precisa**. O `mpirun` com hostfile já resolve. Se alguém estiver pensando "preciso abrir terminal em cada máquina e rodar o programa à mão", é sinal de que o hostfile não está configurado corretamente.

---

## Observando o ganho de desempenho

Esse programa é ideal para medir **speedup** porque a comunicação é mínima. Execute a mesma carga com números crescentes de processos e compare os tempos de cálculo:

```bash
# Fixar uma carga grande o bastante para o tempo ser mensuravel
N=2000000000

mpirun -np 1 ./pi_montecarlo $N
mpirun -np 2 ./pi_montecarlo $N
mpirun -np 4 ./pi_montecarlo $N
mpirun --hostfile hostfile -np 8 ./pi_montecarlo $N
mpirun --hostfile hostfile -np 16 ./pi_montecarlo $N
```

Em um algoritmo verdadeiramente embaraçosamente paralelo, o speedup ideal é linear: dobrar o número de processos deveria cortar o tempo pela metade. Na prática, há três razões pelas quais isso não acontece exatamente:

1. **Overhead de inicialização do MPI** — `MPI_Init` e o handshake entre processos levam um tempo fixo. Para cargas pequenas, ele domina.
2. **Contenção de recursos dentro de uma máquina** — quando dois processos dividem o mesmo núcleo físico (via hyperthreading, por exemplo), cada um vai a menos de 100% da velocidade de um núcleo exclusivo.
3. **O `MPI_Reduce` final custa algo** — em rede, é um tempo não-desprezível, e cresce logaritmicamente com o número de processos.

Ainda assim, para cargas grandes, é comum obter eficiência (speedup / número de processos) acima de 90% nesse tipo de programa. Um bom exercício é construir uma tabela:

| Processos | Tempo (s) | Speedup | Eficiência |
|-----------|-----------|---------|------------|
| 1         | ...       | 1.00    | 100%       |
| 2         | ...       | ...     | ...        |
| 4         | ...       | ...     | ...        |
| 8         | ...       | ...     | ...        |

O speedup é `T₁/Tₚ`; a eficiência é `speedup / p`.

---

## Observando a precisão

Cada execução dá um valor ligeiramente diferente de π, porque os pontos são aleatórios. Para ver a convergência `O(1/√N)`, execute com cargas progressivamente maiores:

```bash
for N in 1000 10000 100000 1000000 10000000 100000000 1000000000; do
    mpirun -np 4 ./pi_montecarlo $N 2>/dev/null | grep -E "(Pi estimado|Erro)"
done
```

Cada aumento de 100× em N deve reduzir o erro tipicamente em 10× (não exatos, porque é estatístico). Isso é a marca registrada do Monte Carlo: robusto, fácil de paralelizar, mas lento a convergir.

---

## Comparativo com `pi_mpi.c` do diretório 03

| Aspecto                       | `pi_mpi.c` (integração)          | `pi_montecarlo.c` (Monte Carlo)     |
|-------------------------------|----------------------------------|--------------------------------------|
| Estratégia                    | Regra do ponto médio             | Sorteio aleatório                    |
| Convergência                  | O(1/n²) — muito rápida           | O(1/√n) — lenta                      |
| Determinismo                  | Mesmo resultado sempre           | Resultado varia entre execuções      |
| Paralelismo                   | Divide domínio, soma parcial     | Divide pontos, soma acertos          |
| Comunicação                   | Bcast + Reduce                   | Bcast + Reduce                       |
| Facilidade de distribuir      | Alta                             | Muito alta (embaraçosamente paralelo) |
| Dependência de aleatoriedade  | Nenhuma                          | Central — exige sementes distintas   |

Ambos chegam a π. A integração é o caminho correto se você quer precisão alta rápido. O Monte Carlo é o caminho correto se o problema *real* é estocástico — simulações físicas, finanças, sistemas estatísticos —, casos em que a forma fechada da integral não existe.

---

## Resumo

O programa demonstra, em um único código curto:

- **Broadcast** para distribuir o parâmetro de entrada.
- **Cálculo local independente** com sementes de aleatoriedade distintas por rank.
- **Redução** para consolidar o resultado.
- **Medição de tempo** para observar escalabilidade.
- **Identificação de nós** para confirmar que o trabalho está realmente distribuído em rede.

É o tipo de programa que, em cursos de computação distribuída, costuma ser o primeiro experimento real em um cluster — porque junta conceitos vistos separadamente e produz um resultado que o aluno consegue comparar com um valor conhecido (π) e com uma métrica de desempenho concreta (speedup).
