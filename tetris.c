#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CAPACIDADE_FILA 5

/* ============================================================================
 1. DECLARACAO DAS STRUCTS
 ============================================================================
 */
typedef struct {
    char nome; // Caractere que representa o tipo ('I', 'O', 'T', 'L')
    int id;    // Numero inteiro unico de criacao
} Peca;

typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int totalElementos;
} FilaCircular;

/* ============================================================================
 2. PROTOTIPOS DAS FUNCOES
 ============================================================================
 */
void inicializarFila(FilaCircular* fila);
void gerarPeca(Peca* novaPeca, int* contadorId);
void enqueue(FilaCircular* fila, int* contadorId);
void dequeue(FilaCircular* fila);
void exibirFila(FilaCircular* fila);
void limparBuffer();

/* ============================================================================
 3. FUNCAO PRINCIPAL
 ============================================================================
 */
int main() {
    FilaCircular filaFuture;
    int contadorGlobalId = 0; 
    int opcao = -1;
    int i;

    srand(time(NULL));
    inicializarFila(&filaFuture);

    // Inicializa a fila de pecas preenchida com 5 elementos
    for (i = 0; i < CAPACIDADE_FILA; i++) {
        Peca pecaInicial;
        gerarPeca(&pecaInicial, &contadorGlobalId);
        filaFuture.itens[filaFuture.tras] = pecaInicial;
        filaFuture.tras = (filaFuture.tras + 1) % CAPACIDADE_FILA;
        filaFuture.totalElementos++;
    }

    // Loop do Menu Interativo do Tetris Stack
    while (1) {
        exibirFila(&filaFuture);

        printf("Opcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("--------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 0) {
            break;
        }

        switch (opcao) {
            case 1:
                dequeue(&filaFuture);
                break;
            case 2:
                enqueue(&filaFuture, &contadorGlobalId);
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("Pressione Enter para continuar...");
                getchar();
                break;
        }
        printf("\n"); 
    }

    printf("\nEncerrando o sistema de controle da ByteBros. Ate a proxima!\n");
    return 0;
}

/* ============================================================================
 4. IMPLEMENTACAO DAS FUNCOES
 ============================================================================
 */

void inicializarFila(FilaCircular* fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->totalElementos = 0;
}

void gerarPeca(Peca* novaPeca, int* contadorId) {
    char formatos[] = {'I', 'O', 'T', 'L'};
    int indiceAleatorio = rand() % 4;

    novaPeca->nome = formatos[indiceAleatorio];
    novaPeca->id = *contadorId;
    
    (*contadorId)++;
}

void enqueue(FilaCircular* fila, int* contadorId) {
    if (fila->totalElementos >= CAPACIDADE_FILA) {
        printf("\n[ALERTA] A fila de pecas futuras ja esta cheia (Limite: %d)!\n", CAPACIDADE_FILA);
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    Peca novaPeca;
    gerarPeca(&novaPeca, contadorId);

    fila->itens[fila->tras] = novaPeca;
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    fila->totalElementos++;

    printf("\nNova peca [%c %d] adicionada ao final da fila com sucesso!\n", novaPeca.nome, novaPeca.id);
}

void dequeue(FilaCircular* fila) {
    if (fila->totalElementos == 0) {
        printf("\n[ALERTA] Nao ha nenhuma peca na fila para ser jogada!\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    Peca pecaJogada = fila->itens[fila->frente];
    
    // LINHA 143 CORRIGIDA: Conta terminada e fechada perfeitamente
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->totalElementos--;

    printf("\nVoce jogou a peca [%c %d] com sucesso!\n", pecaJogada.nome, pecaJogada.id);
}

void exibirFila(FilaCircular* fila) {
    printf("\n==================================================\n");
    printf("Fila de pecas: ");
    
    if (fila->totalElementos == 0) {
        printf("[ Fila Vazia ]");
    } else {
        int i;
        for (i = 0; i < fila->totalElementos; i++) {
            int indiceAtual = (fila->frente + i) % CAPACIDADE_FILA;
            printf("[%c %d] ", fila->itens[indiceAtual].nome, fila->itens[indiceAtual].id);
        }
    }
    printf("\n==================================================\n\n");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

