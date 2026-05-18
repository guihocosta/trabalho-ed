#ifndef PARTIDA_H
#define PARTIDA_H

#include "bd_times.h"

typedef struct partida Partida;

Partida *partida_criar(int id, int time1_id, int time2_id, int gols1, int gols2);
void partida_free(Partida *p);

int partida_get_id(Partida *p);
int partida_get_time1(Partida *p);
int partida_get_time2(Partida *p);
int partida_get_gols1(Partida *p);
int partida_get_gols2(Partida *p);

// Imprime a partida formatada
void partida_imprimir(Partida *p, BDTimes *bdt);

#endif