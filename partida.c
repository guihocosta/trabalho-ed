#include <stdio.h>
#include <stdlib.h>
#include "partida.h"

// Estrutura que representa uma partida
struct partida {
    int id;
    int time1_id;
    int time2_id;
    int gols1;
    int gols2;
};

Partida *partida_criar(int id, int time1_id, int time2_id, int gols1, int gols2) {
    Partida *p = (Partida*)malloc(sizeof(Partida));
    if (p == NULL) return NULL;
    
    p->id = id;
    p->time1_id = time1_id;
    p->time2_id = time2_id;
    p->gols1 = gols1;
    p->gols2 = gols2;
    
    return p;
}

void partida_free(Partida *p) {
    if (p != NULL) free(p);
}

int partida_get_id(Partida *p) { return p ? p->id : -1; }
int partida_get_time1(Partida *p) { return p ? p->time1_id : -1; }
int partida_get_time2(Partida *p) { return p ? p->time2_id : -1; }
int partida_get_gols1(Partida *p) { return p ? p->gols1 : 0; }
int partida_get_gols2(Partida *p) { return p ? p->gols2 : 0; }

void partida_imprimir(Partida *p, BDTimes *bdt) {
    if (p == NULL || bdt == NULL) return;

    Time *t1 = bdt_buscar_por_id(bdt, p->time1_id);
    Time *t2 = bdt_buscar_por_id(bdt, p->time2_id);

    char *nome1 = time_get_nome(t1);
    char *nome2 = time_get_nome(t2);

    printf("%d: %s %d x %d %s\n", 
        p->id, 
        nome1 ? nome1 : "Desconhecido", 
        p->gols1, 
        p->gols2, 
        nome2 ? nome2 : "Desconhecido"
    );
}