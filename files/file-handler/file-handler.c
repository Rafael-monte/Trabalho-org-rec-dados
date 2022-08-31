#pragma once
#include "file-handler.h"
#include "../flag-controller/flag-controller.c"
#include "../logger/logger.c"
#include "../utils/utils.c"


void HandleProcesso(struct Processo* processo) 
{
    if (processo->operacao == 'i')
    {
        ImportarRegistro(processo);
    }

    if (processo ->operacao == 'e') 
    {
        OperacoesEmLote(processo);
    }
    
    LogError(PROCESS_NOT_DEFINED, ConvertCharToString(processo->operacao));
}


void ImportarRegistro(struct Processo* processo) 
{
    FILE* arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "w");
    FILE* arquivoAdicional = AbrirArquivo(processo->nome_arq_busca, "r");

    printf("Pronto para realizar operacoes de operacao de registro!");
}

void OperacoesEmLote(struct Processo* processo)
{
    printf("Arquivo -> %s", processo->nome_arq_principal);
    FILE* arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "w");
    FILE* arquivoAdicional = AbrirArquivo(processo->nome_arq_busca, "r");

    printf("Pronto para realizar operacoes em lote!");
}


FILE* AbrirArquivo(char* nomeArquivo, char* modoAcesso) 
{
    FILE* arq = fopen(nomeArquivo, modoAcesso);
    if (arq == NULL)
    {
        LogError(ERROR_OPENING_FILE, nomeArquivo);
        exit(1);
    }
    return arq;
}