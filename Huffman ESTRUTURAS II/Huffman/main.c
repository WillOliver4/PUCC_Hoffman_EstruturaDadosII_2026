#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigo.h"
#include "tabela_de_frequencias.h"

// funçaõ auxiliar para organizar os nós no início do vetor
boolean novo_no_de_arvore_binaria(Ptr_de_no_de_arvore_binaria* novo,
                                 Ptr_de_no_de_arvore_binaria esq,
                                 Elemento inf,
                                 Ptr_de_no_de_arvore_binaria dir);

// Estrutura para representar um nó da árvore de Huffman
// U8 é um tipo definido em meustiposonlinegdb.h que representa um byte (unsigned char), e U64 é um tipo que representa uma frequência (unsigned long int).
// U64 é usado para armazenar a frequência de cada byte, permitindo contar até 2^64 - 1 ocorrências, o que é suficiente para arquivos muito grandes. A estrutura NoHuffman tem ponteiros para os filhos esquerdo e direito, permitindo a construção da árvore binária necessária para o algoritmo de Huffman.
typedef struct NoHuffman {
    U8 byte;                // Byte armazenado no nó
    U64 frequencia;         // Frequência do byte 
    struct NoHuffman *esquerda;  
    struct NoHuffman *direita;   
} NoHuffman;

// Funções principais
void construir_arvore_huffman(Tabela_de_frequencias *tab, NoHuffman **raiz);
void gerar_codigos(NoHuffman *no, Codigo *codigo_atual, Codigo *tabela_codigos[256]);
void compactar_arquivo(const char *nome_entrada, const char *nome_saida);
void descompactar_arquivo(const char *nome_entrada, const char *nome_saida);
void liberar_arvore(NoHuffman *no);
NoHuffman *criar_no(U8 byte, U64 frequencia, NoHuffman *esquerda, NoHuffman *direita);

// Função principal que gerencia compactação/descompactação
int main(int argc, char *argv[]) {
// argc é o parametro main que indica o numnero total de argumentos passados, argument count
// agrv é uma array de ponteiros para strings, cada elemento é um arghumento.
//agrv[0] é o nome do programa, agrv[1] é a opção (-c ou -d), agrv[2] é o arquivo de entrada e agrv[3] é o arquivo de saída.
    if (argc != 4) {
        printf("Uso:\n");
        printf("  Compactar: %s -c arquivo_entrada arquivo_saida.huf\n", argv[0]);
        printf("  Descompactar: %s -d arquivo_entrada.huf arquivo_saida\n", argv[0]);
        return 1;
    }
// -c = compactar, -d = descompactar
    // Decide entre compactar ou descompactar
//strcmp é uma função da biblioteca string.h que compara duas strings. Retorna 0 se as strings são iguais, um valor negativo se a primeira string é menor que a segunda, e um valor positivo se a primeira string é maior que a segunda.
//argv[1] é a opção passada pelo usuário, que deve ser "-c" para compactar ou "-d" para descompactar. A função strcmp compara essa opção com as strings "-c" e "-d" para determinar qual ação executar.

    if (strcmp(argv[1], "-c") == 0) {
        compactar_arquivo(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-d") == 0) {
        descompactar_arquivo(argv[2], argv[3]);
    } else {
        printf("Opção inválida. Use -c para compactar ou -d para descompactar.\n");
        return 1;
    }
    return 0;
}

// Criar um novo nó da árvore de Huffman
NoHuffman *criar_no(U8 byte, U64 frequencia, NoHuffman *esquerda, NoHuffman *direita) {
    NoHuffman *no = (NoHuffman *)malloc(sizeof(NoHuffman));
    if (no == NULL) return NULL;
    
    no->byte = byte;
    no->frequencia = frequencia;
    no->esquerda = esquerda;
    no->direita = direita;
    
    return no;
}

// Liberar a memória da árvore de Huffman 
void liberar_arvore(NoHuffman *no) {
    if (no == NULL) return;
    
    liberar_arvore(no->esquerda);
    liberar_arvore(no->direita);
    free(no);
}

// Construir a árvore de Huffman a partir da tabela de frequências
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
    
    // Construção da árvore de Huffman
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

void compactar_arquivo(const char *nome_entrada, const char *nome_saida) {
	
    //Calcular frequências dos bytes no arquivo
    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);
    
    FILE *entrada = fopen(nome_entrada, "rb");
    if (entrada == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
        exit(1);
    }
    
    // Contar a frequência de cada byte
    U8 byte;
    while (fread(&byte, sizeof(U8), 1, entrada) == 1) {
        if (!inclua_byte(byte, &tab)) {
            printf("Erro ao incluir byte na tabela\n");
            exit(1);
        }
    }
    fclose(entrada);
    
    //Construir árvore de Huffman
    NoHuffman *raiz = NULL;
    construir_arvore_huffman(&tab, &raiz);
    
    //Gerar códigos para cada byte
    Codigo *tabela_codigos[256] = {NULL};
    Codigo codigo_atual;
    if (!novo_codigo(&codigo_atual)) {
        printf("Erro ao criar código\n");
        exit(1);
    }
    
    gerar_codigos(raiz, &codigo_atual, tabela_codigos);
    free_codigo(&codigo_atual);
    
    //Escrever arquivo compactado
    FILE *saida = fopen(nome_saida, "wb");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saída\n");
        exit(1);
    }
    
    // Escreve cabeçalho com as frequências para reconstrução da árvore
    for (U16 i = 0; i < 256; i++) {
        U64 freq = 0;
        if (tab.vetor[i] != NULL) {
            freq = tab.vetor[i]->informacao.frequencia;
        }
        fwrite(&freq, sizeof(U64), 1, saida);
    }
    
    //Compactar os dados
    entrada = fopen(nome_entrada, "rb");
    if (entrada == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
        exit(1);
    }
    
    Codigo buffer;
    if (!novo_codigo(&buffer)) {
        printf("Erro ao criar buffer\n");
        exit(1);
    }
    
    U8 byte_escrito = 0;
    int bits_escritos = 0;
    
    // Para cada byte do arquivo original, escreve seu código correspondente
    while (fread(&byte, sizeof(U8), 1, entrada)) {
        Codigo *codigo = tabela_codigos[byte];
        for (U8 i = 0; i < codigo->tamanho; i++) {
            int byte_index = i / 8;
            int bit_index = i % 8;
            U8 bit = (codigo->byte[byte_index] >> (7 - bit_index)) & 1;
            
            byte_escrito = (byte_escrito << 1) | bit;
            bits_escritos++;
            
            if (bits_escritos == 8) {
                fwrite(&byte_escrito, sizeof(U8), 1, saida);
                bits_escritos = 0;
                byte_escrito = 0;
            }
        }
    }
    
    // Escreve bits restantes não completos
    if (bits_escritos > 0) {
        byte_escrito <<= (8 - bits_escritos);
        fwrite(&byte_escrito, sizeof(U8), 1, saida);
    }
    
    // Limpeza final
    fclose(entrada);
    fclose(saida);
    free_codigo(&buffer);
    liberar_arvore(raiz);
    for (U16 i = 0; i < 256; i++) {
        if (tabela_codigos[i] != NULL) {
            free_codigo(tabela_codigos[i]);
            free(tabela_codigos[i]);
        }
    }
}

void descompactar_arquivo(const char *nome_entrada, const char *nome_saida) {
	
    //Ler frequências do cabeçalho
    FILE *entrada = fopen(nome_entrada, "rb");
    if (entrada == NULL) {
        printf("Erro ao abrir arquivo compactado\n");
        exit(1);
    }
    
    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);
    
    // Ler a tabela de frequências do arquivo compactado
    for (U16 i = 0; i < 256; i++) {
        U64 freq;
        if (fread(&freq, sizeof(U64), 1, entrada) != 1) {
            printf("Erro ao ler frequências\n");
            exit(1);
        }
        
        if (freq > 0) {
            Elemento elem;
            elem.byte = (U8)i;
            elem.frequencia = freq;
            
            if (!novo_no_de_arvore_binaria(&(tab.vetor[i]), NULL, elem, NULL)) {
                printf("Erro ao criar nó da árvore\n");
                exit(1);
            }
            tab.quantidade_de_posicoes_preenchidas++;
        }
    }
    
    //Reconstrução da árvore de Huffman
    NoHuffman *raiz = NULL;
    construir_arvore_huffman(&tab, &raiz);
    
    //Descompactação dos dados
    FILE *saida = fopen(nome_saida, "wb");
    if (saida == NULL) {
        printf("Erro ao criar arquivo de saída\n");
        exit(1);
    }
    
    NoHuffman *no_atual = raiz;
    U8 byte;
    int bit_pos = 7; // Começa pelo bit mais significativo
    
    // Navegação pela árvore até encontrar um byte original
    while (fread(&byte, sizeof(U8), 1, entrada) == 1) {
        while (bit_pos >= 0) {
            U8 bit = (byte >> bit_pos) & 1;
            bit_pos--;
            
            if (bit == 0) {
                no_atual = no_atual->esquerda;
            } else {
                no_atual = no_atual->direita;
            }
            
            // Quando chega em uma folha, escreve o byte original
            if (no_atual->esquerda == NULL && no_atual->direita == NULL) {
                fwrite(&(no_atual->byte), sizeof(U8), 1, saida);
                no_atual = raiz;
            }
        }
        bit_pos = 7; // Reinicia para o próximo byte
    }
    
    // Limpeza final
    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}
