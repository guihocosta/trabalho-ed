#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_times.h"
#include "utils.h"

#define MAX_TIMES 10

// Estrutura que gerencia a coleção de times
struct bd_times {
    Time *times[MAX_TIMES];
    int qtd; // Quantidade atual de times armazenados
};

// Cria e inicializa o banco de dados de times
BDTimes *bdt_criar(void) {
    BDTimes *bdt = (BDTimes*)malloc(sizeof(BDTimes));

    if (bdt == NULL)
        return NULL;

    bdt->qtd = 0;
    int i;
    for (i = 0; i < MAX_TIMES; i++)
        bdt->times[i] = NULL;
    
    return bdt;
}

// Libera a memória de todos os times carregados e do próprio gerenciador
void bdt_free(BDTimes *bdt) {
    if (bdt == NULL)
        return;

    int i;
    for (i = 0; i < bdt->qtd; i++)
        time_free(bdt->times[i]);
    
    free(bdt);
}

// Carrega os dados dos times a partir do arquivo CSV
int bdt_carregar(BDTimes *bdt, const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (f == NULL)
        return 0;
    
    char linha[256];
    // Ignorar a primeira linha (cabeçalho do CSV)
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }
    
    // Lê cada linha restante do arquivo
    while (fgets(linha, sizeof(linha), f)) {        
        int id;
        char nome[100];
        
        if (sscanf(linha, "%d,%99[^\n\r]", &id, nome) == 2) {
            if (bdt->qtd < MAX_TIMES) 
                bdt->times[bdt->qtd++] = time_criar(id, nome);
        }
    }
    
    fclose(f);
    return 1;
}

// Busca um time pelo seu ID
Time *bdt_buscar_por_id(BDTimes *bdt, int id) {
    if (bdt == NULL)
        return NULL;
        
    int i;
    for (i = 0; i < bdt->qtd; i++) {
        if (time_get_id(bdt->times[i]) == id) {
            return bdt->times[i];
        }
    }
    return NULL;
}

// Busca um time pelo nome ou prefixo usando a função utilitária
Time *bdt_buscar_por_nome(BDTimes *bdt, const char *nome) {
    if (!bdt || !nome) return NULL;
    
    int i;
    for (i = 0; i < bdt->qtd; i++) {
        char *nome_time = time_get_nome(bdt->times[i]);
        if (nome_time && is_prefixo(nome_time, nome)) {
            return bdt->times[i];
        }
    }
    return NULL;
}

// Imprime a tabela de classificação delegando a impressão para o TAD Time
void bdt_imprimir_tabela(BDTimes *bdt) {
    if (!bdt) return;
    
    printf("ID\tTime\t\t\tV\tE\tD\tGM\tGS\tS\tPG\n");
    int i;
    for (i = 0; i < bdt->qtd; i++) {
        time_imprimir(bdt->times[i]);
    }
}

int bdt_get_qtd(BDTimes *bdt) { return bdt ? bdt->qtd : 0; }

Time *bdt_get_time_idx(BDTimes *bdt, int idx) {
    if (!bdt || idx < 0 || idx >= bdt->qtd) return NULL;
    return bdt->times[idx];
}