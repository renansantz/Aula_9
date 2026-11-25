#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char titulo[12];
    int zona;
} Eleitor;

typedef struct {
    Eleitor eleitor;
    int numero_voto;
} Cedula;

/* a) Adiciona um voto, aumentando o bloco com realloc.
   Devolve 1 em caso de sucesso e 0 em caso de erro (a lista antiga continua valida). */
int adicionarVoto(Cedula **lista, int *quantidade, Cedula novoVoto) {
    Cedula *temp = (Cedula *) realloc(*lista, (*quantidade + 1) * sizeof(Cedula));

    if (temp == NULL) {
        return 0;
    }

    *lista = temp;
    (*lista)[*quantidade] = novoVoto;
    (*quantidade)++;
    return 1;
}

/* b) Salva a lista completa em arquivo binario */
int salvarUrna(const char *nomeArquivo, Cedula *lista, int quantidade) {
    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        return 0;
    }

    if (fwrite(lista, sizeof(Cedula), quantidade, arquivo) != (size_t) quantidade) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

/* c) Recuperacao rapida: descobre o tamanho com fseek/ftell, aloca uma vez e le tudo */
Cedula *recuperarUrna(const char *nomeArquivo, int *quantidade) {
    FILE *arquivo;
    long tamanho;
    Cedula *lista;

    *quantidade = 0;

    arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    rewind(arquivo);

    if (tamanho <= 0) {
        fclose(arquivo);
        return NULL;
    }

    lista = (Cedula *) malloc(tamanho);
    if (lista == NULL) {
        fclose(arquivo);
        return NULL;
    }

    *quantidade = (int) (tamanho / sizeof(Cedula));

    if (fread(lista, sizeof(Cedula), *quantidade, arquivo) != (size_t) *quantidade) {
        free(lista);
        fclose(arquivo);
        *quantidade = 0;
        return NULL;
    }

    fclose(arquivo);
    return lista;
}

int main() {
    Cedula *votos = NULL, *recuperados;
    Cedula novo;
    int quantidade = 0, qtdRecuperada = 0;
    char continuar;
    int i;

    do {
        printf("\nTitulo do eleitor (ate 11 caracteres): ");
        scanf(" %11s", novo.eleitor.titulo);

        printf("Zona: ");
        scanf("%d", &novo.eleitor.zona);

        printf("Numero do voto: ");
        scanf("%d", &novo.numero_voto);

        if (!adicionarVoto(&votos, &quantidade, novo)) {
            printf("Erro ao alocar memoria.\n");
            free(votos);
            return 1;
        }

        printf("Registrar outro voto? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    if (!salvarUrna("urnas.bin", votos, quantidade)) {
        printf("Erro ao gravar urnas.bin.\n");
        free(votos);
        return 1;
    }
    printf("\n%d voto(s) salvo(s) em urnas.bin\n", quantidade);

    recuperados = recuperarUrna("urnas.bin", &qtdRecuperada);
    if (recuperados == NULL) {
        printf("Erro ao recuperar urnas.bin.\n");
        free(votos);
        return 1;
    }

    printf("\nVotos recuperados do arquivo (%d):\n", qtdRecuperada);
    for (i = 0; i < qtdRecuperada; i++) {
        printf("Titulo: %s | Zona: %d | Voto: %d\n",
               recuperados[i].eleitor.titulo,
               recuperados[i].eleitor.zona,
               recuperados[i].numero_voto);
    }

    free(votos);
    free(recuperados);

    return 0;
}
