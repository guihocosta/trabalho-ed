#ifndef TIME_H
#define TIME_H

typedef struct time Time; // Representa o Time

// Cria o Time com os valores iniciais zerados
Time *time_criar(int id, const char *nome);

// Calcula a quantidade de pontos
int time_calcular_pontos(Time *t);

// Calcula o saldo de gols
int time_calcular_saldo_gols(Time *t);

// Liberar o ponteiro de Time
void time_free(Time *t);

// Acesso aos dados internos da Struct Time
int time_get_id(Time *t);
char *time_get_nome(Time *t);
int time_get_vitorias(Time *t);
int time_get_empates(Time *t);
int time_get_derrotas(Time *t);
int time_get_gols_marcados(Time *t);
int time_get_gols_sofridos(Time *t);

// Função para atualizar os dados após uma partida
void time_adicionar_resultados(Time *t, int gols_pro, int gols_contra);

#endif
