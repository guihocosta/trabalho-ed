#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bd_times.h"
#include "bd_partidas.h"

#define MAX_INPUT 256

/* Limpa o buffer de entrada do teclado */
static void limpar_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Exibe o menu de opções */
static void exibir_menu(void) {
    printf("\n===== Sistema de Gerenciamento de Partidas =====\n");
    printf("  1 - Consultar time\n");
    printf("  2 - Consultar partidas\n");
    printf("  3 - Atualizar partida\n");
    printf("  4 - Remover partida\n");
    printf("  5 - Inserir partida\n");
    printf("  6 - Imprimir tabela de classificacao\n");
    printf("  Q - Sair\n");
    printf("================================================\n");
    printf("Opcao: ");
}

/* Exibe a prévia da partida formatada para confirmação */
static void imprimir_previa_partida(int id, int t1_id, int t2_id, int g1, int g2, BDTimes *bdt) {
    int w = bdt_get_largura_nome_max(bdt);
    printf("%-4s %-*s %-*s %7s %7s\n", "ID", w, "Time1", w, "Time2", "Placar1", "Placar2");
    Time *time1 = bdt_buscar_por_id(bdt, t1_id);
    Time *time2 = bdt_buscar_por_id(bdt, t2_id);
    char *n1 = time_get_nome(time1);
    char *n2 = time_get_nome(time2);
    printf("%-4d %-*s %-*s %7d %7d\n", id, w, n1, w, n2, g1, g2);
}

/* Pede confirmação e exibe previa */
static int confirmar_operacao(const char *pergunta, int id, int t1_id, int t2_id, int g1, int g2, BDTimes *bdt) {
    printf("\n%s\n", pergunta);
    imprimir_previa_partida(id, t1_id, t2_id, g1, g2, bdt);
    printf("Confirmar: ");
    char conf;
    if (scanf(" %c", &conf) == 1) {
        limpar_entrada();
        return (conf == 'S' || conf == 's');
    }
    return 0;
}

/* Helper para buscar por time e selecionar a partida por ID */
static Partida *buscar_e_selecionar_partida(BDPartidas *bdp, BDTimes *bdt) {
    char entrada[MAX_INPUT];
    printf("Digite o nome do time para buscar a partida: ");
    if (scanf("%255s", entrada) != 1) {
        limpar_entrada();
        return NULL;
    }
    limpar_entrada();
    
    bdp_consultar_partidas(bdp, bdt, 3, entrada);
    
    int id;
    printf("\nInforme o ID da partida: ");
    if (scanf("%d", &id) != 1) {
        limpar_entrada();
        printf("ID invalido.\n");
        return NULL;
    }
    limpar_entrada();
    
    Partida *p = bdp_buscar_por_id(bdp, id);
    if (p == NULL) {
        printf("Erro: Partida com ID %d nao encontrada.\n", id);
    }
    return p;
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
    if (bdp == NULL || bdp_carregar(bdp, arq_partidas) == 0) {
        printf("Erro ao carregar partidas de \"%s\".\n", arq_partidas);
        if (bdp != NULL) {
            bdp_free(bdp);
        }
        bdt_free(bdt);
        return 1;
    }

    /* Ajusta as estatísticas iniciais baseadas nos dados lidos */
    bdt_recalcular_estatisticas(bdt, bdp);

    char opcao;
    char entrada[MAX_INPUT];
    int rodando = 1;

    /* Loop principal do menu */
    while (rodando) {
        exibir_menu();
        
        if (scanf(" %c", &opcao) != 1) {
            rodando = 0;
        }
        limpar_entrada();
        
        if (rodando) {
            if (opcao == 'Q' || opcao == 'q') {
                printf("Encerrando o sistema. Salvando alteracoes...\n");
                bdp_salvar(bdp, "bd_partidas.csv");
                bdt_salvar_classificacao(bdt, "bd_classificacao.csv");
                printf("Ate logo!\n");
                rodando = 0;
            } 
            else if (opcao == '1') {
                printf("\nDigite o nome ou prefixo do time: ");
                if (scanf("%255s", entrada) == 1) {
                    limpar_entrada();
                    bdt_consultar_times(bdt, entrada);
                }
            } 
            else if (opcao == '2') {
                int modo;
                printf("\nEscolha o modo de consulta:\n");
                printf("1 - Por time mandante\n");
                printf("2 - Por time visitante\n");
                printf("3 - Por time mandante ou visitante\n");
                printf("4 - Retornar ao menu principal\n");
                printf("Opcao: ");
                if (scanf("%d", &modo) == 1) {
                    limpar_entrada();
                    if (modo >= 1 && modo <= 3) {
                        printf("Digite o nome: ");
                        if (scanf("%255s", entrada) == 1) {
                            limpar_entrada();
                            bdp_consultar_partidas(bdp, bdt, modo, entrada);
                        }
                    } else if (modo != 4) {
                        printf("Opcao invalida.\n");
                    }
                } else {
                    limpar_entrada();
                    printf("Opcao invalida.\n");
                }
            } 
            else if (opcao == '3') {
                /* Opção 3 — Atualizar partida */
                printf("\n--- Atualizar Partida ---\n");
                Partida *p = buscar_e_selecionar_partida(bdp, bdt);
                if (p != NULL) {
                    char str1[32], str2[32];
                    printf("Digite os novos valores para Placar1 e Placar2 ('-' mantem o atual):\n");
                    if (scanf("%31s %31s", str1, str2) == 2) {
                        limpar_entrada();
                        
                        int g1 = (strcmp(str1, "-") == 0) ? partida_get_gols1(p) : atoi(str1);
                        int g2 = (strcmp(str2, "-") == 0) ? partida_get_gols2(p) : atoi(str2);
                        
                        if (confirmar_operacao("Confirma os novos valores? (S/N)", 
                                               partida_get_id(p), partida_get_time1(p), partida_get_time2(p), g1, g2, bdt)) {
                            partida_set_placar(p, g1, g2);
                            bdt_recalcular_estatisticas(bdt, bdp);
                            printf("Registro atualizado com sucesso.\n");
                        } else {
                            printf("Operacao cancelada.\n");
                        }
                    }
                }
            } 
            else if (opcao == '4') {
                /* Opção 4 — Remover partida */
                printf("\n--- Remover Partida ---\n");
                Partida *p = buscar_e_selecionar_partida(bdp, bdt);
                if (p != NULL) {
                    if (confirmar_operacao("Tem certeza de que deseja excluir o registro abaixo? (S/N)", 
                                           partida_get_id(p), partida_get_time1(p), partida_get_time2(p), partida_get_gols1(p), partida_get_gols2(p), bdt)) {
                        if (bdp_remover_por_id(bdp, partida_get_id(p))) {
                            bdt_recalcular_estatisticas(bdt, bdp);
                            printf("Registro removido com sucesso.\n");
                        } else {
                            printf("Erro ao remover registro.\n");
                        }
                    } else {
                        printf("Operacao cancelada.\n");
                    }
                }
            } 
            else if (opcao == '5') {
                /* Opção 5 — Inserir partida */
                printf("\n--- Inserir Partida ---\n");
                bdt_listar_ids(bdt);
                
                int t1_id, t2_id, g1, g2;
                
                printf("Digite o ID do Time 1 (Mandante): ");
                if (scanf("%d", &t1_id) != 1) {
                    limpar_entrada();
                    printf("ID invalido.\n");
                    continue;
                }
                limpar_entrada();
                
                printf("Digite o ID do Time 2 (Visitante): ");
                if (scanf("%d", &t2_id) != 1) {
                    limpar_entrada();
                    printf("ID invalido.\n");
                    continue;
                }
                limpar_entrada();
                
                printf("Digite os gols do Time 1: ");
                if (scanf("%d", &g1) != 1) {
                    limpar_entrada();
                    printf("Valor de gols invalido.\n");
                    continue;
                }
                limpar_entrada();
                
                printf("Digite os gols do Time 2: ");
                if (scanf("%d", &g2) != 1) {
                    limpar_entrada();
                    printf("Valor de gols invalido.\n");
                    continue;
                }
                limpar_entrada();
                
                Time *time1 = bdt_buscar_por_id(bdt, t1_id);
                Time *time2 = bdt_buscar_por_id(bdt, t2_id);
                
                if (time1 == NULL || time2 == NULL) {
                    printf("Erro: Um ou ambos os times nao existem.\n");
                } else if (t1_id == t2_id) {
                    printf("Erro: Um time nao pode jogar contra si mesmo.\n");
                } else {
                    int novo_id = bdp_gerar_novo_id(bdp);
                    if (confirmar_operacao("Confirma a insercao? (S/N)", novo_id, t1_id, t2_id, g1, g2, bdt)) {
                        Partida *nova = partida_criar(novo_id, t1_id, t2_id, g1, g2);
                        if (nova != NULL) {
                            if (bdp_inserir(bdp, nova)) {
                                bdt_recalcular_estatisticas(bdt, bdp);
                                printf("Registro inserido com sucesso.\n");
                            } else {
                                partida_free(nova);
                                printf("Erro de memoria ao inserir partida.\n");
                            }
                        } else {
                            printf("Erro ao alocar memoria para a nova partida.\n");
                        }
                    } else {
                        printf("Operacao cancelada.\n");
                    }
                }
            }
            else if (opcao == '6') {
                printf("\nImprimindo classificacao...\n");
                bdt_imprimir_tabela(bdt);
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
