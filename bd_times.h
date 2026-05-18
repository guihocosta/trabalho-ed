#ifndef BD_TIMES_H
#define BD_TIMES_H

#include "time.h"

typedef struct bd_times BDTimes;

BDTimes *bdt_criar(void);
void bdt_free(BDTimes *bdt);
int bdt_carregar(BDTimes *bdt, const char *arquivo);

Time *bdt_buscar_por_id(BDTimes *bdt, int id);
Time *bdt_buscar_por_nome(BDTimes *bdt, const char *nome);

void bdt_imprimir_tabela(BDTimes *bdt);

int bdt_get_qtd(BDTimes *bdt);
Time *bdt_get_time_idx(BDTimes *bdt, int idx);

#endif