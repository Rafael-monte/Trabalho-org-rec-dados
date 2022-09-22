
#ifndef TRABALHO_ORG_E_REC_DADOS_FILE_HANDLER_H
#define TRABALHO_ORG_E_REC_DADOS_FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>

struct Game {
    short id;
    char* nomeJogo;
    char* anoLancamento;
    char* modalidade;
    char* empresa;
    char* plataforma;
    int comprimentoCampo;
    short comprimento_registro;
};


struct Led {
    int cabeca;
};


void HandleProcesso(struct Processo* processo);

void OperacoesEmLote(struct Processo* processo);

void ImportarRegistroEmArquivo(struct Processo* processo);

FILE* AbrirArquivo(char* nomeArquivo, char* modoAcesso);

FILE* CopiarConteudoDoArquivo(FILE* arquivoQueSeraCopiado, FILE* arquivoFinal);

void AdicionarCampoAoArquivo(char* campo, FILE* arquivoDestino);

void LimparBuffer(char buffer[]);

int BuscarRegistro(FILE* arquivo_dados, char* id_reg);

int InserirRegistro(FILE* arquivo_dados, char registro[256]);

int RemoverRegistro(FILE* arquivo_dados, char* id_reg);

void mudar_cabecalho_arquivo(int novo_valor_cabecalho);

int buscar_campo(char* campo, int tam, FILE* arquivo_dados);

char* buscar_codigo_registro(char registro[512]);


#endif //TRABALHO_ORG_E_REC_DADOS_FILE_HANDLER_H
