#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>
#include "codigo.h"

// Construção da árvore de Huffman a partir da tabela de frequências
void construir_arvore_huffman(Tabela_de_frequencias *tab, NoHuffman **raiz) {
    // Organiza os nós não nulos no início do vetor
    junte_nodos_no_inicio_do_vetor(tab);
    
    U16 n = tab->quantidade_de_posicoes_preenchidas;
    /*U16 é igual ao U8 só que dessa vez representa um número inteiro 
    sem sinal de 16 bits, ou seja, pode armazenar valores de 0 a 65535. 
    Ele é usado para contar a quantidade de posições preenchidas na tabela de frequências,
    o que é importante para construir a árvore de Huffman apenas com os bytes que realmente 
    aparecem no arquivo.*/
    // Cria lista de nós de Huffman a partir da tabela
    //memorid dinamica para arnzmaner os nós
    NoHuffman **nos = (NoHuffman **)malloc(n * sizeof(NoHuffman *));
    for (U16 i = 0; i < n; i++) {
        Ptr_de_no_de_arvore_binaria no_tab = tab->vetor[i];
        nos[i] = criar_no(no_tab->informacao.byte, no_tab->informacao.frequencia, NULL, NULL);
    }
    while (n > 1) {
        // Encontra os dois nós com menores frequências
        U16 min1 = 0, min2 = 1;
        if (nos[min1]->frequencia > nos[min2]->frequencia) {
            U16 temp = min1;
            min1 = min2;
            min2 = temp;
        }
        
        for (U16 i = 2; i < n; i++) {
            if (nos[i]->frequencia < nos[min1]->frequencia) {
                min2 = min1;
                min1 = i;
            } else if (nos[i]->frequencia < nos[min2]->frequencia) {
                min2 = i;
            }
        }
        
     // Cria novo nó combinando os dois menores
        NoHuffman *novo_no = criar_no(0, nos[min1]->frequencia + nos[min2]->frequencia, nos[min1], nos[min2]);
        
        // Atualização da lista de nós
        if (min1 < min2) {
            nos[min1] = novo_no;
            nos[min2] = nos[n - 1];
        } else {
            nos[min2] = novo_no;
            nos[min1] = nos[n - 1];
        }
        n--;
    }
    
    *raiz = nos[0];
    free(nos);
}

// Gerar os códigos de Huffman para cada byte
void gerar_codigos(NoHuffman *no, Codigo *codigo_atual, Codigo *tabela_codigos[256]) {
    if (no == NULL) return;
    
    // Se for nó folha, armazena o código na tabela
    if (no->esquerda == NULL && no->direita == NULL) {
        Codigo *copia = (Codigo *)malloc(sizeof(Codigo));
        if (!clone(*codigo_atual, copia)) {
            printf("Erro ao clonar código\n");
            exit(1);
        }
        tabela_codigos[no->byte] = copia;
        return;
    }
    
    // Vai para esquerda (adiciona 0)
    if (no->esquerda != NULL) {
        adiciona_bit(codigo_atual, 0);
        gerar_codigos(no->esquerda, codigo_atual, tabela_codigos);
        joga_fora_bit(codigo_atual);
    }
    
    // Vai para direita (adiciona 1)
    if (no->direita != NULL) {
        adiciona_bit(codigo_atual, 1);
        gerar_codigos(no->direita, codigo_atual, tabela_codigos);
        joga_fora_bit(codigo_atual);
    }
}