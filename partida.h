#ifndef PARTIDA_H
#define PARTIDA_H

#include "bd_times.h"

/* Representação de uma Partida */
typedef struct partida Partida;

/* Cria uma nova partida */
Partida *partida_criar(int id, int time1_id, int time2_id, int gols1, int gols2);

/* Libera a memória da partida */
void partida_free(Partida *p);

/* Funções para obter dados da partida (usadas) */
int partida_get_id(Partida *p);
int partida_get_time1(Partida *p);
int partida_get_time2(Partida *p);
int partida_get_gols1(Partida *p);
int partida_get_gols2(Partida *p);
void partida_set_placar(Partida *p, int g1, int g2);

/* Imprime o placar da partida */
void partida_imprimir(Partida *p, BDTimes *bdt, int largura_nome);

#endif
