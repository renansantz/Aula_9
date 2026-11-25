#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char cargo[30];
    float salario;
} Funcionario;

int main() {
    Funcionario *original, *backup;
    FILE *arquivo;


    original = (Funcionario *) malloc(sizeof(Funcionario));
    if (original == NULL) {
        printf("Erro ao alocar memoria para o funcionario.\n");
        return 1;
    }

    printf("ID: ");
    scanf("%d", &original->id);

    printf("Cargo: ");
    scanf(" %29[^\n]", original->cargo);

    printf("Salario: ");
    scanf("%f", &original->salario);


    backup = (Funcionario *) malloc(sizeof(Funcionario));
    if (backup == NULL) {
        printf("Erro ao alocar memoria para o backup.\n");
        free(original);
        return 1;
    }


    memcpy(backup, original, sizeof(Funcionario));


    arquivo = fopen("backup_rh.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo backup_rh.bin.\n");
        free(original);
        free(backup);
        return 1;
    }

    if (fwrite(backup, sizeof(Funcionario), 1, arquivo) != 1) {
        printf("Erro ao gravar o backup no arquivo.\n");
    } else {
        printf("\nBackup salvo em backup_rh.bin\n");
        printf("ID: %d | Cargo: %s | Salario: R$ %.2f\n",
               backup->id, backup->cargo, backup->salario);
    }

    fclose(arquivo);
    free(original);
    free(backup);

    return 0;
}
