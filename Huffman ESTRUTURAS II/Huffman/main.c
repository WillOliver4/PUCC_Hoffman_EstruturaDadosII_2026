#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./huffman.h"
#include "./compactar.h"
#include "./descompactar.h"

// Função principal que gerencia compactação/descompactação
int main() {
    int opcao;
    char arquivo_entrada[256];
    char arquivo_saida[256];

    printf("=== Menu Huffman ===\n");
    printf("1 - Compactar arquivo\n");
    printf("2 - Descompactar arquivo\n");
    printf("Entrada: arquivo existente. Saida: nome diferente da entrada.\n");
    printf("Se compactar, use saida com .huf.\n");
    printf("Acao: ");
    scanf("%d", &opcao);
    
    // Limpar o buffer do teclado após ler o inteiro
    while (getchar() != '\n');

    printf("Entrada: ");
    scanf("%s", arquivo_entrada);

    printf("Saida: ");
    scanf("%s", arquivo_saida);

    if (strcmp(arquivo_entrada, arquivo_saida) == 0) {
        printf("Entrada e saida devem ser diferentes.\n");
        return 1;
    }

    if (opcao == 1) {
        compactar_arquivo(arquivo_entrada, arquivo_saida);
        printf("Arquivo compactado com sucesso!\n");
    } else if (opcao == 2) {
        descompactar_arquivo(arquivo_entrada, arquivo_saida);
        printf("Arquivo descompactado com sucesso!\n");
    } else {
        printf("Opcao invalida. Digite 1 para compactar ou 2 para descompactar.\n");
        return 1;
    }
    return 0;
}
