#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include "utils.h"

/* Estrutura que guarda as informações e estatísticas de um time */
struct time {
    int id;
    char *nome;
    int vitorias;
    int empates;
    int derrotas;
    int gols_marcados;
    int gols_sofridos;
};

/* Aloca memória para o time e inicializa seus campos */
Time *time_criar(int id, const char *nome) {
    Time *t = (Time*)malloc(sizeof(Time));
    if (t == NULL) {
        return NULL;
    }
    
    t->id = id;
    /* Aloca memória para a string do nome e copia o valor */
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

/* Calcula os pontos: 3 por vitória e 1 por empate */
int time_calcular_pontos(Time *t) {
    if (t == NULL) {
        return 0;
    }
    return (t->vitorias * 3) + t->empates;
}

/* Calcula o saldo de gols: marcados menos sofridos */
int time_calcular_saldo_gols(Time *t) {
    if (t == NULL) {
        return 0;
    }
    return t->gols_marcados - t->gols_sofridos;
}

/* Libera a memória alocada para o nome e para a struct do time */
void time_free(Time *t) {
    if (t == NULL) {
        return;
    }
    if (t->nome != NULL) {
        free(t->nome);
    }
    free(t);
}

/* Funções para obter dados específicos do time (usadas por outros módulos) */
int time_get_id(Time *t) {
    if (t == NULL) {
        return -1;
    }
    return t->id;
}

char *time_get_nome(Time *t) {
    if (t == NULL) {
        return NULL;
    }
    return t->nome;
}

int time_get_vitorias(Time *t) {
    if (t == NULL) {
        return 0;
    }
    return t->vitorias;
}

int time_get_empates(Time *t) {
    if (t == NULL) {
        return 0;
    }
    return t->empates;
}

int time_get_derrotas(Time *t) {
    if (t == NULL) {
        return 0;
    }
    return t->derrotas;
}

int time_get_gols_marcados(Time *t) {
    if (t == NULL) {
        return 0;
    }
    return t->gols_marcados;
}

int time_get_gols_sofridos(Time *t) {
    if (t == NULL) {
        return 0;
    }
    return t->gols_sofridos;
}

/* Atualiza estatísticas com base no placar de uma partida */
void time_adicionar_resultados(Time *t, int gols_pro, int gols_contra) {
    if (t == NULL) {
        return;
    }
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

/* Limpa todas as estatísticas acumuladas do time */
void time_limpar_estatisticas(Time *t) {
    if (t == NULL) {
        return;
    }
    t->vitorias = 0;
    t->empates = 0;
    t->derrotas = 0;
    t->gols_marcados = 0;
    t->gols_sofridos = 0;
}

/* Imprime as estatísticas do time em uma linha da tabela */
void time_imprimir(Time *t, int largura_nome) {
    if (t == NULL) {
        return;
    }
    printf("%-4d %s", t->id, t->nome);
    
    /* Preenche com espaços para manter as colunas alinhadas */
    int i, tam = tamanho_texto(t->nome);
    for (i = tam; i < largura_nome; i++) {
        printf(" ");
    }

    printf(" %3d %3d %3d %4d %4d %4d %4d\n",
        t->vitorias,
        t->empates,
        t->derrotas,
        t->gols_marcados,
        t->gols_sofridos,
        time_calcular_saldo_gols(t),
        time_calcular_pontos(t)
    );
}
