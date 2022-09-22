#pragma once
#include "file-handler.h"
#include <stdlib.h>
#include "../flag-controller/flag-controller.c"
#include "../logger/logger.c"
#include "../utils/utils.c"
#define TAM_STR 90
#define TAM_MAX_REG 256
#define DELIM_STR "|"

typedef struct Led LED;
LED* led;

void HandleProcesso(struct Processo* processo) 
{
    led = malloc(sizeof(LED));
    led->cabeca = -1;
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
    fwrite(&led->cabeca, sizeof(int), 1, arquivoOrigem);
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
    FILE* arquivoOrigem;
    FILE* arquivo_comandos = AbrirArquivo(processo->nome_arq_busca, "r+");
    printf("Iniciando operacoes em lote!\n\n");
    struct Processo* comandos = ProcessarArquivoComandos(arquivo_comandos);
    struct Processo* pivot = comandos;
    

    while (pivot != NULL)
    {
        if (pivot->operacao == 'b')
        {
            arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "rb");
            BuscarRegistro(arquivoOrigem, pivot->parametro_operacao);
        }

        if (pivot->operacao == 'i')
        {
            arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "rb+");
            fread(&led->cabeca, sizeof(int), 1, arquivoOrigem);
            InserirRegistro(arquivoOrigem, pivot->parametro_operacao);
        }


        if (pivot->operacao == 'r')
        {
            arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "rb+");
            fread(&led->cabeca, sizeof(int), 1, arquivoOrigem);
            RemoverRegistro(arquivoOrigem, pivot->parametro_operacao);
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
            LogBusca(reg, strlen(reg));
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
    char *token = buscar_codigo_registro(registro);
    int offset = BuscarRegistro(arquivo_dados, token);
    if(offset == -1){
        printf("Local: Final do arquivo...\n");
        fseek(arquivo_dados, 0, SEEK_END);
        int comp_reg = strlen(registro);
        char tamanho_em_string[10];
        char registro_final[512];
        strcpy(registro_final, itoa(comp_reg, tamanho_em_string, 10));
        strcat(registro_final, DELIM_STR);
        strcat(registro_final, registro);
        fputs(registro_final, arquivo_dados);
        LogInsercao(token, strlen(registro), (offset==-1));
    }else{
        printf("Codigo ja existe \n");
        printf("RRN: %i \n", offset);
    }
    fseek(arquivo_dados, offset, SEEK_SET);
}



int RemoverRegistro(FILE* arquivo_dados, char* id_reg)
{
    printf("Remocao do registro de chave \"%s\"\n", id_reg);
    int offset = BuscarRegistro(arquivo_dados, id_reg);
    if(offset == -1){
        printf("Registro nao encontrado \n");
    }else{

        char tamanho_registro[4];
        fseek(arquivo_dados, offset, SEEK_SET);
        buscar_campo(tamanho_registro, 4, arquivo_dados);
        fseek(arquivo_dados, offset + strlen(tamanho_registro) + 1, SEEK_SET);
        char cabeca_led[10];
        strcpy(cabeca_led, itoa(led->cabeca, cabeca_led, 10));
        strcat(cabeca_led, DELIM_STR);
        fputc('*', arquivo_dados);
        fputs(cabeca_led, arquivo_dados);
        LogRemocao(id_reg, offset, offset);
        mudar_cabecalho_arquivo(offset);
    }
}


void mudar_cabecalho_arquivo(int novo_valor_cabecalho)
{
    FILE *arq = AbrirArquivo("dados.dat", "rb+");
    fseek(arq, 0, SEEK_SET);
    fwrite(led, sizeof(LED), 1, arq);
    fclose(arq);
}


void ImprimirCabecaLED(FILE* arquivo_dados)
{
    if (led->cabeca == -1)
    {
        printf("LED: [-1]\n");
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
