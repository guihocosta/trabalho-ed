#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bd_times.h"
#include "bd_partidas.h"

#define ARQUIVO_TIMES    "times.csv"
#define ARQUIVO_PARTIDAS "bd_partidas.csv"
#define MAX_INPUT        256

// Limpa o buffer de entrada
static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Lê uma linha do stdin, removendo o '\n' final
static void ler_linha(char *buf, int tamanho) {
    if (fgets(buf, tamanho, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
    }
}

// ── Opção 1: Consultar time ──
static void menu_consultar_time(BDTimes *bdt) {
    char termo[MAX_INPUT];

    printf("\nDigite o nome ou prefixo do time: ");
    ler_linha(termo, sizeof(termo));

    int qtd = bdt_get_qtd(bdt);
    int encontrou = 0;

    printf("\n%-4s %-12s %3s %3s %3s %4s %4s %4s %4s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        Time *t = bdt_get_time_idx(bdt, i);
        if (t && strncasecmp(time_get_nome(t), termo, strlen(termo)) == 0) {
            time_imprimir(t);
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhum time encontrado com o prefixo \"%s\".\n", termo);
    }
}

// ── Opção 2: Consultar partidas ──
static void menu_consultar_partidas(BDPartidas *bdp, BDTimes *bdt) {
    int opcao;
    char termo[MAX_INPUT];

    printf("\nEscolha o modo de consulta:\n");
    printf("  1 - Por time mandante\n");
    printf("  2 - Por time visitante\n");
    printf("  3 - Por time mandante ou visitante\n");
    printf("  4 - Retornar ao menu principal\n");
    printf("Opcao: ");

    if (scanf("%d", &opcao) != 1) {
        limpar_buffer();
        printf("Entrada invalida.\n");
        return;
    }
    limpar_buffer();

    if (opcao == 4) return;

    if (opcao < 1 || opcao > 3) {
        printf("Opcao invalida.\n");
        return;
    }

    printf("Digite o nome: ");
    ler_linha(termo, sizeof(termo));

    char termo_modo[MAX_INPUT + 4];
    if (opcao == 1) {
        snprintf(termo_modo, sizeof(termo_modo), "M:%s", termo);
    } else if (opcao == 2) {
        snprintf(termo_modo, sizeof(termo_modo), "V:%s", termo);
    } else {
        snprintf(termo_modo, sizeof(termo_modo), "%s", termo);
    }

    bdp_consultar_partidas(bdp, bdt, termo_modo);
}

// ── Opção 6: Imprimir tabela de classificação ──
static void menu_imprimir_tabela(BDTimes *bdt) {
    printf("\nImprimindo classificacao...\n");
    bdt_imprimir_tabela(bdt);
}

// ── Menu principal ──
static void exibir_menu(void) {
    printf("\n===== Sistema de Gerenciamento de Partidas =====\n");
    printf("  1 - Consultar time\n");
    printf("  2 - Consultar partidas\n");
    printf("  3 - Atualizar partida  (desabilitado)\n");
    printf("  4 - Remover partida    (desabilitado)\n");
    printf("  5 - Inserir partida    (desabilitado)\n");
    printf("  6 - Imprimir tabela de classificacao\n");
    printf("  Q - Sair\n");
    printf("================================================\n");
    printf("Opcao: ");
}

int main(void) {
    // Inicializa e carrega os TADs
    BDTimes *bdt = bdt_criar();
    if (!bdt) {
        fprintf(stderr, "Erro ao criar BDTimes.\n");
        return EXIT_FAILURE;
    }
    if (!bdt_carregar(bdt, ARQUIVO_TIMES)) {
        fprintf(stderr, "Erro ao carregar \"%s\".\n", ARQUIVO_TIMES);
        bdt_free(bdt);
        return EXIT_FAILURE;
    }

    BDPartidas *bdp = bdp_criar();
    if (!bdp) {
        fprintf(stderr, "Erro ao criar BDPartidas.\n");
        bdt_free(bdt);
        return EXIT_FAILURE;
    }
    if (!bdp_carregar(bdp, ARQUIVO_PARTIDAS, bdt)) {
        fprintf(stderr, "Erro ao carregar \"%s\".\n", ARQUIVO_PARTIDAS);
        bdp_free(bdp);
        bdt_free(bdt);
        return EXIT_FAILURE;
    }

    // Loop principal
    char entrada[MAX_INPUT];
    int rodando = 1;

    while (rodando) {
        exibir_menu();
        ler_linha(entrada, sizeof(entrada));

        if (strlen(entrada) == 0) continue;

        char opcao = entrada[0];

        switch (opcao) {
            case '1':
                menu_consultar_time(bdt);
                break;
            case '2':
                menu_consultar_partidas(bdp, bdt);
                break;
            case '3':
            case '4':
            case '5':
                printf("Funcionalidade ainda nao implementada.\n");
                break;
            case '6':
                menu_imprimir_tabela(bdt);
                break;
            case 'Q':
            case 'q':
                printf("Encerrando o sistema. Ate logo!\n");
                rodando = 0;
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }

    // Libera memória
    bdp_free(bdp);
    bdt_free(bdt);

    return EXIT_SUCCESS;
}