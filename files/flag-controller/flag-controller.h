#ifndef TRABALHO_ORG_E_REC_DADOS_FLAG_CONTROLLER_H
#define TRABALHO_ORG_E_REC_DADOS_FLAG_CONTROLLER_H

typedef struct Processo {
    char operacao;
    char* nome_arq_principal;
    char* nome_arq_busca;
};

const char* ARQUIVO_OPERACOES="-e";
const char* ARQUIVO_IMPORTACAO="-i";
const int POSICAO_FLAG_OPERACAO=1;
const int POSICAO_ARQUIVO_TARGET=2;
const char* NOME_ARQUIVO_PRINCIPAL="";


struct Processo* ProcessarFlagsIniciais(int numero_argumentos, char* argumentos[]);
struct Processo* ProcessarSubFlags(int numero_argumentos, char* argumentos[]);



#endif //TRABALHO_ORG_E_REC_DADOS_FLAG_CONTROLLER_H
