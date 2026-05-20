#include <stdio.h>
#include <ctype.h>
#include "utils.h"

int is_prefixo(char *nome, const char *termo) {
    if (nome == NULL || termo == NULL)
        return 0;
    
    int i = 0;
    while (termo[i] != '\0') {
        if (nome[i] == '\0' || tolower((unsigned char)nome[i]) != tolower((unsigned char)termo[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

int tamanho_texto(const char *s) {
    if (s == NULL) return 0;
    int tam = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        // Ignora bytes de continuação de caracteres UTF-8 (acentos)
        if ((unsigned char)s[i] < 128 || (unsigned char)s[i] >= 192) {
            tam++;
        }
    }
    return tam;
}