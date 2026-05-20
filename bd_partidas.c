#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_partidas.h"
#include "utils.h"

#define MAX_PARTIDAS 100

// Estrutura que guarda as partidas
struct bd_partidas {
    Partida *partidas[MAX_PARTIDAS];
    int qtd;
};

BDPartidas *bdp_criar(void) {
    BDPartidas *bdp = (BDPartidas*)malloc(sizeof(BDPartidas));
    if (bdp == NULL) return NULL;
    
    bdp->qtd = 0;
    int i;
    for (i = 0; i < MAX_PARTIDAS; i++) {
        bdp->partidas[i] = NULL;
    }
    return bdp;
}

void bdp_free(BDPartidas *bdp) {
    if (bdp == NULL) return;
    
    int i;
    for (i = 0; i < bdp->qtd; i++) {
        partida_free(bdp->partidas[i]);
    }
    free(bdp);
}

int bdp_carregar(BDPartidas *bdp, const char *arquivo, BDTimes *bdt) {
    FILE *f = fopen(arquivo, "r");
    if (f == NULL) return 0;
    
    char linha[256];
    // Pula o cabeçalho
    fgets(linha, sizeof(linha), f);
    
    while (fgets(linha, sizeof(linha), f)) {
        int id, t1, t2, g1, g2;
        // Lê os valores separados por vírgula
        sscanf(linha, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2);
        if (bdp->qtd < MAX_PARTIDAS) {
            bdp->partidas[bdp->qtd] = partida_criar(id, t1, t2, g1, g2);
            bdp->qtd++;
            
            // Busca os times e adiciona os gols para calcular as estatísticas
            Time *time1 = bdt_buscar_por_id(bdt, t1);
            Time *time2 = bdt_buscar_por_id(bdt, t2);
            
            if (time1 != NULL) time_adicionar_resultados(time1, g1, g2);
            if (time2 != NULL) time_adicionar_resultados(time2, g2, g1);
        }
    }
    
    fclose(f);
    return 1;
}

// Função auxiliar para largura do nome (reutilizada ou similar)
static int bdp_calcular_largura_times(BDTimes *bdt) {
    int max = 15;
    int qtd = bdt_get_qtd(bdt);
    for (int i = 0; i < qtd; i++) {
        Time *t = bdt_get_time_idx(bdt, i);
        int len = tamanho_texto(time_get_nome(t));
        if (len > max) max = len;
    }
    return max;
}

void bdp_consultar_partidas(BDPartidas *bdp, BDTimes *bdt, int modo, const char *busca) {
    if (bdp == NULL || bdt == NULL || busca == NULL) return;

    int i, encontrou = 0;
    int w = bdp_calcular_largura_times(bdt);

    printf("\n%-4s %-*s %-*s\n", "ID", w, "Mandante", w, "Visitante");
    for (int i = 0; i < (w * 2) + 12; i++) printf("-");
    printf("\n");

    for (i = 0; i < bdp->qtd; i++) {
        Partida *p = bdp->partidas[i];

        Time *t1 = bdt_buscar_por_id(bdt, partida_get_time1(p));
        Time *t2 = bdt_buscar_por_id(bdt, partida_get_time2(p));

        char *nome1 = time_get_nome(t1);
        char *nome2 = time_get_nome(t2);

        int bate = 0;
        if (strlen(busca) == 0) {
            bate = 1;
        } else if (modo == 1) { // Mandante
            bate = is_prefixo(nome1, busca);
        } else if (modo == 2) { // Visitante
            bate = is_prefixo(nome2, busca);
        } else { // Ambos
            bate = is_prefixo(nome1, busca) || is_prefixo(nome2, busca);
        }

        if (bate) {
            partida_imprimir(p, bdt, w);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Erro: Nenhuma partida encontrada.\n");
    }
}