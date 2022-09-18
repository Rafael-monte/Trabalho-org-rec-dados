#pragma once
#include "flag-controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../file-handler/file-handler.c"
struct Processo* ProcessarFlagsIniciais(int numero_argumentos, char* argumentos[])
{
    struct Processo* processo = malloc(sizeof(processo));
    if (numero_argumentos > 1)
    {
        strcpy(processo->nome_arq_busca, argumentos[POSICAO_ARQUIVO_TARGET]);

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


struct Processo* ProcessarArquivoComandos(FILE* arquivo_comandos)
{
    char caractereAtual;
    char buffer[256];
    char bufferOperacao[256];
    int posicaoCaractereAtual=0;
    char* token;
    struct Processo* processo_final = malloc(sizeof(struct Processo));
    struct Processo* pivot = processo_final;
    while ((caractereAtual = fgetc(arquivo_comandos)) != EOF)
    {
        if (caractereAtual != '\n')
        {
           buffer[posicaoCaractereAtual] = caractereAtual;
           buffer[posicaoCaractereAtual+1]='\0';
           posicaoCaractereAtual++;
        }
        else
        {
            printf("\nString atual: %s\n", buffer);
            token = strtok(buffer, " ");
            pivot->operacao = buffer[0];
            for (int i = 3; i < posicaoCaractereAtual; i++)
            {
                bufferOperacao[i - 3] = buffer[i];
            }
            strcpy(pivot->parametro_operacao, bufferOperacao);
            token = strtok(NULL, " ");
            strcpy(pivot->parametro_operacao, token);
            printf("\nOperacao: %s\n", pivot->parametro_operacao);
            posicaoCaractereAtual = 0;
            LimparBuffer(buffer);
            LimparBuffer(bufferOperacao);
            pivot->proximo_processo = malloc(sizeof(struct Processo));
            pivot = pivot->proximo_processo;
        }
    }
    pivot->proximo_processo = NULL;
    return processo_final;
}