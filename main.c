#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bd_times.h"
#include "bd_partidas.h"

#define MAX_INPUT 256

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

int main(int argc, char *argv[]) {
    char *arq_times = "times.csv";
    char *arq_partidas = "partidas_completo.csv";

    // Se houver um argumento, ele será o arquivo de partidas
    if (argc > 1) arq_partidas = argv[1];

    BDTimes *bdt = bdt_criar();
    if (!bdt || !bdt_carregar(bdt, arq_times)) {
        printf("Erro ao carregar times de \"%s\".\n", arq_times);
        if (bdt) bdt_free(bdt);
        return 1;
    }

    BDPartidas *bdp = bdp_criar();
    if (!bdp || !bdp_carregar(bdp, arq_partidas, bdt)) {
        printf("Erro ao carregar partidas de \"%s\".\n", arq_partidas);
        if (bdp) bdp_free(bdp);
        bdt_free(bdt);
        return 1;
    }

    char opcao;
    char entrada[MAX_INPUT];
    int rodando = 1;

    while (rodando) {
        exibir_menu();
        
        // Se a leitura falhar, encerramos o loop
        if (scanf(" %c", &opcao) != 1) {
            rodando = 0;
        }
        
        if (rodando) {
            // Opção: SAIR
            if (opcao == 'Q' || opcao == 'q') {
                printf("Encerrando o sistema. Ate logo!\n");
                rodando = 0;
            } 
            
            // Opção 1: CONSULTAR TIME
            else if (opcao == '1') {
                printf("\nDigite o nome ou prefixo do time: ");
                scanf("%s", entrada);
                bdt_consultar_times(bdt, entrada);
            } 
            
            // Opção 2: CONSULTAR PARTIDAS
            else if (opcao == '2') {
                int modo;
                printf("\nEscolha o modo de consulta:\n");
                printf("1 - Por time mandante\n");
                printf("2 - Por time visitante\n");
                printf("3 - Por time mandante ou visitante\n");
                printf("4 - Retornar ao menu principal\n");
                if (scanf("%d", &modo) == 1) {
                    if (modo == 4) continue;
                    printf("Digite o nome: ");
                    scanf("%s", entrada);
                    bdp_consultar_partidas(bdp, bdt, modo, entrada);
                } else {
                    while (getchar() != '\n'); 
                    printf("Opcao invalida.\n");
                }
            } 
            
            // Opção 6: IMPRIMIR TABELA
            else if (opcao == '6') {
                printf("\nImprimindo classificacao...\n");
                bdt_imprimir_tabela(bdt);
            } 
            
            // Opções desabilitadas
            else if (opcao >= '3' && opcao <= '5') {
                printf("Funcionalidade ainda nao implementada.\n");
            } 
            
            // Opção Inválida
            else {
                printf("Opcao invalida. Tente novamente.\n");
            }
        }
    }

    bdp_free(bdp);
    bdt_free(bdt);
    return 0;
}