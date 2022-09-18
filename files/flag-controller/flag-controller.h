#pragma once
#ifndef FLAG_CONTROLLER_H
#define FLAG_CONTROLLER_H
#include <stdio.h>

struct Processo {
    char operacao;
    char nome_arq_principal[256];
    char nome_arq_busca[256];
    char parametro_operacao[256];
    struct Processo* proximo_processo;
};

const char* ARQUIVO_OPERACOES="-e";
const char* ARQUIVO_IMPORTACAO="-i";
const int POSICAO_FLAG_OPERACAO=1;
const int POSICAO_ARQUIVO_TARGET=2;
const char* NOME_ARQUIVO_PRINCIPAL="dados.dat\0";

struct Processo* ProcessarFlagsIniciais(int numero_argumentos, char* argumentos[]);
struct Processo* ProcessarSubFlags(int numero_argumentos, char* argumentos[]);
struct Processo* ProcessarArquivoComandos(FILE* arquivo_comandos);


#endif //FLAG_CONTROLLER_H
