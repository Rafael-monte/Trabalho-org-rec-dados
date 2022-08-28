#include <stdio.h>
#include "./files/flag-controller/flag-controller.c"
int main(int argc, char* argv[]) {
    struct Processo* processo = ProcessarFlagsIniciais(argc, argv);
    printf("ProcessarFlagsIniciais: operacao -> %c", processo->operacao);
    printf("\nNome do arquivo de busca -> %s",processo->nome_arq_busca);
    printf("\nNome do arquivo principal -> %s",processo->nome_arq_principal);
    return 0;
}
