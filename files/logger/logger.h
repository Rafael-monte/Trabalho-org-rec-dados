#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>

void LogError(char* ERROR_TYPE, char* object);

void LogRemocao(char* chaveRegistro, int bytes, int offset);

void LogBusca(char* registro, int bytes);

void LogInsercao(char* chave, int bytes, int isFimArquivo);

void ClearColor();


char* ERROR_OPENING_FILE="\033[0;31mErro: nao foi possivel abrir o arquivo %s\n";
char* ERROR_OPENING_WITH_FLAG="\033[0;31mErro: nao foi possivel abrir o arquivo com a flag \"%s\"\n";
char* REGISTER_NOT_FOUND="\033[0;31mErro: Registro [%s] nao encontrado!\n";
char* PROCESS_NOT_DEFINED="\033[0;31mErro: Processo [%s] nao encontrado!\n";
char* REG_ALREADY_EXISTS="\033[0;31mErro: o registro [%s] ja existe\n";

#endif