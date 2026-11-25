#include <stdio.h>

typedef struct {
    int conta_corrente;
    char nome[50];
    float saldo;
} Cliente;

int main() {
    FILE *arquivo;
    Cliente cliente;
    int conta, encontrou = 0;
    float valor;


    printf("Numero da conta corrente: ");
    scanf("%d", &conta);

    printf("Valor (positivo = deposito, negativo = saque): ");
    scanf("%f", &valor);


    arquivo = fopen("clientes.bin", "rb+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo clientes.bin.\n");
        return 1;
    }


    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.conta_corrente == conta) {
            encontrou = 1;

            if (valor < 0 && cliente.saldo + valor < 0) {
                printf("Saldo insuficiente. Saldo atual: R$ %.2f\n", cliente.saldo);
                break;
            }

            cliente.saldo += valor;


            fseek(arquivo, -(long) sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
            fflush(arquivo);

            printf("Conta %d (%s) atualizada. Novo saldo: R$ %.2f\n",
                   cliente.conta_corrente, cliente.nome, cliente.saldo);
            break;
        }
    }

    if (!encontrou) {
        printf("Conta %d nao encontrada.\n", conta);
    }

    fclose(arquivo);
    return 0;
}
