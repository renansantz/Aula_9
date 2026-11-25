#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define N 4
#define MAXV 20
#define IDADE_MAX 20

unsigned long long fatorial(int n) {
    unsigned long long r = 1;
    int i;

    for (i = 2; i <= n; i++) {
        r *= i;
    }
    return r;
}

unsigned long long potencia(unsigned long long base, int expoente) {
    unsigned long long r = 1;
    int i;

    for (i = 0; i < expoente; i++) {
        r *= base;
    }
    return r;
}

int idExiste(unsigned long long ids[], int qtd, unsigned long long id) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (ids[i] == id) {
            return 1;
        }
    }
    return 0;
}


unsigned long long gerarID(int idade, int qtdValores, unsigned long long ids[], int cadastrados) {
    unsigned long long id = fatorial(idade) / 2;
    int metade = idade;

    while (idExiste(ids, cadastrados, id)) {
        if (metade > 0) {
            metade = metade / 2;
            id = potencia(fatorial(metade), qtdValores);
        } else {
            id++;
        }
    }
    return id;
}


double mediaPonderada(double valores[], int qtd) {
    double soma = 0, somaPesos = 0;
    int i;

    for (i = 0; i < qtd; i++) {
        soma += valores[i] * (i + 1);
        somaPesos += (i + 1);
    }
    return soma / somaPesos;
}


int compararNomes(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) {
            return ca - cb;
        }
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

int main() {
    char nomes[N][50], tmpNome[50];
    int idades[N];
    unsigned long long ids[N], tmpId;
    double valores[N][MAXV], tmpVal;
    double medias[N];
    double lados[2], diagonal = 0;
    int qtdLados = 0, temDiagonal = 0;
    int qtdValores, i, j, k;


    for (i = 0; i < N; i++) {
        printf("Participante %d - Nome: ", i + 1);
        scanf(" %49[^\n]", nomes[i]);

        do {
            printf("Participante %d - Idade (1 a %d): ", i + 1, IDADE_MAX);
            scanf("%d", &idades[i]);
        } while (idades[i] < 1 || idades[i] > IDADE_MAX);
    }


    do {
        printf("\nQuantidade de valores por participante (1 a %d): ", MAXV);
        scanf("%d", &qtdValores);
    } while (qtdValores < 1 || qtdValores > MAXV);

    for (i = 0; i < N; i++) {
        for (j = 0; j < qtdValores; j++) {
            printf("%s - valor %d: ", nomes[i], j + 1);
            scanf("%lf", &valores[i][j]);

            if (valores[i][j] >= 0 && qtdLados < 2) {
                lados[qtdLados] = valores[i][j];
                qtdLados++;
            }
        }
    }


    printf("\n");
    for (i = 0; i < N; i++) {
        ids[i] = gerarID(idades[i], qtdValores, ids, i);
        printf("ID gerado para %s: %llu\n", nomes[i], ids[i]);
    }


    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - 1 - i; j++) {
            if (compararNomes(nomes[j], nomes[j + 1]) > 0) {
                strcpy(tmpNome, nomes[j]);
                strcpy(nomes[j], nomes[j + 1]);
                strcpy(nomes[j + 1], tmpNome);

                tmpId = ids[j];
                ids[j] = ids[j + 1];
                ids[j + 1] = tmpId;

                for (k = 0; k < qtdValores; k++) {
                    tmpVal = valores[j][k];
                    valores[j][k] = valores[j + 1][k];
                    valores[j + 1][k] = tmpVal;
                }
            }
        }
    }


    for (i = 0; i < N; i++) {
        medias[i] = mediaPonderada(valores[i], qtdValores);
    }


    if (qtdLados == 2) {
        diagonal = sqrt(lados[0] * lados[0] + lados[1] * lados[1]);
        temDiagonal = 1;
    }


    printf("\n========== CLASSIFICACAO FINAL ==========\n\n");

    if (!temDiagonal) {
        printf("Nao foi possivel calcular a diagonal (menos de dois valores nao negativos).\n\n");
    } else {
        printf("Lados: %.2f e %.2f | Diagonal: %.2f\n\n", lados[0], lados[1], diagonal);
    }

    for (i = 0; i < N; i++) {
        const char *texto;

        if (!temDiagonal) {
            texto = "SEM CLASSIFICACAO";
        } else if (fabs(medias[i] - diagonal) < 1e-9) {
            texto = "SOBRE A DIAGONAL";
        } else if (medias[i] > diagonal) {
            texto = "ACIMA DA DIAGONAL";
        } else {
            texto = "ABAIXO DA DIAGONAL";
        }

        printf("%s ", nomes[i]);
        for (k = (int)strlen(nomes[i]); k < 20; k++) {
            putchar('.');
        }
        printf(" %s\n", texto);
    }

    printf("\n=========================================\n");

    return 0;
}
