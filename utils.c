#include <stdio.h>
#include "utils.h"

int is_prefixo(char *nome, const char *termo) {
    if (nome == NULL || termo == NULL)
        return 0;
    
    int i = 0;
    while (termo[i] != '\0') {
        if (nome[i] == '\0' || nome[i] != termo[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}