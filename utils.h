#ifndef UTILS_H
#define UTILS_H

/* Verifica se uma string começa com outra (prefixo) */
int is_prefixo(char *nome, const char *termo);

/* Retorna o tamanho visual do texto (tratando UTF-8) */
int tamanho_texto(const char *s);

#endif
