#include <stdlib.h>
#include "codigo.h"

boolean novo_codigo (Codigo* c /* por referência */) //Cria e inicializa um novo Codigo
{
    c->byte=(U8*)malloc(1*sizeof(U8)); // Aloca 1 byte (8 bits)
    if (c->byte==NULL) return false;

    c->capacidade=8; //capacidade inicial: 8 bits
    c->tamanho=0; //nenhum bit ainda armazenado
    return true;
}

void free_codigo (Codigo* c /* por referência */) //Libera a memória usada pelo Codigo
{
    if (c->byte!=NULL) free(c->byte); //libera memória
    c->byte=NULL; //evita ponteiro pendente
}

boolean adiciona_bit (Codigo* c /* por referência */,
                      U8 valor /* 0 ou 1 */) //Adiciona um bit (0 ou 1) ao final do Codigo
{
    if (c->tamanho==c->capacidade) //Se não há espaço suficiente, aumenta a capacidade
    {
        //Aloca mais 1 byte (8 bits a mais)
        U8* novo=(U8*)malloc((c->capacidade/8+1)*sizeof(U8));
        if (novo==NULL) return false;

        for(int i=0;i<c->capacidade/8;i++) //Copia os bytes antigos
            novo[i]=c->byte[i];

        free(c->byte); //libera memória antiga
        c->byte=novo; //aponta para nova memória

        c->capacidade+=8; //aumenta capacidade em 8 bits
    }
    c->byte[c->capacidade/8-1]<<=1; //Desloca bits do último byte para a esquerda
    
    if (valor==1) //Se o valor for 1, ativa o bit menos significativo
        c->byte[c->capacidade/8-1]|=1;

    c->tamanho++;//incrementa quantidade de bits
    return true;
}


boolean joga_fora_bit (Codigo* c /* por referência */) //Remove o último bit armazenado
{
    if (c->tamanho==0) return false; //nada para remover

    c->byte[c->capacidade/8-1]>>=1; //Desloca bits do último byte para a direita (remove último bit)
    c->tamanho--;

    if (c->capacidade>8 && c->capacidade-c->tamanho==8) //Se sobrou 1 byte vazio, reduz a capacidade
    {
        U8* novo=(U8*)malloc((c->capacidade/8-1)*sizeof(U8)); //Aloca um byte a menos
        if (novo==NULL) return false;

        for(int i=0;i<c->tamanho/8;i++) //Copia os dados úteis
            novo[i]=c->byte[i];

        free(c->byte);
        c->byte=novo;

        c->capacidade-=8; //reduz capacidade
    }
    
    return true;
}
//Retorna um byte específico do Codigo
boolean pega_byte (Codigo c /* por valor "*/,
                   U8 qual /* por valor */,
                   U8* b /*por referencia */)
{
	if (qual>c.capacidade/8) return false;  //Verifica se índice é válido
	
	*b = c.byte[qual]; //Retorna o byte solicitado por referência
	return true;
}

boolean clone (Codigo original, /* por valor */
               Codigo* copia    /* por referencia */) //Cria uma cópia idêntica do Codigo original
{
    copia->byte=(U8*)malloc(original.capacidade/8*sizeof(U8)); //Aloca memória para os bytes da cópia
    if (copia->byte==NULL) return false; 


    for(int i=0;i<original.capacidade/8;i++) //Copia os bytes do original para a cópia
        copia->byte[i]=original.byte[i];

    copia->capacidade=original.capacidade; //Copia a capacidade
    copia->tamanho=original.tamanho; 
    return true;
}

char* toString (Codigo c) //Converte o Codigo para uma string de '0' e '1' 
{
    char* retorno = (char*)malloc((c.tamanho+3)*sizeof(char)); //Aloca memória para a strings (bits + margem de segurança)
    if (retorno == NULL) return NULL;

    int bits_ultimo_byte = c.tamanho % 8; //Calcula quantos bits estão no último byte (pode ser de 1 a 8)
    
    if (bits_ultimo_byte == 0) bits_ultimo_byte = 8; //se tamanho é multiplo de 8, o ultimo byte está cheio (8 bits)

    int pos = 0; 
    for (int b = 0; b < c.capacidade/8; b++) //Percorre cada byte do Codigo
    {
        int bits_neste_byte = (b < c.capacidade/8-1) ? 8 : bits_ultimo_byte; //Determina quantos bits estão neste byte 
        for (int i = bits_neste_byte-1; i >= 0; i--) //Percorre os bits do byte do mais significativo para o menos significativo
            retorno[pos++] = ((c.byte[b] >> i) & 1) ? '1' : '0';
    }

    retorno[c.tamanho] = '\0'; //Retorna caractere de fim de string
    return retorno;
}
