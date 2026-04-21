#ifndef FILACI_H
#define FILACI_H
// Enum que define o tipo de dado armazenado na fila

// Pode ser um inteiro ou um caractere
typedef enum {
    TIPO_INT,   // representa números inteiros
    TIPO_CHAR   // representa caracteres (ex: operadores)
} TipoDado;
// Estrutura que representa um elemento da fila
typedef struct {
    TipoDado tipo;     // indica se o elemento é int ou char
    int valor_int;     // armazena o valor inteiro (se tipo for TIPO_INT)
    char valor_char;   // armazena o caractere (se tipo for TIPO_CHAR)
} Elemento;
// Estrutura da fila circular com tipos mistos (int e char)
typedef struct {
    int inicio;        // índice do início da fila
    int fim;           // índice do fim da fila
    int qtdAtual;      // quantidade atual de elementos na fila
    int capacidade;    // capacidade máxima da fila
    Elemento *elem;    // ponteiro para o vetor de elementos
} FilaCI;
// Função para criar uma fila com capacidade definida
// Retorna um ponteiro para a fila criada
FilaCI* criarFilaCI(int qtd);
// Verifica se a fila está cheia
// Retorna 1 (true) se estiver cheia, 0 (false) caso contrário
int filaCheiaCI(FilaCI *fila);
// Insere um elemento na fila (enfileirar)
// tipo: define se é int ou char
// num: valor inteiro (usado se tipo = TIPO_INT)
// op: valor char (usado se tipo = TIPO_CHAR)
// Retorna 1 se inseriu com sucesso, 0 se falhou
int enfileirarCI(FilaCI *fila, TipoDado tipo, int num, char op);
// Verifica se a fila está vazia
// Retorna 1 (true) se estiver vazia, 0 (false) caso contrário
int filaVaziaCI(FilaCI *fila);
// Remove um elemento da fila (desenfileirar)
// Retorna o elemento removido
Elemento desenfileirarCI(FilaCI *fila);
// Libera a memória ocupada pela fila
// Retorna NULL após liberar (boa prática para evitar ponteiro solto)
FilaCI* liberarMemoriaFilaCI(FilaCI *fila);
#endif
