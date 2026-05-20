#ifndef BD_TIMES_H
#define BD_TIMES_H

#include "time.h"

/* Gerenciador da coleção de times */
typedef struct bd_times BDTimes;

/* Cria o gerenciador */
BDTimes *bdt_criar(void);

/* Libera o gerenciador e todos os times */
void bdt_free(BDTimes *bdt);

/* Carrega times de um arquivo CSV */
int bdt_carregar(BDTimes *bdt, const char *arquivo);

/* Busca um time pelo ID */
Time *bdt_buscar_por_id(BDTimes *bdt, int id);

/* Busca um time pelo nome ou prefixo */
Time *bdt_buscar_por_nome(BDTimes *bdt, const char *nome);

/* Imprime a tabela de classificação */
void bdt_imprimir_tabela(BDTimes *bdt);

/* Consulta e imprime times pelo prefixo */
void bdt_consultar_times(BDTimes *bdt, const char *termo);

/* Retorna a largura do maior nome para alinhamento */
int bdt_get_largura_nome_max(BDTimes *bdt);

#endif
