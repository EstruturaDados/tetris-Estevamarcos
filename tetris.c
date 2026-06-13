#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

/* ============================================================================
 1. DECLARACAO DAS STRUCTS
 ============================================================================
 */
typedef struct {
    char nome; // Caractere que representa o tipo ('I', 'O', 'T', 'L')
    int id;    // Numero inteiro unico de criacao
} Peca;

// Estrutura da Fila Circular (Proximas Pecas)
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int totalElementos;
} FilaCircular;

// Estrutura da Pilha Linear (Pecas Reservadas)
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; // Indica o indice do elemento no topo da pilha
} PilhaLinear;

/* ============================================================================
 2. PROTOTIPOS DAS FUNCOES (Modularizacao)
 ============================================================================
 */
void inicializarFila(FilaCircular* fila);
void inicializarPilha(PilhaLinear* pilha);
void gerarPeca(Peca* novaPeca, int* contadorId);

// Operacoes da Fila
void enqueue(FilaCircular* fila, int* contadorId);
Peca dequeue(FilaCircular* fila);

// Operacoes da Pilha
void push(PilhaLinear* pilha, Peca peca);
Peca pop(PilhaLinear* pilha);

// Interface e Fluxos
void exibirEstadoAtual(FilaCircular* fila, PilhaLinear* pilha);
void reservarPeca(FilaCircular* fila, PilhaLinear* pilha, int* contadorId);
void limparBuffer();

/* ============================================================================
 3. FUNCAO PRINCIPAL
 ============================================================================
 */
int main() {
    FilaCircular filaFuture;
    PilhaLinear pilhaReserva;
    int contadorGlobalId = 0; 
    int opcao = -1;
    int i;

    srand(time(NULL));
    inicializarFila(&filaFuture);
    inicializarPilha(&pilhaReserva);

    // REQUISITO: Inicializa a fila de pecas preenchida com 5 elementos
    for (i = 0; i < CAPACIDADE_FILA; i++) {
        Peca pecaInicial;
        gerarPeca(&pecaInicial, &contadorGlobalId);
        filaFuture.itens[filaFuture.tras] = pecaInicial;
        filaFuture.tras = (filaFuture.tras + 1) % CAPACIDADE_FILA;
        filaFuture.totalElementos++;
    }

    // Loop do Menu Interativo - Tetris Stack Intermediario
    while (1) {
        // REQUISITO: Exibir o estado atual (Fila e Pilha) apos cada acao
        exibirEstadoAtual(&filaFuture, &pilhaReserva);

        // Menu estruturado conforme as tabelas do Exemplo de Saida
        printf("Opcoes de Acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("--------------------------------------------------\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 0) {
            break;
        }

        switch (opcao) {
            case 1: {
                if (filaFuture.totalElementos == 0) {
                    printf("\n[ALERTA] Nao ha pecas na fila!\n");
                } else {
                    Peca jogada = dequeue(&filaFuture);
                    printf("\nVoce jogou a peca [%c %d] com sucesso!\n", jogada.nome, jogada.id);
                    // Completa a fila automaticamente com uma nova peca
                    enqueue(&filaFuture, &contadorGlobalId);
                }
                printf("Pressione Enter para continuar...");
                getchar();
                break;
            }
            case 2:
                reservarPeca(&filaFuture, &pilhaReserva, &contadorGlobalId);
                break;
                
            case 3: {
                if (pilhaReserva.topo == -1) {
                    printf("\n[ALERTA] A pilha de reserva está vazia! Nao ha pecas para usar.\n");
                } else {
                    Peca usada = pop(&pilhaReserva);
                    printf("\nVoce usou a peca reservada [%c %d]!\n", usada.nome, usada.id);
                }
                printf("Pressione Enter para continuar...");
                getchar();
                break;
            }
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("Pressione Enter para continuar...");
                getchar();
                break;
        }
        printf("\n"); 
    }

    printf("\nEncerrando o Tetris Stack. Sistema ByteBros finalizado com sucesso!\n");
    return 0;
}

/* ============================================================================
 4. IMPLEMENTACAO DAS FUNCOES DE INICIALIZACAO E GERACAO
 ============================================================================
 */
void inicializarFila(FilaCircular* fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->totalElementos = 0;
}

void inicializarPilha(PilhaLinear* pilha) {
    pilha->topo = -1; // -1 indica que a pilha comeca vazia
}

void gerarPeca(Peca* novaPeca, int* contadorId) {
    char formatos[] = {'I', 'O', 'T', 'L'};
    int indiceAleatorio = rand() % 4;

    novaPeca->nome = formatos[indiceAleatorio];
    novaPeca->id = *contadorId;
    
    (*contadorId)++;
}

/* ============================================================================
 5. OPERACOES DA FILA CIRCULAR
 ============================================================================
 */
void enqueue(FilaCircular* fila, int* contadorId) {
    if (fila->totalElementos >= CAPACIDADE_FILA) {
        return;
    }
    Peca novaPeca;
    gerarPeca(&novaPeca, contadorId);

    fila->itens[fila->tras] = novaPeca;
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    fila->totalElementos++;
}

Peca dequeue(FilaCircular* fila) {
    Peca pecaRemovida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->totalElementos--;
    return pecaRemovida;
}

/* ============================================================================
 6. OPERACOES DA PILHA LINEAR
 ============================================================================
 */
void push(PilhaLinear* pilha, Peca peca) {
    if (pilha->topo >= CAPACIDADE_PILHA - 1) {
        return;
    }
    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
}

Peca pop(PilhaLinear* pilha) {
    Peca pecaRemovida = pilha->itens[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

/* ============================================================================
 7. FLUXOS E INTERFACE DO JOGO (Ajustado estritamente ao Exemplo de Saida)
 ============================================================================
 */

// Move a peca da frente da fila para o topo da pilha de reserva
void reservarPeca(FilaCircular* fila, PilhaLinear* pilha, int* contadorId) {
    if (pilha->topo >= CAPACIDADE_PILHA - 1) {
        printf("\n[ALERTA] A pilha de reserva ja esta cheia (Limite: %d)!\n", CAPACIDADE_PILHA);
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }
    if (fila->totalElementos == 0) {
        printf("\n[ALERTA] Nao ha pecas na fila para reservar!\n");
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }

    // Move da frente da fila para o topo da pilha
    Peca pecaParaReserva = dequeue(fila);
    push(pilha, pecaParaReserva);

    // REQUISITO: A cada acao, uma nova peca e adicionada automaticamente ao final da fila
    enqueue(fila, contadorId);

    printf("\nPeca [%c %d] movida para a reserva com sucesso!\n", pecaParaReserva.nome, pecaParaReserva.id);
    printf("Pressione Enter para continuar...");
    getchar();
}

void exibirEstadoAtual(FilaCircular* fila, PilhaLinear* pilha) {
    printf("\nEstado atual:\n");
    
    // 1. Exibe a Fila
    printf("Fila de pecas: ");
    if (fila->totalElementos == 0) {
        printf("[Vazia]");
    } else {
        int i;
        for (i = 0; i < fila->totalElementos; i++) {
            int indiceAtual = (fila->frente + i) % CAPACIDADE_FILA;
            printf("[%c %d] ", fila->itens[indiceAtual].nome, fila->itens[indiceAtual].id);
        }
    }
    
    // 2. Exibe a Pilha (Do Topo para a Base conforme exigido)
    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilha->topo == -1) {
        printf("[Vazia]");
    } else {
        int i;
        for (i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome,  pilha->itens[i].id);
        }
    }
    printf("\n--------------------------------------------------\n\n");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


