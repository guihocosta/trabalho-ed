#ifndef BD_PARTIDAS_H
#define BD_PARTIDAS_H

#include "partida.h"
#include "bd_times.h"

/* Gerenciador da coleção de partidas */
typedef struct bd_partidas BDPartidas;

/* Cria o gerenciador de partidas */
BDPartidas *bdp_criar(void);

/* Libera o gerenciador e as partidas */
void bdp_free(BDPartidas *bdp);

/* Carrega partidas e atualiza as estatísticas dos times */
int bdp_carregar(BDPartidas *bdp, const char *arquivo, BDTimes *bdt);

/* Consulta e filtra partidas */
void bdp_consultar_partidas(BDPartidas *bdp, BDTimes *bdt, int modo, const char *termo);

#endif
