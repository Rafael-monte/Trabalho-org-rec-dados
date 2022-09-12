#pragma once
#include "flag-controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Processo* ProcessarFlagsIniciais(int numero_argumentos, char* argumentos[])
{
    struct Processo* processo = malloc(sizeof(processo));
    if (numero_argumentos > 1)
    {
        //Realocando strings para evitar problemas de memoria
        if (!(processo->nome_arq_busca = malloc(strlen(argumentos[POSICAO_ARQUIVO_TARGET] + 1))))
        {
            printf("Ocorreu um erro ao alocar o espaco necessario para a string %s", argumentos[POSICAO_ARQUIVO_TARGET]);
            exit(EXIT_FAILURE);
        }
        strcpy(processo->nome_arq_busca, argumentos[POSICAO_ARQUIVO_TARGET]);

        //Realocando strings para evitar problemas de memoria
        if (!(processo->nome_arq_principal = malloc(strlen(NOME_ARQUIVO_PRINCIPAL + 1))))
        {
            printf("Ocorreu um erro ao alocar o espaco necessario para a string %s", NOME_ARQUIVO_PRINCIPAL);
            exit(EXIT_FAILURE);
        }
        strcpy(processo->nome_arq_principal, NOME_ARQUIVO_PRINCIPAL);
        if (strcmp(argumentos[POSICAO_FLAG_OPERACAO], ARQUIVO_IMPORTACAO) == 0) {
            processo->operacao='i';
        }
        else if (strcmp(argumentos[POSICAO_FLAG_OPERACAO], ARQUIVO_OPERACOES) == 0)
        {
            processo->operacao='e';
        }
        else
        {
            printf("\033[0;31m[ERRO] Nao foi possivel processar o argumento %s. Abortando.", argumentos[POSICAO_FLAG_OPERACAO]);
            exit(EXIT_FAILURE);
        }
        return processo;
    }
    else
    {
        printf("\033[0;31m[ERRO] Nao existe arquivo para processar as operacoes");
        exit(EXIT_FAILURE);
    }
}