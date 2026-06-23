#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partida.h"
#include "utils.h"

/* Estrutura que guarda os dados de um jogo */
struct partida {
    int id;
    int time1_id;
    int time2_id;
    int gols1;
    int gols2;
};

/* Aloca e inicializa uma partida */
Partida *partida_criar(int id, int time1_id, int time2_id, int gols1, int gols2) {
    Partida *p = (Partida*)malloc(sizeof(Partida));
    if (p == NULL) {
        return NULL;
    }
    
    p->id = id;
    p->time1_id = time1_id;
    p->time2_id = time2_id;
    p->gols1 = gols1;
    p->gols2 = gols2;
    
    return p;
}

/* Libera memória da struct partida */
void partida_free(Partida *p) {
    if (p != NULL) {
        free(p);
    }
}

/* Getters e Setters */
int partida_get_id(Partida *p) {
    return p->id;
}

int partida_get_time1(Partida *p) {
    if (p == NULL) {
        return -1;
    }
    return p->time1_id;
}

int partida_get_time2(Partida *p) {
    if (p == NULL) {
        return -1;
    }
    return p->time2_id;
}

int partida_get_gols1(Partida *p) {
    return p->gols1;
}

int partida_get_gols2(Partida *p) {
    return p->gols2;
}

void partida_set_placar(Partida *p, int g1, int g2) {
    p->gols1 = g1;
    p->gols2 = g2;
}

/* Imprime placar formatado buscando nomes no banco de times */
void partida_imprimir(Partida *p, BDTimes *bdt, int largura_nome) {
    if (p == NULL || bdt == NULL) {
        return;
    }

    Time *t1 = bdt_buscar_por_id(bdt, p->time1_id);
    Time *t2 = bdt_buscar_por_id(bdt, p->time2_id);

    char *nome1 = t1 ? time_get_nome(t1) : "Desconhecido";
    char *nome2 = t2 ? time_get_nome(t2) : "Desconhecido";

    printf("%-4d ", p->id);

    printf("%s", nome1);
    int i, tam1 = tamanho_texto(nome1);
    for (i = tam1; i < largura_nome; i++) {
        printf(" ");
    }
    printf(" ");

    printf("%s", nome2);
    int tam2 = tamanho_texto(nome2);
    for (i = tam2; i < largura_nome; i++) {
        printf(" ");
    }

    printf(" %7d %7d\n", p->gols1, p->gols2);
}
