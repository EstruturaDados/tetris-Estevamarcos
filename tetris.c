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
    char nome; 
    int id;    
} Peca;

typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int totalElementos;
} FilaCircular;

typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;
} PilhaLinear;

/* ============================================================================
 2. PROTOTIPOS DAS FUNCOES
 ============================================================================
 */
void inicializarFila(FilaCircular* fila);
void inicializarPilha(PilhaLinear* pilha);
void gerarPeca(Peca* novaPeca, int* contadorId);
void enqueue(FilaCircular* fila, int* contadorId);
Peca dequeue(FilaCircular* fila);
void push(PilhaLinear* pilha, Peca peca);
Peca pop(PilhaLinear* pilha);
void trocarPecaAtual(FilaCircular* fila, PilhaLinear* pilha);
void trocaMultipla(FilaCircular* fila, PilhaLinear* pilha);
void exibirEstadoAtual(FilaCircular* fila, PilhaLinear* pilha);
void reservarPeca(FilaCircular* fila, PilhaLinear* pilha, int* contadorId);

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

    for (i = 0; i < CAPACIDADE_FILA; i++) {
        Peca pecaInicial;
        gerarPeca(&pecaInicial, &contadorGlobalId);
        filaFuture.itens[filaFuture.tras] = pecaInicial;
        filaFuture.tras = (filaFuture.tras + 1) % CAPACIDADE_FILA;
        filaFuture.totalElementos++;
    }

    while (1) {
        exibirEstadoAtual(&filaFuture, &pilhaReserva);

        printf("Opcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("--------------------------------------------------\n");
        printf("Opcao escolhida: ");
        
        if (scanf("%d", &opcao) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }
        int ch; while ((ch = getchar()) != '\n' && ch != EOF);

        if (opcao == 0) break;

        switch (opcao) {
            case 1:
                if (filaFuture.totalElementos == 0) {
                    printf("\n[ALERTA] Fila vazia!\n");
                } else {
                    Peca jogada = dequeue(&filaFuture);
                    printf("\nVoce jogou a peca [%c %d]!\n", jogada.nome, jogada.id);
                    enqueue(&filaFuture, &contadorGlobalId);
                }
                printf("Pressione Enter para continuar..."); getchar();
                break;
            case 2:
                reservarPeca(&filaFuture, &pilhaReserva, &contadorGlobalId);
                break;
            case 3:
                if (pilhaReserva.topo == -1) {
                    printf("\n[ALERTA] Pilha de reserva vazia!\n");
                } else {
                    Peca usada = pop(&pilhaReserva);
                    printf("\nVoce usou a peca reservada [%c %d]!\n", usada.nome, usada.id);
                }
                printf("Pressione Enter para continuar..."); getchar();
                break;
            case 4:
                trocarPecaAtual(&filaFuture, &pilhaReserva);
                break;
            case 5:
                trocaMultipla(&filaFuture, &pilhaReserva);
                break;
            default:
                printf("\nOpcao invalida!\n");
                printf("Pressione Enter para continuar..."); getchar();
                break;
        }
        printf("\n"); 
    }
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

void inicializarPilha(PilhaLinear* pilha) {
    pilha->topo = -1;
}

void gerarPeca(Peca* novaPeca, int* contadorId) {
    char formatos[] = {'I', 'O', 'T', 'L'};
    novaPeca->nome = formatos[rand() % 4];
    novaPeca->id = *contadorId;
    (*contadorId)++;
}

void enqueue(FilaCircular* fila, int* contadorId) {
    if (fila->totalElementos >= CAPACIDADE_FILA) return;
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

void push(PilhaLinear* pilha, Peca peca) {
    if (pilha->topo >= CAPACIDADE_PILHA - 1) return;
    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
}

Peca pop(PilhaLinear* pilha) {
    Peca pecaRemovida = pilha->itens[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

void trocarPecaAtual(FilaCircular* fila, PilhaLinear* pilha) {
    if (fila->totalElementos == 0 || pilha->topo == -1) {
        printf("\n[ALERTA] Estruturas vazias.\n");
        printf("Pressione Enter para continuar..."); getchar();
        return;
    }
    Peca temp = fila->itens[fila->frente];
    fila->itens[fila->frente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;
    printf("\nAcao: Troca realizada com sucesso!\n");
    printf("Pressione Enter para continuar..."); getchar();
}

void trocaMultipla(FilaCircular* fila, PilhaLinear* pilha) {
    if (fila->totalElementos < 3 || pilha->topo < 2) {
        printf("\n[ALERTA] Requer ao menos 3 elementos em cada estrutura!\n");
        printf("Pressione Enter para continuar..."); getchar();
        return;
    }
    Peca pecasFila[3]; Peca pecasPilha[3]; int i;
    for (i = 0; i < 3; i++) pecasFila[i] = fila->itens[(fila->frente + i) % CAPACIDADE_FILA];
    for (i = 0; i < 3; i++) pecasPilha[i] = pilha->itens[pilha->topo - i];
    for (i = 0; i < 3; i++) fila->itens[(fila->frente + i) % CAPACIDADE_FILA] = pecasPilha[i];
    for (i = 0; i < 3; i++) pilha->itens[pilha->topo - i] = pecasFila[i];
    printf("\nAcao: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
    printf("Pressione Enter para continuar..."); getchar();
}

void reservarPeca(FilaCircular* fila, PilhaLinear* pilha, int* contadorId) {
    if (pilha->topo >= CAPACIDADE_PILHA - 1 || fila->totalElementos == 0) {
        printf("\n[ALERTA] Operacao invalida (Limite atingido ou Fila vazia)!\n");
        printf("Pressione Enter para continuar..."); getchar();
        return;
    }
    Peca pecaParaReserva = dequeue(fila);
    push(pilha, pecaParaReserva);
    enqueue(fila, contadorId);
    printf("\nPeca [%c %d] movida para a reserva!\n", pecaParaReserva.nome, pecaParaReserva.id);
    printf("Pressione Enter para continuar..."); getchar();
}

void exibirEstadoAtual(FilaCircular* fila, PilhaLinear* pilha) {
    printf("\nEstado atual:\nFila de pecas: ");
    if (fila->totalElementos == 0) {
        printf("[Vazia]");
    } else {
        int i;
        for (i = 0; i < fila->totalElementos; i++) {
            printf("[%c %d] ", fila->itens[(fila->frente + i) % CAPACIDADE_FILA].nome, fila->itens[(fila->frente + i) % CAPACIDADE_FILA].id);
        }
    }
    printf("\nPilha de reserva (Topo -> base): ");
    if (pilha->topo == -1) {
        printf("[Vazia]");
    } else {
        int i;
        for (i = pilha->topo; i >= 0; i--) printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
    }
    printf("\n--------------------------------------------------\n\n");
}


