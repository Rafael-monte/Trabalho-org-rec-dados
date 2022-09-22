#include "logger.h"


void LogError(char* ERROR_TYPE, char* object)
{
    printf(ERROR_TYPE, object);
}

void LogRemocao(char* chaveRegistro, int bytes, int offset)
{
    printf("\nRegistro \"%s\" removido! (%d)\nLocal: offset = %d bytes (%x)\n", chaveRegistro, bytes, offset, offset);
}

void LogBusca(char* registro, int bytes)
{
    printf("\n%s (%d) bytes\n", registro, bytes);
}

void LogInsercao(char* chave, short bytes, int isFimArquivo)
{
    printf("\nInsercao do registro de chave \"%s\" (%d) bytes\n",chave, bytes);
    if (isFimArquivo)
    {
        printf("\nLocal: fim do arquivo\n");
    }
}