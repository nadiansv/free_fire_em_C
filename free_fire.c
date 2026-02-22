#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// -----------------------------
// Struct Item (exigida no enunciado)
// -----------------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// -----------------------------
// Funções auxiliares
// -----------------------------

// Remove o '\n' que o fgets coloca no final (se existir)
void tirarQuebraLinha(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

// Busca sequencial pelo nome
// Retorna o índice do item encontrado, ou -1 se não encontrar
int buscarItem(Item mochila[], int qtd, const char *nomeBuscado) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(mochila[i].nome, nomeBuscado) == 0) {
            return i;
        }
    }
    return -1;
}

// Lista todos os itens cadastrados
void listarItens(Item mochila[], int qtd) {
    printf("\n========== INVENTARIO ==========\n");
    if (qtd == 0) {
        printf("Mochila vazia.\n");
    } else {
        for (int i = 0; i < qtd; i++) {
            printf("[%d] Nome: %s | Tipo: %s | Quantidade: %d\n",
                   i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        }
    }
    printf("================================\n\n");
}

// Insere um item (se houver espaço)
void inserirItem(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("Mochila cheia! (max: %d itens)\n", MAX_ITENS);
        return;
    }

    Item novo;

    printf("Digite o NOME do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    tirarQuebraLinha(novo.nome);

    printf("Digite o TIPO (arma/municao/cura/ferramenta): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    tirarQuebraLinha(novo.tipo);

    printf("Digite a QUANTIDADE: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Quantidade invalida.\n");
        // limpa entrada ruim
        while (getchar() != '\n') {}
        return;
    }
    // consome o '\n' deixado pelo scanf
    while (getchar() != '\n') {}

    // (Opcional, mas útil) Se item com mesmo nome existir, soma quantidades
    int idx = buscarItem(mochila, *qtd, novo.nome);
    if (idx != -1) {
        mochila[idx].quantidade += novo.quantidade;
        printf("Item ja existia. Quantidade somada! ✅\n");
    } else {
        mochila[*qtd] = novo;
        (*qtd)++;
        printf("Item cadastrado com sucesso! ✅\n");
    }
}

// Remove um item pelo nome (shift no vetor)
void removerItem(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("Nao tem nada para remover. Mochila vazia.\n");
        return;
    }

    char nome[30];
    printf("Digite o NOME do item para remover: ");
    fgets(nome, sizeof(nome), stdin);
    tirarQuebraLinha(nome);

    int idx = buscarItem(mochila, *qtd, nome);
    if (idx == -1) {
        printf("Item '%s' nao encontrado.\n", nome);
        return;
    }

    // Move os elementos à esquerda para "tapar o buraco"
    for (int i = idx; i < *qtd - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*qtd)--;

    printf("Item removido com sucesso! ✅\n");
}

// Mostra dados de um item buscado
void mostrarItem(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("Mochila vazia. Nada para buscar.\n");
        return;
    }

    char nome[30];
    printf("Digite o NOME do item para buscar: ");
    fgets(nome, sizeof(nome), stdin);
    tirarQuebraLinha(nome);

    int idx = buscarItem(mochila, qtd, nome);
    if (idx == -1) {
        printf("Item '%s' nao encontrado.\n", nome);
        return;
    }

    printf("\n🎯 Item encontrado!\n");
    printf("Nome: %s\n", mochila[idx].nome);
    printf("Tipo: %s\n", mochila[idx].tipo);
    printf("Quantidade: %d\n\n", mochila[idx].quantidade);
}

// -----------------------------
// Programa principal (menu)
// -----------------------------
int main() {
    Item mochila[MAX_ITENS];
    int qtd = 0;

    int opcao;

    do {
        printf("=== SISTEMA DE INVENTARIO (NOVATO) ===\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item (busca sequencial)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            while (getchar() != '\n') {}
            continue;
        }
        while (getchar() != '\n') {} // limpa '\n' do buffer

        switch (opcao) {
            case 1:
                inserirItem(mochila, &qtd);
                listarItens(mochila, qtd); // listar após operação (exigido)
                break;

            case 2:
                removerItem(mochila, &qtd);
                listarItens(mochila, qtd); // listar após operação (exigido)
                break;

            case 3:
                listarItens(mochila, qtd);
                break;

            case 4:
                mostrarItem(mochila, qtd);
                listarItens(mochila, qtd); // pode listar após operação também
                break;

            case 0:
                printf("Saindo... Boa sobrevivencia na ilha! 🏝️\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}