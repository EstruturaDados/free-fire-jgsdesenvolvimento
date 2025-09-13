#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

// Enum para critérios de ordenação
typedef enum {
    NOME,
    TIPO,
    PRIORIDADE
} CriterioOrdenacao;

// Struct Item
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int prioridade; // 1 a 5
} Item;

// Struct para Nó da lista encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// Protótipos das funções
void limparTela();
void exibirMenu(bool ordenadaPorNome);
void inserirItem(Item mochila[], int *numItens, bool *ordenadaPorNome);
void removerItem(Item mochila[], int *numItens, bool *ordenadaPorNome);
void listarItens(Item mochila[], int numItens);
void menuDeOrdenacao(Item mochila[], int numItens, bool *ordenadaPorNome, int *comparacoes);
void insertionSort(Item mochila[], int numItens, CriterioOrdenacao criterio, int *comparacoes);
void buscaBinariaPorNome(Item mochila[], int numItens, bool ordenadaPorNome);
void buscaSequencialPorNome(Item mochila[], int numItens);
void compararEstruturas(Item mochila[], int numItens, bool ordenadaPorNome);
void testarDesempenhoBusca(Item mochila[], int numItens, bool ordenadaPorNome);
void limparBuffer();

// Funções para lista encadeada
No* criarNo(Item item);
No* inserirListaEncadeada(No* head, Item item);
No* buscarListaEncadeada(No* head, char nome[], int *comparacoes);
void liberarLista(No* head);

// Variáveis globais para análise de desempenho
int comparacoes = 0;
int comp_sequencial = 0;
int comp_binaria = 0;
int comp_lista = 0;

int main() {
    Item mochila[MAX_ITENS];
    int numItens = 0;
    bool ordenadaPorNome = false;
    int opcao;
    
    printf("=== GERENCIAMENTO DE MOCHILA - ILHA FREE FIRE ===\n\n");
    
    do {
        exibirMenu(ordenadaPorNome);
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case 1:
                inserirItem(mochila, &numItens, &ordenadaPorNome);
                break;
            case 2:
                removerItem(mochila, &numItens, &ordenadaPorNome);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4:
                menuDeOrdenacao(mochila, numItens, &ordenadaPorNome, &comparacoes);
                break;
            case 5:
                buscaBinariaPorNome(mochila, numItens, ordenadaPorNome);
                break;
            case 6:
                buscaSequencialPorNome(mochila, numItens);
                break;
            case 7:
                compararEstruturas(mochila, numItens, ordenadaPorNome);
                break;
            case 8:
                testarDesempenhoBusca(mochila, numItens, ordenadaPorNome);
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
        printf("\nPressione Enter para continuar...");
        getchar();
        limparTela();
        
    } while (opcao != 0);
    
    return 0;
}

// Função para limpar a tela
void limparTela() {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

// Função para exibir o menu principal (atualizada)
void exibirMenu(bool ordenadaPorNome) {
    printf("=========================================\n");
    printf("       GERENCIADOR DE MOCHILA\n");
    printf("=========================================\n");
    printf("1. Adicionar um item\n");
    printf("2. Remover um item\n");
    printf("3. Listar todos os itens\n");
    printf("4. Ordenar os itens por criterio\n");
    printf("5. Busca binaria por nome\n");
    printf("6. Busca sequencial por nome\n");
    printf("7. Comparar estruturas\n");
    printf("8. Testar desempenho de busca\n");
    printf("0. Sair\n");
    printf("=========================================\n");
    printf("Status: %s\n", ordenadaPorNome ? "Ordenada por nome" : "Nao ordenada por nome");
    printf("Comparacoes realizadas: %d\n", comparacoes);
    printf("=========================================\n");
}

// Função para inserir um item na mochila
void inserirItem(Item mochila[], int *numItens, bool *ordenadaPorNome) {
    if (*numItens >= MAX_ITENS) {
        printf("Mochila cheia! Capacidade maxima de %d itens.\n", MAX_ITENS);
        return;
    }
    
    printf("\n=== ADICIONAR ITEM ===\n");
    
    // Ler nome do item
    printf("Nome do item: ");
    fgets(mochila[*numItens].nome, TAM_NOME, stdin);
    mochila[*numItens].nome[strcspn(mochila[*numItens].nome, "\n")] = '\0';
    
    // Ler tipo do item
    printf("Tipo do item: ");
    fgets(mochila[*numItens].tipo, TAM_TIPO, stdin);
    mochila[*numItens].tipo[strcspn(mochila[*numItens].tipo, "\n")] = '\0';
    
    // Ler quantidade
    printf("Quantidade: ");
    scanf("%d", &mochila[*numItens].quantidade);
    limparBuffer();
    
    // Validar quantidade
    if (mochila[*numItens].quantidade <= 0) {
        printf("Quantidade deve ser maior que zero!\n");
        return;
    }
    
    // Ler prioridade
    printf("Prioridade (1-5, onde 5 é a mais alta): ");
    scanf("%d", &mochila[*numItens].prioridade);
    limparBuffer();
    
    // Validar prioridade
    if (mochila[*numItens].prioridade < 1 || mochila[*numItens].prioridade > 5) {
        printf("Prioridade deve estar entre 1 e 5!\n");
        return;
    }
    
    (*numItens)++;
    *ordenadaPorNome = false; // Mochila não está mais ordenada por nome
    printf("Item adicionado com sucesso!\n");
}

// Função para remover um item da mochila
void removerItem(Item mochila[], int *numItens, bool *ordenadaPorNome) {
    if (*numItens == 0) {
        printf("Mochila vazia! Nenhum item para remover.\n");
        return;
    }
    
    printf("\n=== REMOVER ITEM ===\n");
    
    char nomeBusca[TAM_NOME];
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    // Buscar o item pelo nome
    int indice = -1;
    for (int i = 0; i < *numItens; i++) {
        comparacoes++;
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indice = i;
            break;
        }
    }
    
    if (indice == -1) {
        printf("Item '%s' nao encontrado na mochila.\n", nomeBusca);
        return;
    }
    
    // Exibir informações do item a ser removido
    printf("Removendo item: %s (%s) - Quantidade: %d - Prioridade: %d\n", 
           mochila[indice].nome, mochila[indice].tipo, 
           mochila[indice].quantidade, mochila[indice].prioridade);
    
    // Remover item deslocando os elementos
    for (int i = indice; i < *numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    
    (*numItens)--;
    *ordenadaPorNome = false; // Mochila não está mais ordenada por nome
    printf("Item removido com sucesso!\n");
}

// Função para listar todos os itens da mochila
void listarItens(Item mochila[], int numItens) {
    printf("\n=== ITENS NA MOCHILA ===\n");
    
    if (numItens == 0) {
        printf("Mochila vazia!\n");
        return;
    }
    
    printf("=================================================================\n");
    printf("%-20s %-15s %-10s %-10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("=================================================================\n");
    
    for (int i = 0; i < numItens; i++) {
        printf("%-20s %-15s %-10d %-10d\n", 
               mochila[i].nome, 
               mochila[i].tipo, 
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    
    printf("=================================================================\n");
    printf("Total de itens: %d/%d\n", numItens, MAX_ITENS);
}

// Menu de ordenação
void menuDeOrdenacao(Item mochila[], int numItens, bool *ordenadaPorNome, int *comparacoes) {
    if (numItens == 0) {
        printf("Mochila vazia! Nenhum item para ordenar.\n");
        return;
    }
    
    printf("\n=== ORDENAR ITENS ===\n");
    printf("1. Por nome (ordem alfabetica)\n");
    printf("2. Por tipo (ordem alfabetica)\n");
    printf("3. Por prioridade (mais alta primeiro)\n");
    printf("Escolha o criterio: ");
    
    int opcao;
    scanf("%d", &opcao);
    limparBuffer();
    
    CriterioOrdenacao criterio;
    switch (opcao) {
        case 1:
            criterio = NOME;
            break;
        case 2:
            criterio = TIPO;
            break;
        case 3:
            criterio = PRIORIDADE;
            break;
        default:
            printf("Opcao invalida!\n");
            return;
    }
    
    insertionSort(mochila, numItens, criterio, comparacoes);
    
    if (criterio == NOME) {
        *ordenadaPorNome = true;
    }
    
    printf("Ordenacao concluida! Total de comparacoes: %d\n", *comparacoes);
    listarItens(mochila, numItens);
}

// Algoritmo de ordenação por inserção
void insertionSort(Item mochila[], int numItens, CriterioOrdenacao criterio, int *comparacoes) {
    *comparacoes = 0;
    
    for (int i = 1; i < numItens; i++) {
        Item key = mochila[i];
        int j = i - 1;
        
        while (j >= 0) {
            (*comparacoes)++;
            int comparacao = 0;
            
            switch (criterio) {
                case NOME:
                    comparacao = strcmp(mochila[j].nome, key.nome);
                    break;
                case TIPO:
                    comparacao = strcmp(mochila[j].tipo, key.tipo);
                    break;
                case PRIORIDADE:
                    comparacao = mochila[j].prioridade < key.prioridade ? 1 : 
                                (mochila[j].prioridade > key.prioridade ? -1 : 0);
                    break;
            }
            
            if (comparacao > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            } else {
                break;
            }
        }
        mochila[j + 1] = key;
    }
}

// Busca binária por nome
void buscaBinariaPorNome(Item mochila[], int numItens, bool ordenadaPorNome) {
    if (numItens == 0) {
        printf("Mochila vazia! Nenhum item para buscar.\n");
        return;
    }
    
    if (!ordenadaPorNome) {
        printf("Atencao: A mochila nao esta ordenada por nome.\n");
        printf("Deseja ordenar antes da busca? (1-Sim, 0-Nao): ");
        
        int opcao;
        scanf("%d", &opcao);
        limparBuffer();
        
        if (opcao == 1) {
            insertionSort(mochila, numItens, NOME, &comparacoes);
            ordenadaPorNome = true;
            printf("Mochila ordenada por nome. Total de comparacoes: %d\n", comparacoes);
        } else {
            printf("Busca binaria requer ordenacao por nome. Use busca sequencial.\n");
            return;
        }
    }
    
    printf("\n=== BUSCA BINARIA POR NOME ===\n");
    
    char nomeBusca[TAM_NOME];
    printf("Digite o nome do item a ser buscado: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    int left = 0, right = numItens - 1;
    int encontrado = -1;
    comp_binaria = 0;
    
    while (left <= right) {
        comp_binaria++;
        int mid = left + (right - left) / 2;
        
        int comparacao = strcmp(mochila[mid].nome, nomeBusca);
        
        if (comparacao == 0) {
            encontrado = mid;
            break;
        } else if (comparacao < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    if (encontrado != -1) {
        printf("\n✅ Item encontrado (apos %d comparacoes):\n", comp_binaria);
        printf("Nome: %s\n", mochila[encontrado].nome);
        printf("Tipo: %s\n", mochila[encontrado].tipo);
        printf("Quantidade: %d\n", mochila[encontrado].quantidade);
        printf("Prioridade: %d\n", mochila[encontrado].prioridade);
    } else {
        printf("\n❌ Item '%s' nao encontrado (apos %d comparacoes).\n", nomeBusca, comp_binaria);
    }
}

// Nova função: Busca sequencial por nome
void buscaSequencialPorNome(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("Mochila vazia! Nenhum item para buscar.\n");
        return;
    }
    
    printf("\n=== BUSCA SEQUENCIAL POR NOME ===\n");
    
    char nomeBusca[TAM_NOME];
    printf("Digite o nome do item a ser buscado: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    comp_sequencial = 0;
    int encontrado = -1;
    
    for (int i = 0; i < numItens; i++) {
        comp_sequencial++;
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }
    
    if (encontrado != -1) {
        printf("\n✅ Item encontrado (apos %d comparacoes):\n", comp_sequencial);
        printf("Nome: %s\n", mochila[encontrado].nome);
        printf("Tipo: %s\n", mochila[encontrado].tipo);
        printf("Quantidade: %d\n", mochila[encontrado].quantidade);
        printf("Prioridade: %d\n", mochila[encontrado].prioridade);
    } else {
        printf("\n❌ Item '%s' nao encontrado (apos %d comparacoes).\n", nomeBusca, comp_sequencial);
    }
}

// Nova função: Comparar estruturas (Vetor vs Lista Encadeada)
void compararEstruturas(Item mochila[], int numItens, bool ordenadaPorNome) {
    if (numItens == 0) {
        printf("Mochila vazia! Adicione itens primeiro.\n");
        return;
    }
    
    printf("\n=== COMPARATIVO: VETOR vs LISTA ENCADEADA ===\n");
    
    char nomeBusca[TAM_NOME];
    printf("Digite o nome do item para comparacao: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    // Criar lista encadeada a partir do vetor
    No* head = NULL;
    for (int i = 0; i < numItens; i++) {
        head = inserirListaEncadeada(head, mochila[i]);
    }
    
    // Busca no vetor (sequencial)
    comp_sequencial = 0;
    int encontradoVetor = -1;
    for (int i = 0; i < numItens; i++) {
        comp_sequencial++;
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontradoVetor = i;
            break;
        }
    }
    
    // Busca na lista encadeada
    comp_lista = 0;
    No* encontradoLista = buscarListaEncadeada(head, nomeBusca, &comp_lista);
    
    printf("\n=== RESULTADOS ===\n");
    printf("VETOR - Busca Sequencial: %d comparacoes - %s\n", 
           comp_sequencial, encontradoVetor != -1 ? "Encontrado" : "Nao encontrado");
    printf("LISTA - Busca Sequencial: %d comparacoes - %s\n", 
           comp_lista, encontradoLista != NULL ? "Encontrado" : "Nao encontrado");
    
    printf("\n=== ANALISE ===\n");
    printf("Vetor: Acesso direto, memoria contigua\n");
    printf("Lista: Alocacao dinamica, insercao/remocao eficiente\n");
    printf("Espaço Vetor: %lu bytes\n", numItens * sizeof(Item));
    printf("Espaço Lista: %lu bytes\n", numItens * sizeof(No));
    
    liberarLista(head);
}

// Nova função: Testar desempenho de busca
void testarDesempenhoBusca(Item mochila[], int numItens, bool ordenadaPorNome) {
    if (numItens == 0) {
        printf("Mochila vazia! Adicione itens primeiro.\n");
        return;
    }
    
    printf("\n=== TESTE DE DESEMPENHO DE BUSCA ===\n");
    
    char nomeBusca[TAM_NOME];
    printf("Digite o nome do item para teste: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    // Testar diferentes métodos de busca
    comp_sequencial = 0;
    comp_binaria = 0;
    
    // Busca sequencial
    int encontradoSeq = -1;
    for (int i = 0; i < numItens; i++) {
        comp_sequencial++;
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontradoSeq = i;
            break;
        }
    }
    
    // Busca binária (se estiver ordenada)
    int encontradoBin = -1;
    if (ordenadaPorNome) {
        int left = 0, right = numItens - 1;
        while (left <= right) {
            comp_binaria++;
            int mid = (left + right) / 2;
            int comparacao = strcmp(mochila[mid].nome, nomeBusca);
            
            if (comparacao == 0) {
                encontradoBin = mid;
                break;
            } else if (comparacao < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    
    printf("\n=== DESEMPENHO ===\n");
    printf("Busca Sequencial: %d comparacoes - %s\n", 
           comp_sequencial, encontradoSeq != -1 ? "Encontrado" : "Nao encontrado");
    
    if (ordenadaPorNome) {
        printf("Busca Binaria: %d comparacoes - %s\n", 
               comp_binaria, encontradoBin != -1 ? "Encontrado" : "Nao encontrado");
        printf("Melhoria: %.2fx mais rapida\n", (float)comp_sequencial / comp_binaria);
    } else {
        printf("Busca Binaria: Nao disponivel (vetor nao ordenado)\n");
    }
}

// Funções para lista encadeada
No* criarNo(Item item) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->dados = item;
    novoNo->proximo = NULL;
    return novoNo;
}

No* inserirListaEncadeada(No* head, Item item) {
    No* novoNo = criarNo(item);
    novoNo->proximo = head;
    return novoNo;
}

No* buscarListaEncadeada(No* head, char nome[], int *comparacoes) {
    *comparacoes = 0;
    No* atual = head;
    
    while (atual != NULL) {
        (*comparacoes)++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

void liberarLista(No* head) {
    No* atual = head;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}