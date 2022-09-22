#include "logger.h"


void LogError(char* ERROR_TYPE, char* object)
{
    printf(ERROR_TYPE, object);
    ClearColor();
}

void LogRemocao(char* chaveRegistro, int bytes, int offset)
{
    printf("\n\033[0;33mRegistro \"%s\" removido! (%d)\nLocal: offset = %d bytes (%x)\n", chaveRegistro, bytes, offset, offset);
    ClearColor();
}

void LogBusca(char* registro, int bytes)
{
    printf("\n\033[0;33m%s (%d) bytes\n", registro, bytes);
    ClearColor();
}

void LogInsercao(char* chave, short bytes, int isFimArquivo)
{
    printf("\n\033[0;33mInsercao do registro de chave \"%s\" (%d) bytes\n",chave, bytes);
    if (isFimArquivo)
    {
        printf("\n\033[0;33mLocal: fim do arquivo\n");
    }
    ClearColor();
}

void ClearColor()
{
    printf("\033[0;37m");
}