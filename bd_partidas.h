#ifndef BD_PARTIDAS_H
#define BD_PARTIDAS_H

#include "partida.h"
#include "bd_times.h"

typedef struct bd_partidas BDPartidas;

BDPartidas *bdp_criar(void);
void bdp_free(BDPartidas *bdp);
int bdp_carregar(BDPartidas *bdp, const char *arquivo, BDTimes *bdt);

void bdp_consultar_partidas(BDPartidas *bdp, BDTimes *bdt, const char *termo);

#endif