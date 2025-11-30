//Nivel Mestre - Desafio Detective Quest

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 26  // Tamanho da tabela hash baseado na primeira letra (A-Z)

// Struct para representar um cômodo da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esq, *dir;
} Sala;

// Struct para representar uma pista na BST
typedef struct Pista {
    char nome[100];
    struct Pista *esq, *dir;
} Pista;

// Struct para lista encadeada de pistas associadas a um suspeito
typedef struct ListaPistas {
    char pista[100];
    struct ListaPistas *prox;
} ListaPistas;

// Struct para suspeito, com lista de pistas e contador
typedef struct Suspeito {
    char nome[50];
    ListaPistas *pistas;
    int contador;
    struct Suspeito *prox;
} Suspeito;

// Tabela hash: array de ponteiros para listas encadeadas de suspeitos
Suspeito *hash[TAM_HASH];

// Função para calcular o índice hash baseado na primeira letra do suspeito
int calcularHash(char *suspeito) {
    return toupper(suspeito[0]) - 'A';
}

// Função para inicializar a tabela hash
void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        hash[i] = NULL;
    }
}

// Função para criar um cômodo dinamicamente
Sala* criarSala(char nome[], char pista[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = nova->dir = NULL;
    return nova;
}

// Função para inserir uma pista na BST de pistas coletadas
void inserirPista(Pista **raiz, char pista[]) {
    if (*raiz == NULL) {
        *raiz = (Pista*) malloc(sizeof(Pista));
        strcpy((*raiz)->nome, pista);
        (*raiz)->esq = (*raiz)->dir = NULL;
    } else {
        if (strcmp(pista, (*raiz)->nome) < 0) {
            inserirPista(&((*raiz)->esq), pista);
        } else {
            inserirPista(&((*raiz)->dir), pista);
        }
    }
}

// Função para inserir associação pista-suspeito na tabela hash
void inserirNaHash(Suspeito *hash[], char pista[], char suspeito[]) {
    int indice = calcularHash(suspeito);
    Suspeito *atual = hash[indice];
    
    // Procurar se o suspeito já existe na lista
    while (atual != NULL) {
        if (strcmp(atual->nome, suspeito) == 0) {
            // Suspeito encontrado, adicionar pista à lista e incrementar contador
            ListaPistas *novaPista = (ListaPistas*) malloc(sizeof(ListaPistas));
            strcpy(novaPista->pista, pista);
            novaPista->prox = atual->pistas;
            atual->pistas = novaPista;
            atual->contador++;
            return;
        }
        atual = atual->prox;
    }
    
    // Suspeito não encontrado, criar novo
    Suspeito *novo = (Suspeito*) malloc(sizeof(Suspeito));
    strcpy(novo->nome, suspeito);
    novo->pistas = (ListaPistas*) malloc(sizeof(ListaPistas));
    strcpy(novo->pistas->pista, pista);
    novo->pistas->prox = NULL;
    novo->contador = 1;
    novo->prox = hash[indice];
    hash[indice] = novo;
}

// Função para consultar o suspeito correspondente a uma pista
char* encontrarSuspeito(Suspeito *hash[], char pista[]) {
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = hash[i];
        while (atual != NULL) {
            ListaPistas *p = atual->pistas;
            while (p != NULL) {
                if (strcmp(p->pista, pista) == 0) {
                    return atual->nome;
                }
                p = p->prox;
            }
            atual = atual->prox;
        }
    }
    return NULL;  // Pista não encontrada
}

// Função para listar todos os suspeitos e suas pistas
void listarAssociacoes(Suspeito *hash[]) {
    printf("\nAssociações Pista-Suspeito:\n");
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = hash[i];
        while (atual != NULL) {
            printf("Suspeito: %s (Contador: %d)\n", atual->nome, atual->contador);
            ListaPistas *p = atual->pistas;
            while (p != NULL) {
                printf("  - Pista: %s\n", p->pista);
                p = p->prox;
            }
            atual = atual->prox;
        }
    }
}

// Função para encontrar o suspeito mais provável (com maior contador)
char* suspeitoMaisProvavel(Suspeito *hash[]) {
    char *maisProvavel = NULL;
    int maxContador = 0;
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = hash[i];
        while (atual != NULL) {
            if (atual->contador > maxContador) {
                maxContador = atual->contador;
                maisProvavel = atual->nome;
            }
            atual = atual->prox;
        }
    }
    return maisProvavel;
}

// Função para imprimir pistas da BST em ordem
void imprimirPistas(Pista *raiz) {
    if (raiz != NULL) {
        imprimirPistas(raiz->esq);
        printf("- %s\n", raiz->nome);
        imprimirPistas(raiz->dir);
    }
}

// Função para navegar pela árvore de salas e coletar pistas
void explorarSalas(Sala *raiz, Pista **bstPistas, Suspeito *hash[]) {
    if (raiz == NULL) return;
    
    char opcao;
    printf("\nVocê está na sala: %s\n", raiz->nome);
    printf("Pista encontrada: %s\n", raiz->pista);
    
    // Inserir pista na BST e na hash (associar a suspeito baseado em lógica codificada)
    inserirPista(bstPistas, raiz->pista);
    
    // Lógica codificada para associar pistas a suspeitos (exemplo simples)
    if (strcmp(raiz->pista, "Pegada de sangue") == 0) {
        inserirNaHash(hash, raiz->pista, "Sr. Smith");
    } else if (strcmp(raiz->pista, "Carta suspeita") == 0) {
        inserirNaHash(hash, raiz->pista, "Sra. Johnson");
    } else if (strcmp(raiz->pista, "Relógio quebrado") == 0) {
        inserirNaHash(hash, raiz->pista, "Sr. Smith");
    } else if (strcmp(raiz->pista, "Chave misteriosa") == 0) {
        inserirNaHash(hash, raiz->pista, "Sra. Johnson");
    } else if (strcmp(raiz->pista, "Veneno na cozinha") == 0) {
        inserirNaHash(hash, raiz->pista, "Sr. Smith");
    }
    
    printf("Escolha: esquerda (e), direita (d) ou sair (s): ");
    scanf(" %c", &opcao);
    
    if (opcao == 'e') {
        explorarSalas(raiz->esq, bstPistas, hash);
    } else if (opcao == 'd') {
        explorarSalas(raiz->dir, bstPistas, hash);
    } else if (opcao == 's') {
        return;
    } else {
        printf("Opção inválida. Tente novamente.\n");
        explorarSalas(raiz, bstPistas, hash);
    }
}

// Função para a fase de julgamento final
void verificarSuspeitoFinal(Suspeito *hash[], Pista *bstPistas) {
    printf("\nPistas coletadas:\n");
    imprimirPistas(bstPistas);
    
    listarAssociacoes(hash);
    
    char suspeitoAcusado[50];
    printf("\nQuem você acusa? ");
    scanf("%s", suspeitoAcusado);
    
    // Verificar contador do suspeito acusado
    int indice = calcularHash(suspeitoAcusado);
    Suspeito *atual = hash[indice];
    while (atual != NULL) {
        if (strcmp(atual->nome, suspeitoAcusado) == 0) {
            if (atual->contador >= 2) {
                printf("Parabéns! Você acertou. %s é o culpado com %d pistas.\n", suspeitoAcusado, atual->contador);
            } else {
                printf("Acusação incorreta. Não há pistas suficientes contra %s.\n", suspeitoAcusado);
            }
            break;
        }
        atual = atual->prox;
    }
    if (atual == NULL) {
        printf("Suspeito não encontrado.\n");
    }
    
    char *maisProvavel = suspeitoMaisProvavel(hash);
    if (maisProvavel != NULL) {
        printf("Suspeito mais provável baseado nas pistas: %s\n", maisProvavel);
    }
}

int main() {
    // Inicializar estruturas
    Pista *bstPistas = NULL;
    inicializarHash();
    
    // Montar árvore de salas manualmente (mapa fixo)
    Sala *raiz = criarSala("Hall de Entrada", "Pegada de sangue");
    raiz->esq = criarSala("Sala de Estar", "Carta suspeita");
    raiz->dir = criarSala("Escritório", "Relógio quebrado");
    raiz->esq->esq = criarSala("Cozinha", "Veneno na cozinha");
    raiz->esq->dir = criarSala("Quarto Principal", "Chave misteriosa");
    
    // Iniciar exploração
    printf("Bem-vindo ao Detective Quest!\n");
    explorarSalas(raiz, &bstPistas, hash);
    
    // Fase final
    verificarSuspeitoFinal(hash, bstPistas);
    
    // Liberar memória (simplificado, não implementado completamente para brevidade)
    return 0;
}
