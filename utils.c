#include <stdio.h>
#include <ctype.h>
#include "utils.h"

/* Compara se 'nome' inicia com 'termo' de forma insensível a maiúsculas */
int is_prefixo(char *nome, const char *termo) {
    if (nome == NULL || termo == NULL) {
        return 0;
    }
    
    int i = 0;
    while (termo[i] != '\0') {
        if (nome[i] == '\0' || tolower((unsigned char)nome[i]) != tolower((unsigned char)termo[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

/* Retorna o número de caracteres visíveis para evitar desalinhamento com acentos */
int tamanho_texto(const char *s) {
    if (s == NULL) {
        return 0;
    }
    int tam = 0;
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        /* Pula bytes de continuação de caracteres multi-byte (acentos) */
        if ((unsigned char)s[i] < 128 || (unsigned char)s[i] >= 192) {
            tam++;
        }
    }
    return tam;
}
