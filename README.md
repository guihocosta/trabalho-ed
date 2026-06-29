# Campeonato Computacional de Futebol

Sistema de gerenciamento de partidas e classificação de um campeonato de futebol, desenvolvido em linguagem C como trabalho prático da disciplina de Estruturas de Dados do IFES Campus Serra.

---

> **Disciplina:** Estrutura de Dados  
> **Período:** 3º Período – Bacharelado em Sistemas de Informação  
> **Instituição:** IFES – Campus Serra  
> **Professor:** Thiago M. Paixão  
> **Alunos:** Luan Otoni e Guilherme Omena

---

## 📂 PARTE II — Listas Encadeadas, Persistência Dinâmica e Autoincremento (Entrega Atual)

Nesta etapa (Parte II), migramos o armazenamento interno do banco de dados de vetores estáticos para **listas simplesmente encadeadas** dinâmicas (`BDTNode` e `BDPNode`), e habilitamos todas as operações de escrita no menu principal com recalculo em tempo real de estatísticas esportivas.

### Estrutura do repositório (Parte II)

```
.
├── main.c                        # Ponto de entrada do programa e fluxo do menu
├── time.c / time.h               # TAD Time (dados e estatísticas da equipe)
├── bd_times.c / bd_times.h       # TAD BDTimes (coleção de times via lista encadeada)
├── partida.c / partida.h         # TAD Partida (dados de cada confronto)
├── bd_partidas.c / bd_partidas.h # TAD BDPartidas (coleção de partidas via lista encadeada)
├── utils.c / utils.h             # Funções utilitárias (prefixos, caracteres UTF-8)
├── Makefile                      # Regras de compilação e teste do projeto
├── times.csv                     # Banco de dados inicial de times
├── partidas_vazio.csv            # Cenário 1: campeonato sem partidas
├── partidas_parcial.csv          # Cenário 2: campeonato em andamento
└── partidas_completo.csv         # Cenário 3: campeonato finalizado
```

### Como compilar e executar

#### Compilar

```bash
make compile
```

#### Executar

O sistema aceita o arquivo de partidas como argumento opcional. Se nenhum for passado, usa `partidas_completo.csv` por padrão. Ao sair com `Q`, as partidas em memória são sempre salvas em `bd_partidas.csv` e a classificação ordenada em `bd_classificacao.csv`, independente do arquivo carregado.

```bash
# Executando com o cenário padrão (partidas_completo.csv)
./campeonato

# Cenário 1 — Campeonato vazio
./campeonato partidas_vazio.csv

# Cenário 2 — Campeonato em andamento
./campeonato partidas_parcial.csv

# Cenário 3 — Campeonato finalizado
./campeonato partidas_completo.csv
```

#### Limpar arquivos gerados

```bash
make clean
```

### Funcionalidades do Sistema (Parte II)

Ao iniciar, o sistema oferece as **6 opções + encerramento** ativas:

```
===== Sistema de Gerenciamento de Partidas =====
  1 - Consultar time
  2 - Consultar partidas
  3 - Atualizar partida
  4 - Remover partida
  5 - Inserir partida
  6 - Imprimir tabela de classificação
  Q - Sair
================================================
```

#### Opção 1 — Consultar time
Busca equipes por nome ou prefixo e exibe o desempenho completo (`ID, Time, V, E, D, GM, GS, S, PG`).

#### Opção 2 — Consultar partidas
Pesquisa confrontos por time mandante, visitante ou ambos (por prefixo). Exibe o placar em colunas alinhadas:
```
ID   Time1           Time2           Placar1 Placar2
85   RUSTicos        SeQueLas              3       4
```

#### Opção 3 — Atualizar partida
Permite alterar o placar de uma partida existente pesquisando por ID. O caractere `-` pode ser inserido para manter o placar de um dos times inalterado. Após confirmação (`S/N`), as estatísticas dos times são recalculadas.

#### Opção 4 — Remover partida
Exclui uma partida da lista encadeada por ID. Após confirmação, remove o nó correspondente e recalcula as estatísticas do campeonato.

#### Opção 5 — Inserir partida
Adiciona um novo jogo solicitando as informações linha a linha (ID do time mandante, ID do time visitante, gols do mandante e gols do visitante). O sistema valida a existência dos times, impede confrontos de uma equipe consigo mesma, autoincrementa o ID da partida e recalcula a tabela.

#### Opção 6 — Imprimir tabela de classificação
Imprime todas as equipes ordenadas decrescentemente por mérito esportivo, usando os 7 critérios oficiais de desempate em ordem de prioridade: **PG → V → S → GM → GS → E → D**.

#### Q — Sair
Gera os arquivos de saída finais e limpa toda a memória dinâmica:
- Grava as partidas salvas em `bd_partidas.csv`.
- Grava a classificação final ordenada em `bd_classificacao.csv`.

### Principais TADs (Parte II)

#### `Time` & `BDTimes` (`bd_times.h` / `bd_times.c`)
- **`Time`**: Representa uma equipe individualmente, suas estatísticas e dados de pontuação derivados.
- **`BDTimes`**: Gerencia a coleção de times usando uma lista simplesmente encadeada (`BDTNode`). Implementa ordenação esportiva instanciando um vetor dinâmico de ponteiros no momento da exibição/salvamento e aplicando o algoritmo de ordenação rápida `qsort`.
- **`bdt_recalcular_estatisticas`**: Centraliza o processamento de pontuação. Ela limpa o status de todas as equipes e reaplica a totalização iterando sobre a lista encadeada de partidas ativas.

#### `Partida` & `BDPartidas` (`bd_partidas.h` / `bd_partidas.c`)
- **`Partida`**: Representa um confronto específico e seu placar.
- **`BDPartidas`**: Gerencia a lista de partidas na memória utilizando uma lista simplesmente encadeada (`BDPNode`).
- **`bdp_gerar_novo_id`**: Realiza o autoincremento localizando o maior ID corrente uma única vez e, após isso, controlando a numeração sequencial um a um com o auxílio de variáveis `static`.

### Principais decisões de projeto (Parte II)
- **Lista Encadeada Direta com Ponteiro `last` (Inserção no Fim)**: Implementamos as listas encadeadas diretamente nos módulos de banco de dados (`BDTNode` e `BDPNode`) com suporte a ponteiros para o primeiro (`first`) e último (`last`) nó.

---

## 📂 PARTE I — Vetores Estáticos e Consultas Iniciais (Histórico)

Esta foi a entrega inicial do campeonato (Parte I), focada exclusivamente na leitura de dados em vetores estáticos e na realização de buscas e consultas básicas.

### Estrutura do repositório (Parte I)

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

### Como compilar e executar (Parte I)

#### Compilar

```bash
make compile
```

#### Executar

```bash
# Usando o padrão (partidas_completo.csv)
./campeonato

# Cenário 1 — Campeonato vazio
./campeonato partidas_vazio.csv

# Cenário 2 — Campeonato em andamento
./campeonato partidas_parcial.csv

# Cenário 3 — Campeonato finalizado
./campeonato partidas_completo.csv
```

#### Compilar e executar de uma vez (cenário padrão)

```bash
make run
```

#### Limpar arquivos gerados

```bash
make clean
```

### Como usar o sistema (Parte I)

Ao iniciar, o sistema exibe o menu principal com opções parciais habilitadas:

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

#### Opção 1 — Consultar time

Solicita um nome ou prefixo e exibe as estatísticas de todos os times correspondentes.

```
Digite o nome ou prefixo do time: Se

ID   Time              V   E   D   GM   GS    S   PG
2    SemCTRL          11   2   5   43   34    9   35
5    SeQueLas          7   3   8   50   49    1   24
```

#### Opção 2 — Consultar partidas

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

#### Opção 6 — Imprimir tabela de classificação

Exibe todos os times ordenados por ID com suas estatísticas acumuladas.

```
Imprimindo classificacao...
ID   Time            V   E   D   GM   GS    S   PG
0    JAVAlis         13  3   2   58   30   28   42
1    ESCorpiões      10  2   6   55   39   16   32
2    SemCTRL         11  2   5   43   34    9   35
...
```

#### Q — Sair

Encerra o sistema e libera toda a memória alocada.

### Principais TADs (Parte I)

#### `Time` (`time.h` / `time.c`)

Representa uma equipe de futebol. Armazena identificador, nome e estatísticas de desempenho (vitórias, empates, derrotas, gols marcados e sofridos). Pontos ganhos e saldo de gols são calculados sob demanda pelas funções `time_calcular_pontos` e `time_calcular_saldo_gols`, garantindo consistência dos dados. As estatísticas são atualizadas via `time_adicionar_resultados` a cada partida processada.

| Função | Descrição |
|---|---|
| `time_criar` | Aloca e inicializa um time com estatísticas zeradas |
| `time_adicionar_resultados` | Atualiza vitórias, empates, derrotas e gols após uma partida |
| `time_calcular_pontos` | Retorna PG = 3×V + E |
| `time_calcular_saldo_gols` | Retorna S = GM − GS |
| `time_imprimir` | Imprime a linha formatada do time na tabela |
| `time_free` | Libera a memória do time |

#### `BDTimes` (`bd_times.h` / `bd_times.c`)

Gerencia a coleção de todos os times. Carrega os dados do arquivo `times.csv` e cria uma instância de `Time` para cada registro. Fornece busca por ID e por nome (prefixo), além de listagem completa da tabela.

| Função | Descrição |
|---|---|
| `bdt_criar` | Aloca o gerenciador |
| `bdt_carregar` | Lê `times.csv` e popula a coleção |
| `bdt_buscar_por_id` | Retorna o time com o ID informado |
| `bdt_buscar_por_nome` | Retorna o primeiro time cujo nome bate com o prefixo |
| `bdt_consultar_times` | Busca e imprime todos os times que batem com o prefixo |
| `bdt_imprimir_tabela` | Imprime todos os times formatados |
| `bdt_get_largura_nome_max` | Retorna a maior largura de nome para alinhamento |
| `bdt_get_qtd` | Retorna a quantidade de times carregados |
| `bdt_get_time_idx` | Retorna o time pelo índice no vetor interno |
| `bdt_free` | Libera toda a memória da coleção |

#### `Partida` (`partida.h` / `partida.c`)

Representa um jogo entre dois times, encapsulando ID da partida, IDs dos times e placar. É a estrutura elementar manipulada pelo `BDPartidas`.

| Função | Descrição |
|---|---|
| `partida_criar` | Aloca e inicializa uma partida |
| `partida_imprimir` | Imprime o placar formatado usando os nomes do `BDTimes` |
| `partida_free` | Libera a memória da partida |

#### `BDPartidas` (`bd_partidas.h` / `bd_partidas.c`)

Gerencia a coleção de todas as partidas. Ao carregar as partidas, além de armazenar cada partida, já repassa os resultados ao `BDTimes` via `time_adicionar_resultados`, mantendo as estatísticas sempre atualizadas. Fornece consulta por prefixo com filtragem por mandante, visitante ou ambos.

| Função | Descrição |
|---|---|
| `bdp_criar` | Aloca o gerenciador |
| `bdp_carregar` | Lê o CSV de partidas, cria partidas e updates os times |
| `bdp_consultar_partidas` | Filtra e imprime partidas pelo termo e modo (1, 2 ou 3) |
| `bdp_free` | Libera toda a memória da coleção |

#### `utils` (`utils.h` / `utils.c`)

Módulo auxiliar com funções de uso geral.

| Função | Descrição |
|---|---|
| `is_prefixo` | Verifica se uma string começa com o termo (case-insensitive) |
| `tamanho_texto` | Retorna o número de caracteres visíveis (UTF-8) |

### Principais decisões de implementação (Parte I)

- **Vetor estático interno nos BDs:** `BDTimes` e `BDPartidas` utilizam vetores estáticos de tamanho pré-definido (10 times, 100 partidas), garantindo simplicidade e previsibilidade de consumo de memória.
- **Atualização de estatísticas na carga:** as estatísticas dos times são calculadas durante o carregamento das partidas em `bdp_carregar`, evitando reprocessamento a cada consulta. Pontos ganhos e saldo de gols são derivados sob demanda.
- **Busca por prefixo centralizada:** a função `is_prefixo` em `utils.c` centraliza a lógica de comparação, reutilizada tanto na consulta de times quanto na de partidas.
- **Alinhamento dinâmico:** as tabelas calculam a largura necessária baseada no maior nome de time presente no banco, garantindo que a visualização seja limpa independentemente do tamanho dos nomes.
- **Separação de responsabilidades:** cada módulo tem responsabilidade única — `Time` e `Partida` modelam entidades individuais; `BDTimes` e `BDPartidas` gerenciam as coleções; `utils` concentra funções genéricas; `main.c` cuida exclusivamente do fluxo de interação com o usuário.