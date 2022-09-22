#pragma once
#include "file-handler.h"
#include <stdlib.h>
#include "../flag-controller/flag-controller.c"
#include "../logger/logger.c"
#include "../utils/utils.c"
#define TAM_STR 90
#define TAM_MAX_REG 256
#define DELIM_STR "|"



typedef struct offset_handler offsetHandler;
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

    if (processo->operacao == 'e') 
    {
        OperacoesEmLote(processo);
        return;
    }

    if (processo->operacao == 'p')
    {
        FILE* arquivo = AbrirArquivo("dados.dat", "rb+");
        ImprimirCabecaLED(arquivo);
        fclose(arquivo);
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
            LogBusca(reg, strlen(reg));
            return offset;
        }
        buscar_campo(tam_reg, 4, arquivo_dados);
    }
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
    if (offset != -1)
    {
        LogError(REG_ALREADY_EXISTS, token);
        return -1;
    }

    offsetHandler melhor_encaixe = best_fit(strlen(registro), arquivo_dados);

    int comp_reg = strlen(registro);
    char tamanho_em_string[10];
    char registro_com_tamanho[512];
    strcpy(registro_com_tamanho, itoa(comp_reg, tamanho_em_string, 10));
    strcat(registro_com_tamanho, "|");
    strcat(registro_com_tamanho, registro);

    if(melhor_encaixe.offset == -1)
    {
        fseek(arquivo_dados, 0, SEEK_END);
        fputs(registro_com_tamanho, arquivo_dados);
        LogInsercao(token, strlen(registro_com_tamanho), (melhor_encaixe.offset == -1));
    }
    else
    {
        int offset_prox_reg = melhor_encaixe.offset  + sizeof(melhor_encaixe.tamanho) - 1;
        fseek(arquivo_dados, offset_prox_reg, SEEK_SET);
        for(int i = 0; i < melhor_encaixe.tamanho - 1; i++)
        {
            if(i >= strlen(registro))
            {
                fputc('#', arquivo_dados);
            }
            else
            {
                fputc(registro[i], arquivo_dados);
            }
        }
        fputc('|', arquivo_dados);
        if(melhor_encaixe.offset_anterior == 0)
        {
            mudar_cabecalho_arquivo(melhor_encaixe.prox_offset);
        }
        else
        {
            fseek(arquivo_dados, melhor_encaixe.offset_anterior, SEEK_SET);
            char tam_reg_offset_ant[4];
            char novo_offset[10];
            buscar_campo(tam_reg_offset_ant, 4, arquivo_dados);
            char offset_prox[10];
            strcpy(novo_offset, itoa(melhor_encaixe.prox_offset, offset_prox, 10));
            strcat(novo_offset, DELIM_STR);
            LogInsercao(token, atoi(novo_offset), 0);
            fputs(novo_offset, arquivo_dados);
        }
    }
}



int RemoverRegistro(FILE* arquivo_dados, char* id_reg)
{
    int offset = BuscarRegistro(arquivo_dados, id_reg);
    if(offset == -1){
       LogError(REGISTER_NOT_FOUND, id_reg);
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
    char tam_reg[4];
    char prox_offset[10];
    
    offsetHandler reg_atual;

    reg_atual.offset = led->cabeca;
    reg_atual.offset_anterior = 0;

    if(reg_atual.offset == -1){
        printf("LED: [-1]\n");
        return;
    }

    while(reg_atual.offset != -1 && reg_atual.offset != 0){
        printf("Offset: %d\n", reg_atual.offset);
        fseek(arquivo_dados, reg_atual.offset, SEEK_SET);
        buscar_campo(tam_reg, 4, arquivo_dados);
        fseek(arquivo_dados, 1, SEEK_CUR);
        buscar_campo(prox_offset, 10, arquivo_dados);
        reg_atual.offset_anterior = reg_atual.offset;
        reg_atual.offset = atoi(prox_offset);
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


struct offset_handler best_fit(int tam_registro_atual, FILE* arquivo_dados)
{
    int offset = -1;
    char tam_reg[4];
    char prox_offset[10];
    offsetHandler melhor_local;

    melhor_local.offset = led->cabeca;
    melhor_local.offset_anterior = 0;

    while(melhor_local.offset != -1){
        fseek(arquivo_dados, melhor_local.offset, SEEK_SET);
        buscar_campo(tam_reg, 4, arquivo_dados);
        fseek(arquivo_dados, 1, SEEK_CUR);
        buscar_campo(prox_offset, 10, arquivo_dados);

        if(atoi(tam_reg) >= tam_registro_atual){
            melhor_local.tamanho = atoi(tam_reg);
            melhor_local.prox_offset = atoi(prox_offset);
            break;
        }

        melhor_local.offset_anterior = melhor_local.offset;
        melhor_local.offset = atoi(prox_offset);
    }
    return melhor_local;
}
