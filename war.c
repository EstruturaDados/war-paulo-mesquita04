#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 5
#define TAM_NOME 50
#define TAM_COR 20

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

/* limpa o buffer de entrada ate o fim da linha */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* imprime o mapa atual */
void imprimirMapa(const Territorio *m, int n) {
    int i;
    printf("\n=== MAPA ATUAL ===\n");
    for (i = 0; i < n; ++i) {
        printf("%d) %s | Cor: %s | Tropas: %d\n",
               i + 1, m[i].nome, m[i].cor, m[i].tropas);
    }
}

/* NIVEL NOVATO - vetor estatico */
void nivelNovato(void) {
    Territorio mapa[MAX];
    int i;

    printf("\n=== NIVEL NOVATO ===\n");

    for (i = 0; i < MAX; ++i) {
        printf("\n--- Territorio %d ---\n", i + 1);

        printf("Nome: ");
        if (fgets(mapa[i].nome, TAM_NOME, stdin) == NULL)
            mapa[i].nome[0] = '\0';
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        if (fgets(mapa[i].cor, TAM_COR, stdin) == NULL)
            mapa[i].cor[0] = '\0';
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Numero de tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1) {
            mapa[i].tropas = 0;
        }
        limparBuffer();
    }

    imprimirMapa(mapa, MAX);
    printf("\nFim do nivel Novato.\n\n");
}

/* sistema de batalha (usado nos niveis seguintes) */
void atacar(Territorio *m, int a, int d) {
    int atk = rand() % 6 + 1;
    int def = rand() % 6 + 1;

    if (a < 0 || a >= MAX || d < 0 || d >= MAX) {
        printf("Indices de territórios invalidos.\n");
        return;
    }

    printf("\nDados -> ATK: %d | DEF: %d\n", atk, def);

    if (atk >= def) {
        m[d].tropas--;
        printf("Atacante venceu! Defesa perdeu 1 tropa.\n");
        if (m[d].tropas <= 0) {
            printf(">>> Territorio conquistado!\n");
            strncpy(m[d].cor, m[a].cor, TAM_COR - 1);
            m[d].cor[TAM_COR - 1] = '\0';
            m[d].tropas = 1;
        }
    } else {
        printf("Defensor resistiu!\n");
    }
}

/* NIVEL AVENTUREIRO - alocacao dinamica + batalhas */
void nivelAventureiro(void) {
    Territorio *mapa;
    int i;
    int atk, def;

    printf("\n=== NIVEL AVENTUREIRO ===\n");

    mapa = (Territorio *) calloc(MAX, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        return;
    }

    for (i = 0; i < MAX; ++i) {
        printf("\n--- Territorio %d ---\n", i + 1);

        printf("Nome: ");
        if (fgets(mapa[i].nome, TAM_NOME, stdin) == NULL)
            mapa[i].nome[0] = '\0';
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        if (fgets(mapa[i].cor, TAM_COR, stdin) == NULL)
            mapa[i].cor[0] = '\0';
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Numero de tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1) mapa[i].tropas = 0;
        limparBuffer();
    }

    while (1) {
        imprimirMapa(mapa, MAX);

        printf("\nAtacante (1-5, 0 para sair): ");
        if (scanf("%d", &atk) != 1) { limparBuffer(); break; }
        if (atk == 0) { limparBuffer(); break; }

        printf("Defensor (1-5): ");
        if (scanf("%d", &def) != 1) { limparBuffer(); break; }
        limparBuffer();

        atacar(mapa, atk - 1, def - 1);
    }

    free(mapa);
    printf("\nFim do nivel Aventureiro.\n\n");
}

/* NIVEL MESTRE - inicializacao e missoes */
void initTerritorios(Territorio *m) {
    strncpy(m[0].nome, "Alfa", TAM_NOME - 1); m[0].nome[TAM_NOME - 1] = '\0';
    strncpy(m[0].cor, "Vermelho", TAM_COR - 1); m[0].cor[TAM_COR - 1] = '\0';
    m[0].tropas = 3;

    strncpy(m[1].nome, "Bravo", TAM_NOME - 1); m[1].nome[TAM_NOME - 1] = '\0';
    strncpy(m[1].cor, "Verde", TAM_COR - 1); m[1].cor[TAM_COR - 1] = '\0';
    m[1].tropas = 3;

    strncpy(m[2].nome, "Charlie", TAM_NOME - 1); m[2].nome[TAM_NOME - 1] = '\0';
    strncpy(m[2].cor, "Azul", TAM_COR - 1); m[2].cor[TAM_COR - 1] = '\0';
    m[2].tropas = 3;

    strncpy(m[3].nome, "Delta", TAM_NOME - 1); m[3].nome[TAM_NOME - 1] = '\0';
    strncpy(m[3].cor, "Amarelo", TAM_COR - 1); m[3].cor[TAM_COR - 1] = '\0';
    m[3].tropas = 3;

    strncpy(m[4].nome, "Echo", TAM_NOME - 1); m[4].nome[TAM_NOME - 1] = '\0';
    strncpy(m[4].cor, "Preto", TAM_COR - 1); m[4].cor[TAM_COR - 1] = '\0';
    m[4].tropas = 3;
}

int missaoCumprida(const Territorio *m, int tipo, const char *alvo) {
    int i;
    if (tipo == 1) {
        /* destruir todos os territorios da cor 'alvo' */
        for (i = 0; i < MAX; ++i) {
            if (strcmp(m[i].cor, alvo) == 0) return 0;
        }
        return 1;
    } else if (tipo == 2) {
        /* conquistar 3 territorios Vermelho */
        int cont = 0;
        for (i = 0; i < MAX; ++i) {
            if (strcmp(m[i].cor, "Vermelho") == 0) ++cont;
        }
        return cont >= 3;
    }
    return 0;
}

void nivelMestre(void) {
    Territorio mapa[MAX];
    int tipoMissao;
    char corAlvo[TAM_COR] = "Verde";
    int op;

    printf("\n=== NIVEL MESTRE ===\n");

    initTerritorios(mapa);
    tipoMissao = (rand() % 2) + 1;

    while (1) {
        imprimirMapa(mapa, MAX);

        printf("\n=== MENU ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &op) != 1) { limparBuffer(); break; }
        limparBuffer();

        if (op == 0) break;
        if (op == 1) {
            int a, d;
            printf("Atacante (1-5): ");
            if (scanf("%d", &a) != 1) { limparBuffer(); break; }
            printf("Defensor (1-5): ");
            if (scanf("%d", &d) != 1) { limparBuffer(); break; }
            limparBuffer();
            atacar(mapa, a - 1, d - 1);
        } else if (op == 2) {
            printf("\n--- MISSÃO ---\n");
            if (tipoMissao == 1) {
                printf("Destruir o exército %s.\n", corAlvo);
            } else {
                printf("Conquistar 3 territorios Vermelhos.\n");
            }

            if (missaoCumprida(mapa, tipoMissao, corAlvo)) {
                printf("\n** MISSÃO CUMPRIDA! VOCE VENCEU! **\n");
                break;
            } else {
                printf("Missao ainda nao concluida.\n");
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }

    printf("\nFim do nivel Mestre.\n\n");
}

/* main - menu geral */
int main(void) {
    int opcao;

    srand((unsigned int) time(NULL));

    while (1) {
        printf("\n=== WAR ESTRUTURADO - MENU PRINCIPAL ===\n");
        printf("1 - Nivel Novato\n");
        printf("2 - Nivel Aventureiro\n");
        printf("3 - Nivel Mestre\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) { limparBuffer(); break; }
        limparBuffer();

        switch (opcao) {
            case 1:
                nivelNovato();
                break;
            case 2:
                nivelAventureiro();
                break;
            case 3:
                nivelMestre();
                break;
            case 0:
                printf("\nSaindo...\n");
                return 0;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    }

    return 0;
}
