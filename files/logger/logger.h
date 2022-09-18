#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>

void LogError(char* ERROR_TYPE, char* object);

void LogRemocao(char* chaveRegistro, short bytes, short offset);

void LogBusca(char* registro, short bytes);

void LogInsercao(char* chave, short bytes, int isFimArquivo);


char* ERROR_OPENING_FILE="\033[0;31mErro: nao foi possivel abrir o arquivo %s\n";
char* REGISTER_NOT_FOUND="\033[0;31mErro: Registro [%s] nao encontrado!\n";
char* PROCESS_NOT_DEFINED="\033[0;31mErro: Processo [%s] nao encontrado!\n";

#endif