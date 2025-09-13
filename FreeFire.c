#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20

// Struct Componente para a torre de resgate
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade; // 1 a 10
} Componente;

// Protótipos das funções para a torre de resgate
void bubbleSortPorNome(Componente componentes[], int numComponentes, int *comparacoes, double *tempo);
void insertionSortPorTipo(Componente componentes[], int numComponentes, int *comparacoes, double *tempo);
void selectionSortPorPrioridade(Componente componentes[], int numComponentes, int *comparacoes, double *tempo);
void buscaBinariaComponenteChave(Componente componentes[], int numComponentes);
void mostrarComponentesTorre(Componente componentes[], int numComponentes);
void cadastrarComponentesTorre(Componente componentes[], int *numComponentes);
void menuTorreResgate(Componente componentes[], int numComponentes);
void limparBuffer();

// Variáveis globais para estatísticas
int comp_bubble = 0;
int comp_insertion = 0;
int comp_selection = 0;
double tempo_bubble = 0.0;
double tempo_insertion = 0.0;
double tempo_selection = 0.0;

int main() {
    Componente componentesTorre[MAX_COMPONENTES];
    int numComponentes = 0;
    int opcaoPrincipal;
    
    printf("=== SISTEMA DE CONSTRUCAO DA TORRE DE RESGATE ===\n\n");
    
    do {
        printf("=========================================\n");
        printf("          MENU PRINCIPAL\n");
        printf("=========================================\n");
        printf("1. Gerenciar Mochila\n");
        printf("2. Construir Torre de Resgate\n");
        printf("0. Sair\n");
        printf("=========================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcaoPrincipal);
        limparBuffer();
        
        switch (opcaoPrincipal) {
            case 1:
                // Aqui viria o código existente da mochila
                printf("\n=== MODULO MOCHILA ===\n");
                printf("Funcionalidades de mochila aqui...\n");
                break;
                
            case 2:
                printf("\n=== TORRE DE RESGATE ===\n");
                cadastrarComponentesTorre(componentesTorre, &numComponentes);
                if (numComponentes > 0) {
                    menuTorreResgate(componentesTorre, numComponentes);
                }
                break;
                
            case 0:
                printf("Saindo do sistema...\n");
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
        printf("\nPressione Enter para continuar...");
        getchar();
        
    } while (opcaoPrincipal != 0);
    
    return 0;
}

void cadastrarComponentesTorre(Componente componentes[], int *numComponentes) {
    printf("\n=== CADASTRO DE COMPONENTES DA TORRE ===\n");
    printf("Quantos componentes deseja cadastrar? (max %d): ", MAX_COMPONENTES);
    scanf("%d", numComponentes);
    limparBuffer();
    
    if (*numComponentes > MAX_COMPONENTES) {
        printf("Limite excedido! Cadastrando %d componentes.\n", MAX_COMPONENTES);
        *numComponentes = MAX_COMPONENTES;
    }
    
    for (int i = 0; i < *numComponentes; i++) {
        printf("\nComponente %d:\n", i + 1);
        
        printf("Nome: ");
        fgets(componentes[i].nome, TAM_NOME, stdin);
        componentes[i].nome[strcspn(componentes[i].nome, "\n")] = '\0';
        
        printf("Tipo (controle/suporte/propulsao): ");
        fgets(componentes[i].tipo, TAM_TIPO, stdin);
        componentes[i].tipo[strcspn(componentes[i].tipo, "\n")] = '\0';
        
        printf("Prioridade (1-10): ");
        scanf("%d", &componentes[i].prioridade);
        limparBuffer();
        
        // Validar prioridade
        while (componentes[i].prioridade < 1 || componentes[i].prioridade > 10) {
            printf("Prioridade deve ser entre 1 e 10! Digite novamente: ");
            scanf("%d", &componentes[i].prioridade);
            limparBuffer();
        }
    }
    printf("\nCadastro concluido! %d componentes registrados.\n", *numComponentes);
}

void menuTorreResgate(Componente componentes[], int numComponentes) {
    int opcao;
    bool ordenadoPorNome = false;
    
    do {
        printf("\n=== TORRE DE RESGATE - MENU ===\n");
        printf("=========================================\n");
        printf("1. Mostrar componentes\n");
        printf("2. Ordenar por nome (Bubble Sort)\n");
        printf("3. Ordenar por tipo (Insertion Sort)\n");
        printf("4. Ordenar por prioridade (Selection Sort)\n");
        printf("5. Buscar componente-chave\n");
        printf("6. Mostrar estatisticas de ordenacao\n");
        printf("7. Montar torre (Verificar componentes)\n");
        printf("0. Voltar\n");
        printf("=========================================\n");
        printf("Status: %s\n", ordenadoPorNome ? "Ordenado por nome" : "Nao ordenado");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case 1:
                mostrarComponentesTorre(componentes, numComponentes);
                break;
                
            case 2:
                bubbleSortPorNome(componentes, numComponentes, &comp_bubble, &tempo_bubble);
                ordenadoPorNome = true;
                printf("✅ Componentes ordenados por nome!\n");
                break;
                
            case 3:
                insertionSortPorTipo(componentes, numComponentes, &comp_insertion, &tempo_insertion);
                printf("✅ Componentes ordenados por tipo!\n");
                break;
                
            case 4:
                selectionSortPorPrioridade(componentes, numComponentes, &comp_selection, &tempo_selection);
                printf("✅ Componentes ordenados por prioridade!\n");
                break;
                
            case 5:
                if (ordenadoPorNome) {
                    buscaBinariaComponenteChave(componentes, numComponentes);
                } else {
                    printf("❌ Primeiro ordene por nome para usar busca binaria!\n");
                }
                break;
                
            case 6:
                printf("\n=== ESTATISTICAS DE ORDENACAO ===\n");
                printf("Bubble Sort (Nome): %d comparacoes, %.6f segundos\n", comp_bubble, tempo_bubble);
                printf("Insertion Sort (Tipo): %d comparacoes, %.6f segundos\n", comp_insertion, tempo_insertion);
                printf("Selection Sort (Prioridade): %d comparacoes, %.6f segundos\n", comp_selection, tempo_selection);
                break;
                
            case 7:
                printf("\n=== MONTAGEM DA TORRE ===\n");
                printf("Verificando componentes necessarios...\n");
                
                // Verificar se temos componentes essenciais
                bool temControle = false;
                bool temPropulsao = false;
                bool temSuporte = false;
                
                for (int i = 0; i < numComponentes; i++) {
                    if (strcmp(componentes[i].tipo, "controle") == 0) temControle = true;
                    if (strcmp(componentes[i].tipo, "propulsao") == 0) temPropulsao = true;
                    if (strcmp(componentes[i].tipo, "suporte") == 0) temSuporte = true;
                }
                
                if (temControle && temPropulsao && temSuporte) {
                    printf("✅ TORRE MONTADA COM SUCESSO!\n");
                    printf("🚀 Preparando para escapar da ilha...\n");
                } else {
                    printf("❌ Componentes insuficientes para montar a torre!\n");
                    if (!temControle) printf("   - Falta: componente de controle\n");
                    if (!temPropulsao) printf("   - Falta: componente de propulsao\n");
                    if (!temSuporte) printf("   - Falta: componente de suporte\n");
                }
                break;
                
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
        printf("\nPressione Enter para continuar...");
        getchar();
        
    } while (opcao != 0);
}

// Bubble Sort para ordenar por nome
void bubbleSortPorNome(Componente componentes[], int numComponentes, int *comparacoes, double *tempo) {
    *comparacoes = 0;
    clock_t inicio = clock();
    
    for (int i = 0; i < numComponentes - 1; i++) {
        for (int j = 0; j < numComponentes - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                // Troca os componentes
                Componente temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
    
    clock_t fim = clock();
    *tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// Insertion Sort para ordenar por tipo
void insertionSortPorTipo(Componente componentes[], int numComponentes, int *comparacoes, double *tempo) {
    *comparacoes = 0;
    clock_t inicio = clock();
    
    for (int i = 1; i < numComponentes; i++) {
        Componente chave = componentes[i];
        int j = i - 1;
        
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) {
                componentes[j + 1] = componentes[j];
                j--;
            } else {
                break;
            }
        }
        componentes[j + 1] = chave;
    }
    
    clock_t fim = clock();
    *tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// Selection Sort para ordenar por prioridade (decrescente)
void selectionSortPorPrioridade(Componente componentes[], int numComponentes, int *comparacoes, double *tempo) {
    *comparacoes = 0;
    clock_t inicio = clock();
    
    for (int i = 0; i < numComponentes - 1; i++) {
        int indiceMaximo = i;
        
        for (int j = i + 1; j < numComponentes; j++) {
            (*comparacoes)++;
            if (componentes[j].prioridade > componentes[indiceMaximo].prioridade) {
                indiceMaximo = j;
            }
        }
        
        if (indiceMaximo != i) {
            Componente temp = componentes[i];
            componentes[i] = componentes[indiceMaximo];
            componentes[indiceMaximo] = temp;
        }
    }
    
    clock_t fim = clock();
    *tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

void buscaBinariaComponenteChave(Componente componentes[], int numComponentes) {
    printf("\n=== BUSCA DE COMPONENTE-CHAVE ===\n");
    printf("Componente essencial para ativacao da torre:\n");
    
    char nomeChave[TAM_NOME];
    printf("Digite o nome do componente-chave: ");
    fgets(nomeChave, TAM_NOME, stdin);
    nomeChave[strcspn(nomeChave, "\n")] = '\0';
    
    int comp_busca = 0;
    int esquerda = 0, direita = numComponentes - 1;
    int encontrado = -1;
    
    while (esquerda <= direita) {
        comp_busca++;
        int meio = (esquerda + direita) / 2;
        int comparacao = strcmp(componentes[meio].nome, nomeChave);
        
        if (comparacao == 0) {
            encontrado = meio;
            break;
        } else if (comparacao < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    
    if (encontrado != -1) {
        printf("\n✅ COMPONENTE-CHAVE ENCONTRADO!\n");
        printf("Nome: %s\n", componentes[encontrado].nome);
        printf("Tipo: %s\n", componentes[encontrado].tipo);
        printf("Prioridade: %d\n", componentes[encontrado].prioridade);
        printf("Comparacoes necessarias: %d\n", comp_busca);
        printf("🎯 Torre pode ser ativada!\n");
    } else {
        printf("\n❌ COMPONENTE-CHAVE NAO ENCONTRADO!\n");
        printf("Comparacoes realizadas: %d\n", comp_busca);
        printf("⚠️  Verifique o nome ou adquira o componente\n");
    }
}

void mostrarComponentesTorre(Componente componentes[], int numComponentes) {
    printf("\n=== COMPONENTES DA TORRE ===\n");
    printf("==================================================\n");
    printf("%-20s %-15s %-10s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("==================================================\n");
    
    for (int i = 0; i < numComponentes; i++) {
        printf("%-20s %-15s %-10d\n", 
               componentes[i].nome, 
               componentes[i].tipo, 
               componentes[i].prioridade);
    }
    printf("==================================================\n");
    printf("Total: %d componentes\n", numComponentes);
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}