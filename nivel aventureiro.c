#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

// =====================================================
// STRUCTS (obrigatórias)
// =====================================================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// =====================================================
// FUNÇÕES AUXILIARES DE STRING / INPUT
// =====================================================
void tirarQuebraLinha(char* s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

void lerLinha(const char* msg, char* buffer, int tam) {
    printf("%s", msg);
    fgets(buffer, tam, stdin);
    tirarQuebraLinha(buffer);
}

int lerInt(const char* msg) {
    int x;
    printf("%s", msg);
    while (scanf("%d", &x) != 1) {
        printf("Entrada invalida. Digite um numero.\n");
        while (getchar() != '\n') {}
        printf("%s", msg);
    }
    while (getchar() != '\n') {} // limpa \n
    return x;
}

// =====================================================
// ==================== VETOR ===========================
// =====================================================

void listarVetor(Item v[], int qtd) {
    printf("\n===== MOCHILA (VETOR) =====\n");
    if (qtd == 0) {
        printf("Mochila vazia.\n");
    } else {
        for (int i = 0; i < qtd; i++) {
            printf("[%d] Nome: %s | Tipo: %s | Qtd: %d\n",
                   i, v[i].nome, v[i].tipo, v[i].quantidade);
        }
    }
    printf("===========================\n\n");
}

// Inserção no vetor
void inserirItemVetor(Item v[], int* qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("Mochila cheia (max %d itens).\n", MAX_ITENS);
        return;
    }

    Item novo;
    lerLinha("Nome do item: ", novo.nome, sizeof(novo.nome));
    lerLinha("Tipo (arma/municao/cura/ferramenta): ", novo.tipo, sizeof(novo.tipo));
    novo.quantidade = lerInt("Quantidade: ");

    v[*qtd] = novo;
    (*qtd)++;

    printf("Item inserido no vetor! ✅\n");
}

// Busca sequencial no vetor (conta comparações)
int buscarSequencialVetor(Item v[], int qtd, const char* nome, int* comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < qtd; i++) {
        (*comparacoes)++;
        if (strcmp(v[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Remove no vetor por nome (shift)
void removerItemVetor(Item v[], int* qtd) {
    if (*qtd == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    char nome[30];
    lerLinha("Digite o nome para remover: ", nome, sizeof(nome));

    int comps = 0;
    int idx = buscarSequencialVetor(v, *qtd, nome, &comps);

    if (idx == -1) {
        printf("Item nao encontrado. (comparacoes: %d)\n", comps);
        return;
    }

    for (int i = idx; i < *qtd - 1; i++) {
        v[i] = v[i + 1];
    }
    (*qtd)--;

    printf("Item removido do vetor! ✅ (comparacoes na busca: %d)\n", comps);
}

// Ordenação por nome (Bubble Sort)
void ordenarVetor(Item v[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - 1 - i; j++) {
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                Item temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
    printf("Vetor ordenado por nome! ✅\n");
}

// Busca binária no vetor (conta comparações)
// IMPORTANTE: vetor precisa estar ordenado por nome
int buscarBinariaVetor(Item v[], int qtd, const char* nome, int* comparacoes) {
    int ini = 0;
    int fim = qtd - 1;
    *comparacoes = 0;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;

        (*comparacoes)++;
        int cmp = strcmp(nome, v[meio].nome);

        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else ini = meio + 1;
    }
    return -1;
}

// =====================================================
// ================= LISTA ENCADEADA ====================
// =====================================================

void listarLista(No* inicio) {
    printf("\n===== MOCHILA (LISTA) =====\n");
    if (inicio == NULL) {
        printf("Mochila vazia.\n");
    } else {
        int i = 0;
        No* atual = inicio;
        while (atual != NULL) {
            printf("[%d] Nome: %s | Tipo: %s | Qtd: %d\n",
                   i, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            atual = atual->proximo;
            i++;
        }
    }
    printf("===========================\n\n");
}

No* inserirItemLista(No* inicio) {
    Item novo;
    lerLinha("Nome do item: ", novo.nome, sizeof(novo.nome));
    lerLinha("Tipo (arma/municao/cura/ferramenta): ", novo.tipo, sizeof(novo.tipo));
    novo.quantidade = lerInt("Quantidade: ");

    No* noNovo = (No*)malloc(sizeof(No));
    if (!noNovo) {
        printf("Erro de memoria.\n");
        return inicio;
    }

    noNovo->dados = novo;
    noNovo->proximo = inicio; // insere no começo (mais simples)
    printf("Item inserido na lista! ✅\n");
    return noNovo;
}

// Busca sequencial na lista (conta comparações)
No* buscarSequencialLista(No* inicio, const char* nome, int* comparacoes) {
    *comparacoes = 0;
    No* atual = inicio;

    while (atual != NULL) {
        (*comparacoes)++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

No* removerItemLista(No* inicio) {
    if (inicio == NULL) {
        printf("Mochila vazia.\n");
        return inicio;
    }

    char nome[30];
    lerLinha("Digite o nome para remover: ", nome, sizeof(nome));

    No* atual = inicio;
    No* anterior = NULL;
    int comparacoes = 0;

    while (atual != NULL) {
        comparacoes++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL) {
                // remove o primeiro
                inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Item removido da lista! ✅ (comparacoes na busca: %d)\n", comparacoes);
            return inicio;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Item nao encontrado. (comparacoes: %d)\n", comparacoes);
    return inicio;
}

void liberarLista(No* inicio) {
    No* atual = inicio;
    while (atual != NULL) {
        No* prox = atual->proximo;
        free(atual);
        atual = prox;
    }
}

// =====================================================
// ===================== MENUS ===========================
// =====================================================

void menuVetor(Item v[], int* qtd) {
    int op;
    do {
        printf("\n--- MENU VETOR ---\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item (por nome)\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item (sequencial)\n");
        printf("5 - Ordenar vetor por nome\n");
        printf("6 - Buscar item (binaria - precisa estar ordenado)\n");
        printf("0 - Voltar\n");
        op = lerInt("Escolha: ");

        if (op == 1) {
            inserirItemVetor(v, qtd);
            listarVetor(v, *qtd);
        } else if (op == 2) {
            removerItemVetor(v, qtd);
            listarVetor(v, *qtd);
        } else if (op == 3) {
            listarVetor(v, *qtd);
        } else if (op == 4) {
            char nome[30];
            lerLinha("Nome para buscar (sequencial): ", nome, sizeof(nome));
            int comps = 0;
            int idx = buscarSequencialVetor(v, *qtd, nome, &comps);
            if (idx == -1) {
                printf("Nao encontrado. Comparacoes: %d\n", comps);
            } else {
                printf("Encontrado no indice %d. Comparacoes: %d\n", idx, comps);
                printf("Nome: %s | Tipo: %s | Qtd: %d\n", v[idx].nome, v[idx].tipo, v[idx].quantidade);
            }
        } else if (op == 5) {
            ordenarVetor(v, *qtd);
            listarVetor(v, *qtd);
        } else if (op == 6) {
            char nome[30];
            lerLinha("Nome para buscar (binaria): ", nome, sizeof(nome));
            int comps = 0;
            int idx = buscarBinariaVetor(v, *qtd, nome, &comps);
            if (idx == -1) {
                printf("Nao encontrado. Comparacoes: %d\n", comps);
            } else {
                printf("Encontrado no indice %d. Comparacoes: %d\n", idx, comps);
                printf("Nome: %s | Tipo: %s | Qtd: %d\n", v[idx].nome, v[idx].tipo, v[idx].quantidade);
            }
        }

    } while (op != 0);
}

void menuLista(No** inicio) {
    int op;
    do {
        printf("\n--- MENU LISTA ENCADEADA ---\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item (por nome)\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item (sequencial)\n");
        printf("0 - Voltar\n");
        op = lerInt("Escolha: ");

        if (op == 1) {
            *inicio = inserirItemLista(*inicio);
            listarLista(*inicio);
        } else if (op == 2) {
            *inicio = removerItemLista(*inicio);
            listarLista(*inicio);
        } else if (op == 3) {
            listarLista(*inicio);
        } else if (op == 4) {
            char nome[30];
            lerLinha("Nome para buscar (sequencial): ", nome, sizeof(nome));
            int comps = 0;
            No* achou = buscarSequencialLista(*inicio, nome, &comps);
            if (!achou) {
                printf("Nao encontrado. Comparacoes: %d\n", comps);
            } else {
                printf("Encontrado! Comparacoes: %d\n", comps);
                printf("Nome: %s | Tipo: %s | Qtd: %d\n",
                       achou->dados.nome, achou->dados.tipo, achou->dados.quantidade);
            }
        }

    } while (op != 0);
}

// =====================================================
// ===================== MAIN ============================
// =====================================================
int main() {
    Item mochilaVetor[MAX_ITENS];
    int qtdVetor = 0;

    No* mochilaLista = NULL;

    int op;
    do {
        printf("\n=== MOCHILA - NIVEL AVENTUREIRO ===\n");
        printf("1 - Usar Mochila com VETOR\n");
        printf("2 - Usar Mochila com LISTA ENCADEADA\n");
        printf("0 - Sair\n");
        op = lerInt("Escolha: ");

        if (op == 1) {
            menuVetor(mochilaVetor, &qtdVetor);
        } else if (op == 2) {
            menuLista(&mochilaLista);
        }

    } while (op != 0);

    liberarLista(mochilaLista);
    printf("Programa encerrado. 🏝️\n");
    return 0;
}