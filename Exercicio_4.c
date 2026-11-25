#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int registro;
    int horas_aula;
} Professor;

/* Grava o buffer bruto inteiro com uma unica chamada de fwrite */
int gravarBuffer(const char *nomeArquivo, const unsigned char *buffer, size_t tamanhoBytes) {
    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        return 0;
    }

    if (fwrite(buffer, 1, tamanhoBytes, arquivo) != tamanhoBytes) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

/* Le o arquivo todo para um novo array dinamico. Devolve NULL em caso de erro */
Professor *lerProfessores(const char *nomeArquivo, int n) {
    FILE *arquivo;
    Professor *lista;

    arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        return NULL;
    }

    lista = (Professor *) malloc(n * sizeof(Professor));
    if (lista == NULL) {
        fclose(arquivo);
        return NULL;
    }

    if (fread(lista, sizeof(Professor), n, arquivo) != (size_t) n) {
        free(lista);
        fclose(arquivo);
        return NULL;
    }

    fclose(arquivo);
    return lista;
}

int main() {
    Professor *professores, *carregados;
    unsigned char *buffer;
    int n, i;
    size_t tamanhoBytes;

    printf("Quantos professores deseja cadastrar? ");
    scanf("%d", &n);

    if (n < 1) {
        printf("Quantidade invalida.\n");
        return 1;
    }

    /* Array dinamico de N professores */
    professores = (Professor *) malloc(n * sizeof(Professor));
    if (professores == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("\nProfessor %d\n", i + 1);
        printf("Registro: ");
        scanf("%d", &professores[i].registro);
        printf("Horas-aula: ");
        scanf("%d", &professores[i].horas_aula);
    }

    /* Buffer bruto de bytes com N * sizeof(Professor) */
    tamanhoBytes = n * sizeof(Professor);
    buffer = (unsigned char *) malloc(tamanhoBytes);
    if (buffer == NULL) {
        printf("Erro ao alocar o buffer.\n");
        free(professores);
        return 1;
    }

    /* Copia o array inteiro de structs para o buffer */
    memcpy(buffer, professores, tamanhoBytes);

    /* Uma unica chamada de fwrite grava tudo (dentro de gravarBuffer) */
    if (!gravarBuffer("folha.dat", buffer, tamanhoBytes)) {
        printf("Erro ao gravar folha.dat.\n");
        free(professores);
        free(buffer);
        return 1;
    }
    printf("\n%d professor(es) gravado(s) em folha.dat (%zu bytes).\n", n, tamanhoBytes);

    /* Le tudo de volta em um novo array dinamico */
    carregados = lerProfessores("folha.dat", n);
    if (carregados == NULL) {
        printf("Erro ao ler folha.dat.\n");
        free(professores);
        free(buffer);
        return 1;
    }

    printf("\nDados lidos do arquivo:\n");
    for (i = 0; i < n; i++) {
        printf("Registro: %d | Horas-aula: %d\n",
               carregados[i].registro, carregados[i].horas_aula);
    }

    free(professores);
    free(buffer);
    free(carregados);

    return 0;
}
