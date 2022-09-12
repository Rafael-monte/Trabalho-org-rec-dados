#pragma once
#include "file-handler.h"
#include "../flag-controller/flag-controller.c"
#include "../logger/logger.c"
#include "../utils/utils.c"
#define TAM_STR 90
#define TAM_MAX_REG 256
#define DELIM_STR "|"

void HandleProcesso(struct Processo* processo) 
{
    printf("Processo: %c\n", processo->operacao);

    if (processo->operacao == 'i')
    {
        ImportarRegistroEmArquivo(processo);
        return;
    }

    if (processo ->operacao == 'e') 
    {
        OperacoesEmLote(processo);
        return;
    }

    LogError(PROCESS_NOT_DEFINED, ConvertCharToString(processo->operacao));
}

// -i
void ImportarRegistroEmArquivo(struct Processo* processo) 
{
    FILE* arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "w+");
    FILE* arquivoAdicional = AbrirArquivo(processo->nome_arq_busca, "r+");
    printf("Iniciando importacao de registro!\n");
    CopiarConteudoDoArquivo(arquivoAdicional, arquivoOrigem);
    fclose(arquivoAdicional);
    fclose(arquivoOrigem);
}

FILE* CopiarConteudoDoArquivo(FILE* arquivoQueSeraCopiado, FILE* arquivoFinal)
{
    struct Game* games[256];
    char campo[TAM_STR];
    char buffer[TAM_MAX_REG];
    char charAtual;
    //final do registro -> '\n'
    // separador dos campos -> '|'
    // contar o tamanho do registro
    //1|The Legend of Zelda: Majora's Mask|2000|Action-adventure|Nintendo|Nintendo 64| (6 campos)

    int posicaoCaractereAtual=0;
    int posicaoAtual = 0;
    LimparBuffer(buffer);
    while((charAtual = fgetc(arquivoQueSeraCopiado)) != EOF) 
    {
        if (charAtual != '\n')
        {
           printf("%c", charAtual);
           buffer[posicaoCaractereAtual] = charAtual;
           buffer[posicaoCaractereAtual+1]='\0';
           posicaoCaractereAtual++;
        }
        else
        {
            printf("\nAchei um \\n!\n");
            struct Game* game = CriarGamePeloBuffer(buffer);
            games[posicaoAtual] = game;
            printf("\nGame criado! Nome: %s, data lancamento %s\n", game->nomeJogo, game->anoLancamento);
            posicaoAtual++;
            posicaoCaractereAtual = 0;
            LimparBuffer(buffer);
        }
    }

     for(int i = 0; i < sizeof(games)/sizeof(struct Game); i++)
     {
         printf("Nome jogo: %s, Ano de lancamento: %s\n", games[i]->nomeJogo, games[i]->anoLancamento);
     }
}


void LimparBuffer(char buffer[])
{
    for(int i = 0; i < strlen(buffer); i++)
    {
        buffer[i]='\0';
    }
}

struct Game* CriarGamePeloBuffer(char* buffer)
{
    printf("Analisando buffer...\n");
    printf("Valor do buffer: ");
    for (int i = 0; i < strlen(buffer); i++) {
        printf("%c", buffer[i]);
    }

    struct Game* game = malloc(sizeof(struct Game));
    //Abrindo espaço para as strings
    RedefinirTamanhoString(game->nomeJogo, TAM_STR);
    RedefinirTamanhoString(game->anoLancamento, TAM_STR);
    RedefinirTamanhoString(game->modalidade, TAM_STR);
    RedefinirTamanhoString(game->empresa, TAM_STR);
    RedefinirTamanhoString(game->plataforma, TAM_STR);

    int indexPrimeiroPipe = 0;
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == '|' && indexPrimeiroPipe == 0)
        {
            indexPrimeiroPipe = i;
            break;
        }
    }
    printf("\nIndex do pipe: %d\n", indexPrimeiroPipe);
    printf("\nComecando pelo caractere '%c'\n", buffer[indexPrimeiroPipe+1]);
    int valorCampo = 0;
    char bufferStrings[80];
    EsvaziarBufferStrings(bufferStrings);
    int indiceBufferStrings = 0;
    for (int i = indexPrimeiroPipe+1; i < strlen(buffer); i++)
    {
       if (buffer[i] == '|')
       {
            char caractereBufferStrings;
            printf("\nValor do buffer de strings atual: ");
            CopiarCampoPeloValor(game, bufferStrings, valorCampo);
            valorCampo++;
            indiceBufferStrings = 0;
            EsvaziarBufferStrings(bufferStrings);
            i++;
       }

       if (buffer[i] == '\0')
       {
            printf("Entrou no break do buffer!");
            break;
       }
        bufferStrings[indiceBufferStrings] = buffer[i];
        bufferStrings[indiceBufferStrings+1] = '\0';
        //printf("\nBuffer atual: %s\n", bufferStrings);
        //1|The Legend of Zelda: Majora's Mask|2000|Action-adventure|Nintendo|Nintendo 64| (6 campos)
        indiceBufferStrings++;
    }

    return game;
}


void EsvaziarBufferStrings(char bufferStrings[])
{
    for (int j = 0; j < strlen(bufferStrings); j++)
    {
        bufferStrings[j] = '\0';
    }
}

void CopiarCampoPeloValor(struct Game* game, char* bufferStrings, int valorCampo)
{
    switch(valorCampo)
    {
        case 0:
            printf("\nColocando o valor %s no nome do jogo\n", bufferStrings);
            if (!(game->nomeJogo = malloc(strlen(bufferStrings + 1))))
            {
                printf("ERRO: Nao foi possivel alocar espaco para o buffer de strings");
            }
            strcpy(game->nomeJogo, bufferStrings);
            break;
        case 1:
            printf("\nColocando o valor %s no ano de lancamento do jogo\n", bufferStrings);
            if (!(game->anoLancamento = malloc(strlen(bufferStrings + 1))))
            {
                printf("ERRO: Nao foi possivel alocar espaco para o buffer de strings");
            }
            strcpy(game->anoLancamento, bufferStrings);
            break;
        case 2:
            printf("\nColocando o valor %s no tipo de jogo\n", bufferStrings);
            if (!(game->modalidade = malloc(strlen(bufferStrings + 1))))
            {
                printf("ERRO: Nao foi possivel alocar espaco para o buffer de strings");
            }
            strcpy(game->modalidade, bufferStrings);
            break;
        case 3:
            printf("\nColocando o valor %s no nome da empresa do jogo\n", bufferStrings);
            if (!(game->empresa = malloc(strlen(bufferStrings + 1))))
            {
                printf("ERRO: Nao foi possivel alocar espaco para o buffer de strings");
            }
            strcpy(game->empresa, bufferStrings);
            break;
        case 4:
            printf("\nColocando o valor %s no nome da plataforma do jogo\n", bufferStrings);
            printf("Tamanho buffer strings: %d", strlen(bufferStrings+1));
            if (!(game->plataforma = malloc(strlen(bufferStrings + 1))))
            {
                printf("ERRO: Nao foi possivel alocar espaco para o buffer de strings para o tamanho %d", strlen(bufferStrings+1));
            }
            strcpy(game->plataforma, bufferStrings);
            break;
        default:
            LogError("Nao foi possivel descobrir o campo %s", bufferStrings);
            break;
    }
}

// -e
void OperacoesEmLote(struct Processo* processo)
{
    printf("Arquivo -> %s \n", processo->nome_arq_principal);
    FILE* arquivoOrigem = AbrirArquivo(processo->nome_arq_principal, "r+");
    FILE* arquivoAdicional = AbrirArquivo(processo->nome_arq_busca, "r+");
    printf("Iniciando operacoes em lote!");
}


FILE* AbrirArquivo(char* nomeArquivo, char* modoAcesso) 
{
    FILE* arq = fopen(nomeArquivo, modoAcesso);
    if (arq == NULL)
    {
        LogError(ERROR_OPENING_FILE, nomeArquivo);
        exit(1);
    }
    return arq;
}


short leia_reg(char * buffer, int tam, FILE * arq) {
    short comp_reg;
    printf("Tamanho: %d\n", tam);




    if (fread(&comp_reg, sizeof(comp_reg), 1, arq) == 0) {
        return 0;
    }

    printf("Comprimento Registro: %d\n", comp_reg);
    if (comp_reg < tam) {
        fread(buffer, sizeof(char), comp_reg, arq);
        buffer[comp_reg] = '\0';
        return comp_reg;
    } else {
        printf("Buffer overflow\n");
        return 0;
    }
}