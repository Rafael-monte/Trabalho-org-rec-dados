#include "flag-controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Processo* ProcessarFlagsIniciais(int numero_argumentos, char* argumentos[])
{
    struct Processo* processo = malloc(sizeof(processo));
    if (argumentos[POSICAO_ARQUIVO_TARGET] != NULL)
    {
        strcpy(processo->nome_arq_busca, argumentos[POSICAO_ARQUIVO_TARGET]);
        strcpy(processo->nome_arq_principal, NOME_ARQUIVO_PRINCIPAL);
        if (strcmp(argumentos[POSICAO_FLAG_OPERACAO], ARQUIVO_IMPORTACAO) == 1) {
            processo->operacao='i';
        }
        else if (strcmp(argumentos[POSICAO_FLAG_OPERACAO], ARQUIVO_OPERACOES) == 1)
        {
            processo->operacao='e';
        }
        else
        {
            printf("[ERRO] Nao foi possivel processar o argumento %s. Abortando.", argumentos[POSICAO_FLAG_OPERACAO]);
            exit(EXIT_FAILURE);
        }
        return processo;
    }
    else
    {
        printf("[ERRO] Nao existe arquivo para processar as operacoes", argumentos[POSICAO_ARQUIVO_TARGET]);
        exit(EXIT_FAILURE);
    }
}