#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bd_times.h"
#include "bd_partidas.h"

#define MAX_INPUT 256

/* Exibe o menu de opções */
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

/* Ponto de entrada do sistema */
int main(int argc, char *argv[]) {
    char *arq_times = "times.csv";
    char *arq_partidas = "partidas_completo.csv";

    /* Permite passar um arquivo de partidas customizado via CLI */
    if (argc > 1) {
        arq_partidas = argv[1];
    }

    /* Cria e carrega o banco de times */
    BDTimes *bdt = bdt_criar();
    if (bdt == NULL || bdt_carregar(bdt, arq_times) == 0) {
        printf("Erro ao carregar times de \"%s\".\n", arq_times);
        if (bdt != NULL) {
            bdt_free(bdt);
        }
        return 1;
    }

    /* Cria e carrega o banco de partidas */
    BDPartidas *bdp = bdp_criar();
    if (bdp == NULL || bdp_carregar(bdp, arq_partidas, bdt) == 0) {
        printf("Erro ao carregar partidas de \"%s\".\n", arq_partidas);
        if (bdp != NULL) {
            bdp_free(bdp);
        }
        bdt_free(bdt);
        return 1;
    }

    char opcao;
    char entrada[MAX_INPUT];
    int rodando = 1;

    /* Loop principal do menu */
    while (rodando) {
        exibir_menu();
        
        if (scanf(" %c", &opcao) != 1) {
            rodando = 0;
        }
        
        if (rodando) {
            if (opcao == 'Q' || opcao == 'q') {
                printf("Encerrando o sistema. Ate logo!\n");
                rodando = 0;
            } 
            else if (opcao == '1') {
                printf("\nDigite o nome ou prefixo do time: ");
                scanf("%s", entrada);
                bdt_consultar_times(bdt, entrada);
            } 
            else if (opcao == '2') {
                int modo;
                printf("\nEscolha o modo de consulta:\n");
                printf("1 - Por time mandante\n");
                printf("2 - Por time visitante\n");
                printf("3 - Por time mandante ou visitante\n");
                printf("4 - Retornar ao menu principal\n");
                if (scanf("%d", &modo) == 1) {
                    if (modo != 4) {
                        printf("Digite o nome: ");
                        scanf("%s", entrada);
                        bdp_consultar_partidas(bdp, bdt, modo, entrada);
                    }
                } else {
                    while (getchar() != '\n'); 
                    printf("Opcao invalida.\n");
                }
            } 
            else if (opcao == '6') {
                printf("\nImprimindo classificacao...\n");
                bdt_imprimir_tabela(bdt);
            } 
            else if (opcao >= '3' && opcao <= '5') {
                printf("Funcionalidade ainda nao implementada.\n");
            } 
            else {
                printf("Opcao invalida. Tente novamente.\n");
            }
        }
    }

    /* Libera toda a memória antes de encerrar */
    bdp_free(bdp);
    bdt_free(bdt);
    
    return 0;
}
