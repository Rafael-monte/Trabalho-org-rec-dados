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
    short comp_reg;

    char linha[512];
    while (fgets(linha, 512, arquivoQueSeraCopiado) != NULL) {
        if(linha[strlen(linha) - 1] == '\n'){
            linha[strlen(linha) - 1] = '\0';
        }
        comp_reg = strlen(linha);
        char tamanho_em_string[10];
        char registro_final[512];
        strcpy(registro_final, itoa(comp_reg, tamanho_em_string, 10));
        strcat(registro_final, DELIM_STR);
        strcat(registro_final, linha);
        fputs(registro_final, arquivoFinal);
    }

    return arquivoFinal;
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
    FILE* arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "rb");
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

        if (pivot->operacao == 'i')
        {
            InserirRegistro(arquivoOrigem, pivot->parametro_operacao);
        }
        pivot = pivot->proximo_processo;
    }


}


int BuscarRegistro(FILE* arquivo_dados, char* id_reg)
{
    int offset = 0;
    char linha[512];
    char prev_line[512];
    
    char tam_reg[4];
    buscar_campo(tam_reg, 4, arquivo_dados);
    while(!feof(arquivo_dados)){
        offset = ftell(arquivo_dados);
        char reg[atoi(tam_reg)];
        fgets(reg, atoi(tam_reg), arquivo_dados);
        char * codigo_registro = buscar_codigo_registro(reg);
        if(atoi(codigo_registro) == atoi(id_reg)){
            printf("Registro encontrado: %s\n", reg);
            return offset;
        }
        buscar_campo(tam_reg, 4, arquivo_dados);
        
    }
    printf("Registro nao encontrado \n");
    return -1;
}


char* buscar_codigo_registro(char registro[512]){
    char auxiliar[512];
    strcpy(auxiliar, registro);
    char * codigo_registro = strtok(auxiliar, DELIM_STR);
    return codigo_registro;
}


int buscar_campo(char* campo, int tam, FILE* arquivo_dados){
    int i = 0;
    char ch = fgetc(arquivo_dados);
    while (ch != EOF && ch != '|')
    {
        if (i < tam-1) {
            campo[i] = ch;
            i++;
        }
        ch = fgetc(arquivo_dados);
    }
    campo[i] = '\0';
    return i;
}



int InserirRegistro(FILE* arquivo_dados, char registro[256])
{
    printf("Iniciando insercao do registro %s\n", registro);

    char *token = buscar_codigo_registro(registro);
    int offset = BuscarRegistro(arquivo_dados, token);
    if(offset == -1){
        printf("Adicionando registro no final do arquivo...\n");
        int comp_reg = strlen(registro);
        char tamanho_em_string[10];
        char registro_final[512];
        strcpy(registro_final, itoa(comp_reg, tamanho_em_string, 10));
        strcat(registro_final, DELIM_STR);
        strcat(registro_final, registro);
        fputs(registro_final, arquivo_dados);
    }else{
        printf("Codigo ja existe \n");
        printf("RRN: %i \n", offset);
        fseek(arquivo_dados, offset, SEEK_SET);
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
