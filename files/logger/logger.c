#include "logger.h"


void LogError(char* ERROR_TYPE, char* object)
{
    printf(ERROR_TYPE, object);
}

void LogRemocao(char* chaveRegistro, short bytes, short offset)
{
    printf("");
}

void LogBusca(char* registro, short bytes)
{
    printf("");
}

void LogInsercao(char* chave, short bytes, int isFimArquivo)
{
    printf("");
}