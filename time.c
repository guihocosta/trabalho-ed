#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"

// Estrutura que representa um time e suas estatísticas
struct time {
    int id;
    char *nome;
    int vitorias;
    int empates;
    int derrotas;
    int gols_marcados;
    int gols_sofridos;
};

// Cria um novo time
Time *time_criar(int id, const char *nome) {
    Time *t = (Time*)malloc(sizeof(Time));
    if (t == NULL) return NULL;
    
    t->id = id;
    // Aloca memória para o nome e copia
    t->nome = (char*)malloc((strlen(nome) + 1) * sizeof(char));
    if (t->nome != NULL) {
        strcpy(t->nome, nome);
    }
    
    t->vitorias = 0;
    t->empates = 0;
    t->derrotas = 0;
    t->gols_marcados = 0;
    t->gols_sofridos = 0;
    
    return t;
}

int time_calcular_pontos(Time *t) {
    if (t == NULL) return 0;
    return (t->vitorias * 3) + t->empates;
}

int time_calcular_saldo_gols(Time *t) {
    if (t == NULL) return 0;
    return t->gols_marcados - t->gols_sofridos;
}

void time_free(Time *t) {
    if (t == NULL) return;
    if (t->nome != NULL) free(t->nome);
    free(t);
}

int time_get_id(Time *t) { return t ? t->id : -1; }
char *time_get_nome(Time *t) { return t ? t->nome : NULL; }
int time_get_vitorias(Time *t) { return t ? t->vitorias : 0; }
int time_get_empates(Time *t) { return t ? t->empates : 0; }
int time_get_derrotas(Time *t) { return t ? t->derrotas : 0; }
int time_get_gols_marcados(Time *t) { return t ? t->gols_marcados : 0; }
int time_get_gols_sofridos(Time *t) { return t ? t->gols_sofridos : 0; }

void time_adicionar_resultados(Time *t, int gols_pro, int gols_contra) {
    if (t == NULL) return;
    
    t->gols_marcados += gols_pro;
    t->gols_sofridos += gols_contra;
    
    if (gols_pro > gols_contra) {
        t->vitorias++;
    } else if (gols_pro == gols_contra) {
        t->empates++;
    } else {
        t->derrotas++;
    }
}

void time_imprimir(Time *t) {
    if (t == NULL) return;

    printf("%-4d %-12s %3d %3d %3d %4d %4d %4d %4d\n",
        t->id,
        t->nome,
        t->vitorias,
        t->empates,
        t->derrotas,
        t->gols_marcados,
        t->gols_sofridos,
        time_calcular_saldo_gols(t),
        time_calcular_pontos(t)
    );
}