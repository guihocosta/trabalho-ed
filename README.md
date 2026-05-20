# Campeonato Computacional de Futebol — Parte I

Sistema de gerenciamento de partidas e classificação de um campeonato de futebol, desenvolvido em linguagem C como trabalho prático da disciplina de Estruturas de Dados do IFES Campus Serra.

---

## Requisitos

- GCC 11 ou superior
- `make`

---

## Estrutura do repositório

```
.
├── main.c                        # Ponto de entrada do programa
├── time.c / time.h               # TAD Time
├── bd_times.c / bd_times.h       # TAD BDTimes
├── partida.c / partida.h         # TAD Partida
├── bd_partidas.c / bd_partidas.h # TAD BDPartidas
├── utils.c / utils.h             # Funções utilitárias
├── Makefile                      # Regras de compilação
├── times.csv                     # Banco de dados de times
├── partidas_vazio.csv            # Cenário 1: campeonato sem partidas
├── partidas_parcial.csv          # Cenário 2: campeonato em andamento
└── partidas_completo.csv         # Cenário 3: campeonato finalizado
```

---

## Como compilar e executar

### Compilar e executar de uma vez

```bash
make run
```

### Apenas compilar

```bash
make compile
```

### Limpar arquivos gerados

```bash
make clean
```

---

## Arquivos de dados

O sistema utiliza os arquivos CSV do repositório para carregar os dados de times e partidas na inicialização.

---

## Como usar o sistema

Ao iniciar, o sistema exibe o menu principal:

```
===== Sistema de Gerenciamento de Partidas =====
  1 - Consultar time
  2 - Consultar partidas
  3 - Atualizar partida  (desabilitado)
  4 - Remover partida    (desabilitado)
  5 - Inserir partida    (desabilitado)
  6 - Imprimir tabela de classificacao
  Q - Sair
================================================
```

### Opção 1 — Consultar time

Solicita um nome ou prefixo e exibe as estatísticas de todos os times correspondentes.

```
Digite o nome ou prefixo do time: Se

ID   Time              V   E   D   GM   GS    S   PG
2    SemCTRL          11   2   5   43   34    9   35
5    SeQueLas          7   3   8   50   49    1   24
```

### Opção 2 — Consultar partidas

Permite buscar partidas por time mandante, visitante ou qualquer um dos dois. A busca é feita por prefixo do nome.

```
Escolha o modo de consulta:
  1 - Por time mandante
  2 - Por time visitante
  3 - Por time mandante ou visitante
  4 - Retornar ao menu principal
Opcao: 3

Digite o nome: RUSTicos

ID   Time1           Time2          
5    SeQueLas        5 x 2 RUSTicos
17   PYthons         5 x 5 RUSTicos
27   RUSTicos        3 x 4 SemCTRL
28   RUSTicos        2 x 4 NETunos
35   RUSTicos        1 x 5 ESCorpiões
37   REACTivos       4 x 0 RUSTicos
43   NETunos         3 x 1 RUSTicos
```

### Opção 6 — Imprimir tabela de classificação

Exibe todos os times ordenados por ID com suas estatísticas acumuladas.

```
Imprimindo classificacao...

ID   Time            V   E   D   GM   GS    S   PG
0    JAVAlis         13  3   2   58   30   28   42
1    ESCorpiões      10  2   6   55   39   16   32
2    SemCTRL         11  2   5   43   34    9   35
...
```

### Q — Sair

Encerra o sistema e libera toda a memória alocada.

---

## Principais TADs

### `Time` (`time.h` / `time.c`)

Representa uma equipe de futebol. Armazena identificador, nome e estatísticas de desempenho (vitórias, empates, derrotas, gols marcados e sofridos). Pontos ganhos e saldo de gols são calculados sob demanda pelas funções `time_calcular_pontos` e `time_calcular_saldo_gols`, garantindo consistência dos dados. As estatísticas são atualizadas via `time_adicionar_resultados` a cada partida processada.

| Função | Descrição |
|---|---|
| `time_criar` | Aloca e inicializa um time com estatísticas zeradas |
| `time_adicionar_resultados` | Atualiza vitórias, empates, derrotas e gols após uma partida |
| `time_calcular_pontos` | Retorna PG = 3×V + E |
| `time_calcular_saldo_gols` | Retorna S = GM − GS |
| `time_imprimir` | Imprime a linha formatada do time na tabela |
| `time_free` | Libera a memória do time |

---

### `BDTimes` (`bd_times.h` / `bd_times.c`)

Gerencia a coleção de todos os times. Carrega os dados do arquivo `times.csv` e cria uma instância de `Time` para cada registro. Fornece busca por ID e por nome (prefixo), além de listagem completa da tabela.

| Função | Descrição |
|---|---|
| `bdt_criar` | Aloca o gerenciador |
| `bdt_carregar` | Lê `times.csv` e popula a coleção |
| `bdt_buscar_por_id` | Retorna o time com o ID informado |
| `bdt_buscar_por_nome` | Retorna o primeiro time cujo nome bate com o prefixo |
| `bdt_imprimir_tabela` | Imprime todos os times formatados |
| `bdt_get_largura_nome_max` | Retorna a maior largura de nome para alinhamento |
| `bdt_get_qtd` | Retorna a quantidade de times carregados |
| `bdt_get_time_idx` | Retorna o time pelo índice no vetor interno |
| `bdt_free` | Libera toda a memória da coleção |

---

### `Partida` (`partida.h` / `partida.c`)

Representa um jogo entre dois times, encapsulando ID da partida, IDs dos times e placar. É a estrutura elementar manipulada pelo `BDPartidas`.

| Função | Descrição |
|---|---|
| `partida_criar` | Aloca e inicializa uma partida |
| `partida_imprimir` | Imprime o placar formatado usando os nomes do `BDTimes` |
| `partida_free` | Libera a memória da partida |

---

### `BDPartidas` (`bd_partidas.h` / `bd_partidas.c`)

Gerencia a coleção de todas as partidas. Ao carregar as partidas, além de armazenar cada partida, já repassa os resultados ao `BDTimes` via `time_adicionar_resultados`, mantendo as estatísticas sempre atualizadas. Fornece consulta por prefixo com filtragem por mandante, visitante ou ambos.

| Função | Descrição |
|---|---|
| `bdp_criar` | Aloca o gerenciador |
| `bdp_carregar` | Lê o CSV de partidas, cria partidas e atualiza os times |
| `bdp_consultar_partidas` | Filtra e imprime partidas pelo termo e modo (1, 2 ou 3) |
| `bdp_free` | Libera toda a memória da coleção |

---

### `utils` (`utils.h` / `utils.c`)

Módulo auxiliar com funções de uso geral.

| Função | Descrição |
|---|---|
| `is_prefixo` | Verifica se uma string começa com o termo (case-insensitive) |
| `tamanho_texto` | Retorna o número de caracteres visíveis (UTF-8) |

---

## Principais decisões de implementação

- **Vetor estático interno nos BDs:** `BDTimes` e `BDPartidas` utilizam vetores estáticos de tamanho pré-definido (10 times, 100 partidas), garantindo simplicidade e previsibilidade de consumo de memória.

- **Atualização de estatísticas na carga:** as estatísticas dos times são calculadas durante o carregamento das partidas em `bdp_carregar`, evitando reprocessamento a cada consulta. Pontos ganhos e saldo de gols são derivados sob demanda.

- **Busca por prefixo centralizada:** a função `is_prefixo` em `utils.c` centraliza a lógica de comparação, reutilizada tanto na consulta de times quanto na de partidas.

- **Alinhamento dinâmico:** as tabelas calculam a largura necessária baseada no maior nome de time presente no banco, garantindo que a visualização seja limpa independentemente do tamanho dos nomes.

- **Separação de responsabilidades:** cada módulo tem responsabilidade única — `Time` e `Partida` modelam entidades individuais; `BDTimes` e `BDPartidas` gerenciam as coleções; `utils` concentra funções genéricas; `main.c` cuida exclusivamente do fluxo de interação com o usuário.

- **Liberação total de memória:** ao encerrar com `Q`, o sistema chama `bdp_free` e `bdt_free` em ordem, garantindo que não haja vazamento de memória.