
#ifndef TRABALHO_ORG_E_REC_DADOS_FILE_HANDLER_H
#define TRABALHO_ORG_E_REC_DADOS_FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>

void HandleProcesso(struct Processo* processo);

void OperacoesEmLote(struct Processo* processo);

void ImportarRegistro(struct Processo* processo);

FILE* AbrirArquivo(char* nomeArquivo, char* modoAcesso);


#endif //TRABALHO_ORG_E_REC_DADOS_FILE_HANDLER_H
