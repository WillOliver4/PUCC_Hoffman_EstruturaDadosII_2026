#include <stdio.h>
#include <stdlib.h>
#include "codigo.h"
#include "fila.h"
#include "arvore_utils.h"

int main ()
{
    Codigo c;     // estrutura que armazena os bits dinamicamente
    char* txt;    // ponteiro para armazenar a representação em string

    // Inicializa o código (cria espaço para 8 bits inicialmente)
    novo_codigo(&c);

    // adiciona 1, 0, 1
    adiciona_bit(&c, 1);  // agora: 1
    adiciona_bit(&c, 0);  // agora: 10
    adiciona_bit(&c, 1);  // agora: 101

    // Converte os bits para string ("101")
    txt = toString(c);

    // Imprime o resultado
    printf("apos 1,0,1: \"%s\"\n", txt);

    // Libera memória da string criada por toString
    free(txt);

    // adiciona mais: 1, 1, 0, 0, 1
    // agora totaliza 8 bits (1 byte cheio)
    adiciona_bit(&c, 1);  // 1011
    adiciona_bit(&c, 1);  // 10111
    adiciona_bit(&c, 0);  // 101110
    adiciona_bit(&c, 0);  // 1011100
    adiciona_bit(&c, 1);  // 10111001 (8 bits completos)

    txt = toString(c);
    printf("apos 1,1,0,0,1: \"%s\"\n", txt);
    free(txt);

    // adiciona mais 3 bits
    // agora ultrapassa 1 byte → cria segundo byte
    adiciona_bit(&c, 0);  // começa novo byte
    adiciona_bit(&c, 1);
    adiciona_bit(&c, 1);

    txt = toString(c);
    printf("apos 0,1,1: \"%s\"\n", txt);
    free(txt);

    // remove 2 bits do final
    joga_fora_bit(&c);  // remove último bit
    joga_fora_bit(&c);  // remove mais um

    txt = toString(c);
    printf("apos 2 joga_fora_bit: \"%s\"\n", txt);
    free(txt);

    // remove mais 1 bit
    // agora deve voltar para 8 bits (1 byte só)
    joga_fora_bit(&c);

    txt = toString(c);
    printf("apos mais 1 joga_fora_bit (8 bits): \"%s\"\n", txt);
    free(txt);

    // Libera memória usada pela estrutura Codigo
    free_codigo(&c);

    return 0;
}
