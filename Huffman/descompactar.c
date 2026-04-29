#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "descompactar.h"
#include "compactar.h"

void descompactar_arquivo(const char *nome_entrada, const char *nome_saida) {
	
    FILE *entrada = abrir_arquivo_entrada(nome_entrada, "rb");
    if (entrada == NULL) {
        exit(1);
    }
    
    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);
    
    // Ler o tamanho original e a tabela de frequências
    U64 tamanho_original;
    if (fread(&tamanho_original, sizeof(U64), 1, entrada) != 1) {
        printf("Erro ao ler tamanho original\n");
        fclose(entrada);
        exit(1);
    }

    for (U16 i = 0; i < 256; i++) {
        U64 freq;
        if (fread(&freq, sizeof(U64), 1, entrada) != 1) {
            printf("Erro ao ler frequências\n");
            fclose(entrada);
            exit(1);
        }
        
        if (freq > 0) {
            Elemento elem;
            elem.byte = (U8)i;
            elem.frequencia = freq;
            
            if (!novo_no_de_arvore_binaria(&(tab.vetor[i]), NULL, elem, NULL)) {
                printf("Erro ao criar nó da árvore\n");
                fclose(entrada);
                exit(1);
            }
            tab.quantidade_de_posicoes_preenchidas++;
        }
    }
    
    // Reconstrução da árvore de Huffman
    NoHuffman *raiz = NULL;
    construir_arvore_huffman(&tab, &raiz);
    
    // ====================== DESCOMPACTAÇÃO ======================
    FILE *saida = fopen(nome_saida, "wb");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saída\n");
        fclose(entrada);
        liberar_arvore(raiz);
        exit(1);
    }
    
    NoHuffman *no_atual = raiz;
    U8 byte_lido;
    int bits_restantes = 0;        // Bits ainda não processados no byte atual
    U64 bytes_escritos = 0;

    // Loop principal: continua até escrever todos os bytes originais
    while (bytes_escritos < tamanho_original) {
        
        // Se não há mais bits no byte atual, lê o próximo byte do arquivo compactado
        if (bits_restantes == 0) {
            if (fread(&byte_lido, sizeof(U8), 1, entrada) != 1) {
                printf("Erro: fim inesperado do arquivo compactado (dados corrompidos?)\n");
                break;
            }
            bits_restantes = 8;
        }
        
        // Processa os bits restantes do byte atual
        while (bits_restantes > 0 && bytes_escritos < tamanho_original) {
            // Pega o bit mais significativo e desloca o byte para a esquerda
            U8 bit = (byte_lido >> 7) & 1;
            byte_lido <<= 1;
            bits_restantes--;
            
            // Navega na árvore
            if (bit == 0) {
                no_atual = no_atual->esquerda;
            } else {
                no_atual = no_atual->direita;
            }
            
            // Se chegou em uma folha (símbolo encontrado)
            if (no_atual->esquerda == NULL && no_atual->direita == NULL) {
                fwrite(&no_atual->byte, sizeof(U8), 1, saida);
                bytes_escritos++;
                no_atual = raiz;   // Volta para a raiz para o próximo símbolo
                
                // Se já escreveu tudo que precisava, podemos parar imediatamente
                if (bytes_escritos >= tamanho_original) {
                    goto fim_descompactacao;
                }
            }
        }
    }

fim_descompactacao:
    
    // Limpeza final
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}