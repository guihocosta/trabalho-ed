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

// Função interna para calcular a largura necessária para a coluna de nomes
static int bdt_calcular_largura_nome(BDTimes *bdt) {
    int max = 15; // Largura mínima
    for (int i = 0; i < bdt->qtd; i++) {
        int len = tamanho_texto(time_get_nome(bdt->times[i]));
        if (len > max) max = len;
    }
    return max;
}

// Imprime a tabela de classificação delegando a impressão para o TAD Time
void bdt_imprimir_tabela(BDTimes *bdt) {
    if (!bdt) return;
    
    int w = bdt_calcular_largura_nome(bdt);

    printf("\n%-4s %-*s %3s %3s %3s %4s %4s %4s %4s\n",
           "ID", w, "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    
    // Imprime a linha separadora com o tamanho correto
    for (int i = 0; i < w + 42; i++) printf("-");
    printf("\n");

    for (int i = 0; i < bdt->qtd; i++) {
        time_imprimir(bdt->times[i], w);
    }
}

void bdt_consultar_times(BDTimes *bdt, const char *termo) {
    if (!bdt || !termo) return;

    int w = bdt_calcular_largura_nome(bdt);
    int encontrou = 0;

    printf("\n%-4s %-*s %3s %3s %3s %4s %4s %4s %4s\n",
           "ID", w, "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    
    for (int i = 0; i < w + 42; i++) printf("-");
    printf("\n");

    for (int i = 0; i < bdt->qtd; i++) {
        char *nome = time_get_nome(bdt->times[i]);
        if (is_prefixo(nome, termo)) {
            time_imprimir(bdt->times[i], w);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Erro: Nenhum time encontrado com o prefixo \"%s\".\n", termo);
    }
}

int bdt_get_qtd(BDTimes *bdt) { return bdt ? bdt->qtd : 0; }

Time *bdt_get_time_idx(BDTimes *bdt, int idx) {
    if (!bdt || idx < 0 || idx >= bdt->qtd) return NULL;
    return bdt->times[idx];
}