#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_partidas.h"
#include "utils.h"

/* Estrutura que guarda as partidas usando uma lista encadeada */
struct bd_partidas {
    BDPNode *first; /* Aponta para o primeiro elemento da lista (início) */
    BDPNode *last;  /* Aponta para o último elemento (fim) para permitir inserção O(1) */
    int qtd;        /* Quantidade de partidas registradas */
};

BDPartidas *bdp_criar(void) {
    BDPartidas *bdp = (BDPartidas*)malloc(sizeof(BDPartidas));
    if (bdp == NULL) {
        return NULL;
    }
    bdp->first = NULL;
    bdp->last = NULL;
    bdp->qtd = 0;
    return bdp;
}

void bdp_free(BDPartidas *bdp) {
    BDPNode *p = bdp->first;
    while (p != NULL) {
        BDPNode *t = p->next;
        partida_free(p->partida);
        free(p);
        p = t;
    }
    free(bdp);
}

/* Carrega partidas */
int bdp_carregar(BDPartidas *bdp, const char *arquivo) {
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
        int id, t1, t2, g1, g2;
        if (sscanf(linha, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) == 5) {
            Partida *p = partida_criar(id, t1, t2, g1, g2);
            if (p != NULL) {
                if (!bdp_inserir(bdp, p)) {
                    partida_free(p);
                }
            }
        }
    }
    
    fclose(f);
    return 1;
}

/* Filtra partidas por mandante, visitante ou ambos */
void bdp_consultar_partidas(BDPartidas *bdp, BDTimes *bdt, int modo, const char *busca) {
    int encontrou = 0;
    int w = bdt_get_largura_nome_max(bdt);
    BDPNode *curr = bdp->first;

    while (curr != NULL) {
        Partida *p = curr->partida;

        Time *t1 = bdt_buscar_por_id(bdt, partida_get_time1(p));
        Time *t2 = bdt_buscar_por_id(bdt, partida_get_time2(p));

        char *nome1 = time_get_nome(t1);
        char *nome2 = time_get_nome(t2);

        int bate = 0;
        if (strlen(busca) == 0) {
            bate = 1;
        } else if (modo == 1) { /* Só mandante */
            bate = is_prefixo(nome1, busca);
        } else if (modo == 2) { /* Só visitante */
            bate = is_prefixo(nome2, busca);
        } else if (modo == 3) { /* Mandante ou Visitante */
            bate = is_prefixo(nome1, busca) || is_prefixo(nome2, busca);
        }

        if (bate) {
            if (!encontrou) {
                printf("\n%-4s %-*s %-*s %7s %7s\n", "ID", w, "Time1", w, "Time2", "Placar1", "Placar2");
            }
            partida_imprimir(p, bdt, w);
            encontrou = 1;
        }
        curr = curr->next;
    }

    if (!encontrou) {
        printf("Erro: Nenhuma partida encontrada.\n");
    }
}

/* Busca uma partida pelo ID */
Partida *bdp_buscar_por_id(BDPartidas *bdp, int id) {
    BDPNode *curr = bdp->first;
    while (curr != NULL) {
        if (partida_get_id(curr->partida) == id) {
            return curr->partida;
        }
        curr = curr->next;
    }
    return NULL;
}

/* Insere uma nova partida no banco (fim da lista) em O(1) usando o ponteiro 'last' */
int bdp_inserir(BDPartidas *bdp, Partida *p) {
    BDPNode *n = (BDPNode*)malloc(sizeof(BDPNode));
    if (n == NULL) {
        return 0;
    }
    n->partida = p;
    n->next = NULL;
    
    /* Se a lista estiver vazia, o novo nó é tanto o primeiro quanto o último */
    if (bdp->first == NULL) {
        bdp->first = n;
        bdp->last = n;
    } else {
        /* Caso contrário, encadeia no final da lista e atualiza o ponteiro de último nó */
        bdp->last->next = n;
        bdp->last = n;
    }
    bdp->qtd++;
    return 1;
}

/* Remove uma partida pelo ID e atualiza o ponteiro do último nó se necessário */
int bdp_remover_por_id(BDPartidas *bdp, int id) {
    BDPNode *curr = bdp->first;
    BDPNode *prev = NULL;
    while (curr != NULL) {
        if (partida_get_id(curr->partida) == id) {
            if (prev == NULL) {
                bdp->first = curr->next;
            } else {
                prev->next = curr->next;
            }
            
            /* Se o nó que está sendo removido é o último, atualiza o ponteiro 'last' para o nó anterior */
            if (curr == bdp->last) {
                bdp->last = prev;
            }
            
            partida_free(curr->partida);
            free(curr);
            bdp->qtd--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

/* Salva as partidas em um arquivo CSV na ordem em que estão na lista */
void bdp_salvar(BDPartidas *bdp, const char *arquivo) {
    FILE *f = fopen(arquivo, "w");
    if (f == NULL) {
        return;
    }
    fprintf(f, "ID,Time1,Time2,GolsTime1,GolsTime2\n");
    
    BDPNode *p = bdp->first;
    while (p != NULL) {
        Partida *part = p->partida;
        fprintf(f, "%d,%d,%d,%d,%d\n",
            partida_get_id(part),
            partida_get_time1(part),
            partida_get_time2(part),
            partida_get_gols1(part),
            partida_get_gols2(part)
        );
        p = p->next;
    }
    fclose(f);
}

/* Gera um novo ID único por autoincremento */
int bdp_gerar_novo_id(BDPartidas *bdp) {
    static int proximo_id = 0;
    static int inicializado = 0;
    if (!inicializado) {
        int max_id = -1;
        BDPNode *curr = bdp->first;
        while (curr != NULL) {
            if (partida_get_id(curr->partida) > max_id) {
                max_id = partida_get_id(curr->partida);
            }
            curr = curr->next;
        }
        proximo_id = max_id + 1;
        inicializado = 1;
    }
    return proximo_id++;
}

/* Retorna a quantidade de partidas */
int bdp_get_qtd(BDPartidas *bdp) {
    return bdp->qtd;
}

/* Retorna o primeiro nó de partidas para travessia */
BDPNode *bdp_get_first_node(BDPartidas *bdp) {
    return bdp->first;
}
