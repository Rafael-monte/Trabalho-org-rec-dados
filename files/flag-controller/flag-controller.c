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
        
        strcpy(processo->nome_arq_principal, NOME_ARQUIVO_PRINCIPAL);
        if (strcmp(argumentos[POSICAO_FLAG_OPERACAO], ARQUIVO_IMPORTACAO) == 0) {
            processo->operacao='i';
            strcpy(processo->nome_arq_busca, argumentos[POSICAO_ARQUIVO_TARGET]);
        }
        else if (strcmp(argumentos[POSICAO_FLAG_OPERACAO], ARQUIVO_OPERACOES) == 0)
        {
            processo->operacao='e';
            strcpy(processo->nome_arq_busca, argumentos[POSICAO_ARQUIVO_TARGET]);
        }
        else if (strcmp(argumentos[POSICAO_FLAG_OPERACAO], ARQUIVO_IMPRESSAO_LED) == 0)
        {
           processo->operacao='p';
        }
        else 
        {
            LogError(ERROR_OPENING_WITH_FLAG, argumentos[POSICAO_FLAG_OPERACAO]);
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
    char bufferParametro[256];
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
            int indice_espaco = IndexOf(buffer, ' ');

            memmove(&buffer[indice_espaco], &buffer[indice_espaco+1], strlen(buffer) - indice_espaco);
            
            for (int i = indice_espaco; i < posicaoCaractereAtual; i++)
            {
               bufferParametro[i - indice_espaco] = buffer[i];
            }

            pivot->operacao = buffer[0];

            strcpy(pivot->parametro_operacao, bufferParametro);
            posicaoCaractereAtual = 0;
            LimparBuffer(bufferParametro);
            pivot->proximo_processo = malloc(sizeof(struct Processo));
            pivot = pivot->proximo_processo;
            memset(buffer, '\0', sizeof(buffer));
            memset(bufferParametro, '\0', sizeof(bufferParametro));
            memset(bufferOperacao, '\0', sizeof(bufferOperacao));

        }
    }
    InserirInformacoesNoComando(pivot, buffer, bufferParametro, bufferOperacao, posicaoCaractereAtual);
    memset(buffer, '\0', sizeof(buffer));
    memset(bufferParametro, '\0', sizeof(bufferParametro));
    memset(bufferOperacao, '\0', sizeof(bufferOperacao));
    pivot->proximo_processo = NULL;
    return processo_final;
}

void InserirInformacoesNoComando(struct Processo* comando, char buffer[], char bufferParametro[], char bufferOperacao[256], int posicaoCaractereAtual)
{
    int indice_espaco = IndexOf(buffer, ' ');

    memmove(&buffer[indice_espaco], &buffer[indice_espaco+1], strlen(buffer) - indice_espaco);
    
    for (int i = indice_espaco; i < posicaoCaractereAtual; i++)
    {
        bufferParametro[i - indice_espaco] = buffer[i];
    }

    comando->operacao = buffer[0];

    strcpy(comando->parametro_operacao, bufferParametro);
    posicaoCaractereAtual = 0;
    LimparBuffer(bufferParametro);
    comando->proximo_processo = malloc(sizeof(struct Processo));
    comando = comando->proximo_processo;
}