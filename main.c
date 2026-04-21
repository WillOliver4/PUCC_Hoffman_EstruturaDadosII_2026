#include <stdio.h>
#include <stdlib.h>
#include "codigo.h"

int main ()
{
    Codigo c;
    char* txt;

    novo_codigo(&c);

    // adiciona 1, 0, 1
    adiciona_bit(&c, 1);
    adiciona_bit(&c, 0);
    adiciona_bit(&c, 1);
    txt = toString(c);
    printf("apos 1,0,1: \"%s\"\n", txt);
    free(txt);

    // adiciona mais: 1, 1, 0, 0, 1 (total 8 bits, byte cheio)
    adiciona_bit(&c, 1);
    adiciona_bit(&c, 1);
    adiciona_bit(&c, 0);
    adiciona_bit(&c, 0);
    adiciona_bit(&c, 1);
    txt = toString(c);
    printf("apos 1,1,0,0,1: \"%s\"\n", txt);
    free(txt);

    // adiciona mais 3 bits, passando para o segundo byte
    adiciona_bit(&c, 0);
    adiciona_bit(&c, 1);
    adiciona_bit(&c, 1);
    txt = toString(c);
    printf("apos 0,1,1: \"%s\"\n", txt);
    free(txt);

    // remove 2 bits do topo
    joga_fora_bit(&c);
    joga_fora_bit(&c);
    txt = toString(c);
    printf("apos 2 joga_fora_bit: \"%s\"\n", txt);
    free(txt);

    // remove ate esvaziar o segundo byte (volta a 8 bits)
    joga_fora_bit(&c);
    txt = toString(c);
    printf("apos mais 1 joga_fora_bit (8 bits): \"%s\"\n", txt);
    free(txt);

    free_codigo(&c);
    return 0;
}
