#ifndef UTILS_H
#define UTILS_H

// Verifica se 'nome' começa com 'termo'
int is_prefixo(char *nome, const char *termo);

// Retorna a quantidade de caracteres visíveis da string (ajuste para UTF-8)
int tamanho_texto(const char *s);

#endif