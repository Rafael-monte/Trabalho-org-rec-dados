#pragma once
#ifndef FLAG_CONTROLLER_H
#define FLAG_CONTROLLER_H

struct Processo {
    char operacao;
    char* nome_arq_principal;
    char* nome_arq_busca;
};

const char* ARQUIVO_OPERACOES="-e";
const char* ARQUIVO_IMPORTACAO="-i";
const int POSICAO_FLAG_OPERACAO=1;
const int POSICAO_ARQUIVO_TARGET=2;
const char* NOME_ARQUIVO_PRINCIPAL="games.txt";

struct Processo* ProcessarFlagsIniciais(int numero_argumentos, char* argumentos[]);
struct Processo* ProcessarSubFlags(int numero_argumentos, char* argumentos[]);



#endif //FLAG_CONTROLLER_H
