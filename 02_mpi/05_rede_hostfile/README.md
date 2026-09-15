# MPI em Rede — Hostfile e Execução Distribuída

## Introdução

Até aqui, todos os exemplos foram executados com `mpirun -np N ./programa` em uma única máquina. Os "processos" eram apenas processos do sistema operacional local, se comunicando através de memória compartilhada ou sockets internos. Essa configuração é suficiente para aprender a API, mas deixa de fora o que torna o MPI realmente interessante: a capacidade de distribuir o cálculo entre várias máquinas físicas conectadas em rede.

Este diretório mostra como dar esse passo. O programa em si é trivial — cada processo apenas imprime quem é e onde está rodando. O que muda é a infraestrutura ao redor: configuração de SSH, definição de um arquivo de hostnames, e a forma de invocar o `mpirun`.

---

## Como o MPI "espalha" processos pela rede

Quando executamos `mpirun -np 8 ./programa` em uma única máquina, o `mpirun` simplesmente faz `fork` local de 8 processos. Quando há várias máquinas envolvidas, o mecanismo é outro: o `mpirun` usa SSH (ou outro remote launcher) para abrir sessões nas máquinas remotas e iniciar os processos lá. Depois que todos estão vivos, eles se conectam entre si via TCP para trocar mensagens.

Três ingredientes precisam estar em ordem para que isso funcione:

1. **SSH sem senha** entre as máquinas (via chave pública).
2. **Mesmo binário** disponível no mesmo caminho em todas as máquinas.
3. **Um arquivo de hostfile** que diga ao `mpirun` quais máquinas usar e quantos slots cada uma oferece.

Se qualquer um desses três faltar, a execução falha — geralmente com uma mensagem do SSH pedindo senha, ou com erro de "binary not found", ou com processos que nunca chegam a se conectar.

---

## Passo 1 — SSH sem senha

O `mpirun` abre conexões SSH programaticamente, sem possibilidade de digitar senha. A solução padrão é autenticação por chave pública.

Na máquina onde o `mpirun` será executado (chame-a de "frontend"):

```bash
# Gera o par de chaves, se ainda não existir
ssh-keygen -t ed25519

# Copia a chave pública para cada máquina do cluster
ssh-copy-id usuario@no01.local
ssh-copy-id usuario@no02.local
ssh-copy-id usuario@no03.local
```

Depois, teste: `ssh usuario@no01.local hostname` precisa responder sem pedir senha. Se pedir, o `mpirun` também vai falhar.

Nota: o usuário precisa ser o mesmo em todas as máquinas (ou é preciso configurar `~/.ssh/config` para mapear). Do contrário, o MPI não sabe com que credencial entrar nas máquinas remotas.

---

## Passo 2 — Mesmo binário em todas as máquinas

Quando o `mpirun` abre uma sessão SSH em `no02`, ele tenta executar `./hello_hostname` **no sistema de arquivos daquela máquina**, não no da máquina local. O binário precisa existir lá, no mesmo caminho.

Há três estratégias comuns:

- **Copiar manualmente** com `scp` após cada compilação (funciona, mas é tedioso).
- **Compartilhar um diretório via NFS** entre as máquinas (a configuração mais comum em clusters de laboratório — o home do usuário é montado por NFS e o binário aparece em todo lugar automaticamente).
- **Usar `rsync`** em um script que sincroniza o diretório após cada `mpicc`.

Para testes rápidos, `scp` resolve:

```bash
mpicc -o hello_hostname hello_hostname.c
for host in no01.local no02.local no03.local; do
    scp hello_hostname "$host":~/
done
```

---

## Passo 3 — Escrever o hostfile

O hostfile é um arquivo de texto que lista as máquinas e quantos slots cada uma oferece. Um slot é uma unidade de execução — em geral, configurado igual ao número de núcleos da máquina.

O arquivo `hostfile.example` deste diretório mostra o formato:

```
no01.local slots=4
no02.local slots=4
no03.local slots=2
```

Com esse hostfile, o MPI pode rodar até 10 processos no total (4 + 4 + 2). Se você pedir `-np 10`, cada máquina recebe exatamente seus slots. Se pedir menos, o MPI preenche as máquinas em ordem. Se pedir mais, ele recusa — a menos que você passe `--oversubscribe`, que autoriza colocar mais processos do que slots (útil apenas para exercícios didáticos; em produção, degrada o desempenho).

---

## Passo 4 — Executar

Com SSH funcionando, binário distribuído e hostfile pronto, a execução é:

```bash
mpirun --hostfile hostfile.example -np 8 ./hello_hostname
```

A saída deve ser algo como:

```
Processo 0 de 8 rodando em no01.local (MPI: no01.local)
Processo 1 de 8 rodando em no01.local (MPI: no01.local)
Processo 2 de 8 rodando em no01.local (MPI: no01.local)
Processo 3 de 8 rodando em no01.local (MPI: no01.local)
Processo 4 de 8 rodando em no02.local (MPI: no02.local)
Processo 5 de 8 rodando em no02.local (MPI: no02.local)
Processo 6 de 8 rodando em no02.local (MPI: no02.local)
Processo 7 de 8 rodando em no02.local (MPI: no02.local)
```

Os primeiros quatro foram para `no01` porque ela tem 4 slots; os quatro seguintes foram para `no02`. A ordem das linhas impressas pode variar, já que todos os processos imprimem simultaneamente, mas o mapeamento rank → máquina é determinístico.

---

## O programa em si — hello_hostname.c

O código é propositalmente mínimo. A única adição em relação ao "hello world" do diretório `01_hello` é descobrir em que máquina o processo está rodando. Isso é feito de duas formas, para ilustrar ambas:

- `gethostname()` é uma chamada POSIX padrão que retorna o hostname do sistema operacional.
- `MPI_Get_processor_name()` é o equivalente MPI, que em tese pode retornar algo diferente (por exemplo, em clusters com múltiplas redes, o nome "visto pelo MPI" pode diferir do hostname do SO). Na prática, em clusters simples, os dois coincidem.

O ponto do exemplo é visual: ao rodar entre máquinas, os nomes impressos comprovam que os processos realmente estão distribuídos. Se todos os processos imprimirem o mesmo hostname, algo está errado — o `mpirun` caiu em modo local.

---

## Variantes úteis de invocação

Algumas flags do `mpirun` são especialmente úteis ao trabalhar em rede:

```bash
# Listar hostnames diretamente na linha de comando (sem arquivo)
mpirun -H no01.local,no02.local -np 4 ./hello_hostname

# Forçar distribuição "round-robin" entre máquinas (um processo em cada
# antes de preencher a próxima), em vez de preencher por slots
mpirun --hostfile hostfile.example --map-by node -np 6 ./hello_hostname

# Ver exatamente onde cada rank foi mapeado (útil para depuração)
mpirun --hostfile hostfile.example -np 8 --display-map ./hello_hostname

# Rodar sem hostfile, usando um único host remoto
mpirun -H no01.local -np 4 ./hello_hostname
```

A opção `--map-by node` é interessante do ponto de vista didático: com ela, o rank 0 fica em `no01`, o rank 1 em `no02`, o rank 2 em `no03`, o rank 3 volta para `no01`, e assim por diante. Isso muda o padrão de comunicação — agora comunicação entre ranks consecutivos atravessa a rede, enquanto no mapeamento por slots ranks consecutivos geralmente estão na mesma máquina (memória compartilhada). O exemplo do diretório `06_latencia_banda` deixa essa diferença visível em números.

---

## Problemas comuns e como diagnosticar

| Sintoma                                          | Causa provável                                         |
|--------------------------------------------------|--------------------------------------------------------|
| SSH pede senha ao iniciar                        | Chave pública não está no `authorized_keys` remoto     |
| "binary not found" em uma das máquinas           | Binário não foi copiado / NFS não está montado          |
| Processos travam na inicialização (MPI_Init)     | Firewall bloqueando portas TCP entre as máquinas       |
| Todos os processos imprimem o hostname local     | `mpirun` ignorou o hostfile (verifique o caminho dele) |
| Erro "There are not enough slots"                | Pediu mais processos que slots; use `--oversubscribe`  |
| Versões do MPI diferentes entre máquinas         | Recompile todo mundo com a mesma implementação         |

Esse último ponto merece ênfase: o Open MPI e o MPICH não são compatíveis binariamente entre si. Se uma máquina tem Open MPI 4.1 e outra tem MPICH 4.0, a execução vai falhar. Mantenha a mesma versão da mesma implementação em todo o cluster.

---

## Como compilar e executar

```bash
# Em cada máquina (ou uma vez só, se o diretório for compartilhado via NFS):
mpicc -o hello_hostname hello_hostname.c

# Copie o hostfile.example para hostfile e edite com seus hostnames reais
cp hostfile.example hostfile
# ... edite ...

# Execute
mpirun --hostfile hostfile -np 8 ./hello_hostname
```

Para verificar que está realmente distribuindo, basta contar quantos hostnames diferentes aparecem na saída — precisa bater com o número de máquinas no hostfile (considerando quantos processos você pediu).
