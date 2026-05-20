#ifndef TIME_H
#define TIME_H

/* Representação do Time */
typedef struct time Time;

/* Cria um time com valores iniciais zerados */
Time *time_criar(int id, const char *nome);

/* Calcula os pontos ganhos (3*V + E) */
int time_calcular_pontos(Time *t);

/* Calcula o saldo de gols (GM - GS) */
int time_calcular_saldo_gols(Time *t);

/* Libera a memória do time */
void time_free(Time *t);

/* Funções para acessar os dados do time (Getters usados) */
int time_get_id(Time *t);
char *time_get_nome(Time *t);

/* Atualiza as estatísticas após uma partida */
void time_adicionar_resultados(Time *t, int gols_pro, int gols_contra);

/* Imprime o time formatado para a tabela */
void time_imprimir(Time *t, int largura_nome);

#endif
