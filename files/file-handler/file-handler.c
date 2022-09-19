#pragma once
#include "file-handler.h"
#include <stdlib.h>
#include "../flag-controller/flag-controller.c"
#include "../logger/logger.c"
#include "../utils/utils.c"
#define TAM_STR 90
#define TAM_MAX_REG 256
#define DELIM_STR "|"

void HandleProcesso(struct Processo* processo) 
{
    printf("Processo: %c\n", processo->operacao);

    if (processo->operacao == 'i')
    {
        ImportarRegistroEmArquivo(processo);
        return;
    }

    if (processo ->operacao == 'e') 
    {
        OperacoesEmLote(processo);
        return;
    }

    LogError(PROCESS_NOT_DEFINED, ConvertCharToString(processo->operacao));
}

// -i
void ImportarRegistroEmArquivo(struct Processo* processo) 
{
    FILE* arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "wb");
    FILE* arquivoAdicional = AbrirArquivo(processo->nome_arq_busca, "r+");
    printf("Iniciando importacao de registro!\n");
    CopiarConteudoDoArquivo(arquivoAdicional, arquivoOrigem);
    printf("Importacao de arquivo finalizada!");
    fclose(arquivoAdicional);
    fclose(arquivoOrigem);
}

FILE* CopiarConteudoDoArquivo(FILE* arquivoQueSeraCopiado, FILE* arquivoFinal)
{
    char campo[TAM_STR];
    char buffer[TAM_MAX_REG];
    char charAtual;
    int posicaoCaractereAtual=0;
    int posicaoAtual = 0;
    LimparBuffer(buffer);
    while((charAtual = fgetc(arquivoQueSeraCopiado)) != EOF) 
    {
        if (charAtual != '\n')
        {
           buffer[posicaoCaractereAtual] = charAtual;
           buffer[posicaoCaractereAtual+1]='\0';
           posicaoCaractereAtual++;
        }
        else
        {
            char registroFinal[2056];
            short tamanho_registro = strlen(buffer);
            char tamEmString[9];
            itoa(tamanho_registro, tamEmString, 10);
            strcpy(registroFinal, tamEmString);
            // strcat(registroFinal, "|");
            strcat(registroFinal, buffer);
            fputs(registroFinal, arquivoFinal);
            posicaoAtual++;
            posicaoCaractereAtual = 0;
            LimparBuffer(buffer);
        }
    }
    char registroFinal[2056];
    short tamanho_registro = strlen(buffer);
    char tamEmString[9];
    itoa(tamanho_registro, tamEmString, 10);
    strcpy(registroFinal, tamEmString);
    // strcat(registroFinal, "|");
    strcat(registroFinal, buffer);
    fputs(registroFinal, arquivoFinal);
    posicaoAtual++;
    posicaoCaractereAtual = 0;
    LimparBuffer(buffer);
}


void LimparBuffer(char buffer[])
{
    for(int i = 0; i < strlen(buffer); i++)
    {
        buffer[i]='\0';
    }
}

// -e
void OperacoesEmLote(struct Processo* processo)
{
    printf("Arquivo -> %s \n", processo->nome_arq_principal);
    printf("Arquivo secundario ->%s\n", processo->nome_arq_busca);
    FILE* arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "r+");
    FILE* arquivo_comandos = AbrirArquivo(processo->nome_arq_busca, "r+");
    printf("Iniciando operacoes em lote!\n\n");
    struct Processo* comandos = ProcessarArquivoComandos(arquivo_comandos);
    struct Processo* pivot = comandos;


    while (pivot != NULL)
    {
        if (pivot->operacao == 'b')
        {
            BuscarRegistro(arquivoOrigem, pivot->parametro_operacao);
        }
        
        
        pivot = pivot->proximo_processo;
    }


}


void BuscarRegistro(FILE* arquivo_dados, char* id_reg)
{
    printf("Id buscado:%s\n", id_reg);

    char buffer[500];
    char* id_atual;
    int achou = 0;
    short comp_reg;
    comp_reg = leia_reg(buffer, 500, arquivo_dados);


    while(!achou && comp_reg > 0)
    {
        id_atual = strtok(buffer, "|");
        printf("Id atual:%s\n",id_atual);
        if (strcmp(id_atual, id_reg) == 0)
        {
            achou = 1;
        } 
        else 
        {
            comp_reg = leia_reg(buffer, 500, arquivo_dados);
        }
    }


    if (achou)
    {
        printf("Id encontrado!\n");
    }
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


short leia_reg(char * buffer, int tam, FILE* arq){
    short comp_reg;

    if (fread(&comp_reg, sizeof(comp_reg), 1, arq) == 0) {
        return 0;
    }

    if (comp_reg < tam) {
        comp_reg = fread(buffer, sizeof(char), comp_reg, arq);
        buffer[comp_reg] = '\0';
        return comp_reg;
    } else {
        printf("Buffer overflow\n");
        return 0;
    }
}
