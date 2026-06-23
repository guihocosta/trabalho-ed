#ifndef BD_PARTIDAS_H
#define BD_PARTIDAS_H

#include "partida.h"
#include "bd_times.h"

/* Gerenciador da coleção de partidas */
typedef struct bd_partidas BDPartidas;

/* Nó da lista encadeada de partidas */
typedef struct bdp_node BDPNode;
struct bdp_node {
    Partida *partida;
    BDPNode *next;
};

/* Cria o gerenciador de partidas */
BDPartidas *bdp_criar(void);

/* Libera o gerenciador e as partidas */
void bdp_free(BDPartidas *bdp);

/* Carrega partidas */
int bdp_carregar(BDPartidas *bdp, const char *arquivo);

/* Consulta e filtra partidas */
void bdp_consultar_partidas(BDPartidas *bdp, BDTimes *bdt, int modo, const char *busca);

/* Busca uma partida pelo ID */
Partida *bdp_buscar_por_id(BDPartidas *bdp, int id);

/* Insere uma nova partida no banco (início da lista) */
int bdp_inserir(BDPartidas *bdp, Partida *p);

/* Remove uma partida pelo ID */
int bdp_remover_por_id(BDPartidas *bdp, int id);

/* Salva as partidas em um arquivo CSV */
void bdp_salvar(BDPartidas *bdp, const char *arquivo);

/* Gera um novo ID único por autoincremento */
int bdp_gerar_novo_id(BDPartidas *bdp);

/* Retorna a quantidade de partidas */
int bdp_get_qtd(BDPartidas *bdp);

/* Retorna o primeiro nó de partidas para travessia */
BDPNode *bdp_get_first_node(BDPartidas *bdp);

#endif
