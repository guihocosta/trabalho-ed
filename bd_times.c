#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_times.h"
#include "bd_partidas.h"
#include "utils.h"

/* Nó da lista encadeada de times */
typedef struct bdt_node BDTNode;
struct bdt_node {
    Time *time;
    BDTNode *next;
};

/* Estrutura que gerencia a coleção de times usando uma lista encadeada */
struct bd_times {
    BDTNode *first; /* Aponta para o primeiro time (início da lista) */
    BDTNode *last;  /* Aponta para o último time (fim da lista) para append O(1) */
    int qtd;        /* Quantidade total de times cadastrados */
};

/* Aloca o banco de dados de times */
BDTimes *bdt_criar(void) {
    BDTimes *bdt = (BDTimes*)malloc(sizeof(BDTimes));
    if (bdt == NULL) {
        return NULL;
    }
    bdt->first = NULL;
    bdt->last = NULL;
    bdt->qtd = 0;
    return bdt;
}

/* Libera todos os times e o banco */
void bdt_free(BDTimes *bdt) {
    BDTNode *p = bdt->first;
    while (p != NULL) {
        BDTNode *t = p->next;
        time_free(p->time);
        free(p);
        p = t;
    }
    free(bdt);
}

/* Carrega os times do CSV linha por linha */
int bdt_carregar(BDTimes *bdt, const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (f == NULL) {
        return 0;
    }
    
    char linha[256];
    /* Pula o cabeçalho */
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }
    
    while (fgets(linha, sizeof(linha), f)) {        
        int id;
        char nome[100];
        
        if (sscanf(linha, "%d,%99[^\n\r]", &id, nome) == 2) {
            Time *t = time_criar(id, nome);
            if (t != NULL) {
                BDTNode *n = (BDTNode*)malloc(sizeof(BDTNode));
                if (n != NULL) {
                    n->time = t;
                    n->next = NULL;
                    
                    /* Insere no fim da lista utilizando o ponteiro 'last' */
                    if (bdt->first == NULL) {
                        bdt->first = n;
                        bdt->last = n;
                    } else {
                        bdt->last->next = n;
                        bdt->last = n;
                    }
                    bdt->qtd++;
                }
            }
        }
    }
    
    fclose(f);
    return 1;
}

/* Busca por ID */
Time *bdt_buscar_por_id(BDTimes *bdt, int id) {
    BDTNode *p = bdt->first;
    while (p != NULL) {
        if (time_get_id(p->time) == id) {
            return p->time;
        }
        p = p->next;
    }
    return NULL;
}

/* Busca por prefixo */
Time *bdt_buscar_por_nome(BDTimes *bdt, const char *nome) {
    BDTNode *p = bdt->first;
    while (p != NULL) {
        char *nome_time = time_get_nome(p->time);
        if (nome_time != NULL && is_prefixo(nome_time, nome)) {
            return p->time;
        }
        p = p->next;
    }
    return NULL;
}

/* Função interna para imprimir o cabeçalho das tabelas */
static void bdt_imprimir_cabecalho(int w) {
    printf("\n%-4s %-*s %3s %3s %3s %4s %4s %4s %4s\n",
           "ID", w, "Time", "V", "E", "D", "GM", "GS", "S", "PG");
}

/* Calcula a largura necessária para alinhar o maior nome */
int bdt_get_largura_nome_max(BDTimes *bdt) {
    int max = 15;
    BDTNode *p = bdt->first;
    while (p != NULL) {
        int len = tamanho_texto(time_get_nome(p->time));
        if (len > max) {
            max = len;
        }
        p = p->next;
    }
    return max;
}

/* Comparador para qsort: PG -> V -> S -> ID (critérios esportivos) */
static int comparar_times(const void *a, const void *b) {
    Time *t1 = *(Time**)a;
    Time *t2 = *(Time**)b;
    
    int pg1 = time_calcular_pontos(t1);
    int pg2 = time_calcular_pontos(t2);
    if (pg1 != pg2) {
        return pg2 - pg1;
    }
    
    int v1 = time_get_vitorias(t1);
    int v2 = time_get_vitorias(t2);
    if (v1 != v2) {
        return v2 - v1;
    }
    
    int s1 = time_calcular_saldo_gols(t1);
    int s2 = time_calcular_saldo_gols(t2);
    if (s1 != s2) {
        return s2 - s1;
    }
    
    return time_get_id(t1) - time_get_id(t2);
}

/* Lista todos os times de forma ordenada decrescentemente por mérito esportivo */
void bdt_imprimir_tabela(BDTimes *bdt) {
    if (bdt->qtd == 0) return;
    
    int w = bdt_get_largura_nome_max(bdt);
    
    /* Cria vetor temporário para ordenação */
    Time **arr = (Time**)malloc(bdt->qtd * sizeof(Time*));
    if (arr == NULL) {
        return;
    }
    
    int i = 0;
    BDTNode *p = bdt->first;
    while (p != NULL) {
        arr[i++] = p->time;
        p = p->next;
    }
    
    qsort(arr, bdt->qtd, sizeof(Time*), comparar_times);
    
    bdt_imprimir_cabecalho(w);
    for (i = 0; i < bdt->qtd; i++) {
        time_imprimir(arr[i], w);
    }
    
    free(arr);
}

/* Busca e imprime times filtrados */
void bdt_consultar_times(BDTimes *bdt, const char *termo) {
    int w = bdt_get_largura_nome_max(bdt);
    int encontrou = 0;
    BDTNode *p = bdt->first;

    while (p != NULL) {
        char *nome = time_get_nome(p->time);
        if (is_prefixo(nome, termo)) {
            if (!encontrou) {
                bdt_imprimir_cabecalho(w);
            }
            time_imprimir(p->time, w);
            encontrou = 1;
        }
        p = p->next;
    }

    if (!encontrou) {
        printf("Erro: Nenhum time encontrado com o prefixo \"%s\".\n", termo);
    }
}

/* Salva a tabela de classificação ordenada em um arquivo CSV */
void bdt_salvar_classificacao(BDTimes *bdt, const char *arquivo) {
    if (bdt->qtd == 0) return;
    
    FILE *f = fopen(arquivo, "w");
    if (f == NULL) {
        return;
    }
    
    Time **arr = (Time**)malloc(bdt->qtd * sizeof(Time*));
    if (arr == NULL) {
        fclose(f);
        return;
    }
    
    int idx = 0;
    BDTNode *p = bdt->first;
    while (p != NULL) {
        arr[idx++] = p->time;
        p = p->next;
    }
    
    qsort(arr, bdt->qtd, sizeof(Time*), comparar_times);
    
    fprintf(f, "ID,Time,V,E,D,GM,GS,S,PG\n");
    int i;
    for (i = 0; i < bdt->qtd; i++) {
        Time *t = arr[i];
        fprintf(f, "%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
            time_get_id(t),
            time_get_nome(t),
            time_get_vitorias(t),
            time_get_empates(t),
            time_get_derrotas(t),
            time_get_gols_marcados(t),
            time_get_gols_sofridos(t),
            time_calcular_saldo_gols(t),
            time_calcular_pontos(t)
        );
    }
    
    free(arr);
    fclose(f);
}

/* Recalcula as estatísticas de todos os times com base nas partidas */
void bdt_recalcular_estatisticas(BDTimes *bdt, BDPartidas *bdp) {
    /* 1. Limpa estatísticas de todos os times */
    BDTNode *p_time = bdt->first;
    while (p_time != NULL) {
        time_limpar_estatisticas(p_time->time);
        p_time = p_time->next;
    }
    
    /* 2. Aplica resultados de todas as partidas */
    BDPNode *p_part = bdp_get_first_node(bdp);
    while (p_part != NULL) {
        Partida *part = p_part->partida;
        int t1_id = partida_get_time1(part);
        int t2_id = partida_get_time2(part);
        int g1 = partida_get_gols1(part);
        int g2 = partida_get_gols2(part);
        
        Time *time1 = bdt_buscar_por_id(bdt, t1_id);
        Time *time2 = bdt_buscar_por_id(bdt, t2_id);
        if (time1 != NULL) {
            time_adicionar_resultados(time1, g1, g2);
        }
        if (time2 != NULL) {
            time_adicionar_resultados(time2, g2, g1);
        }
        p_part = p_part->next;
    }
}

/* Lista todos os times cadastrados com seus IDs */
void bdt_listar_ids(BDTimes *bdt) {
    printf("Times cadastrados:\n");
    int i, count = 0;
    for (i = 0; i < bdt->qtd; i++) {
        Time *t = bdt_buscar_por_id(bdt, i);
        if (t != NULL) {
            printf("%d: %-12s", time_get_id(t), time_get_nome(t));
            count++;
            if (count % 3 == 0 || i == bdt->qtd - 1) {
                printf("\n");
            } else {
                printf("  |  ");
            }
        }
    }
    printf("\n");
}
