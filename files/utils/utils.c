#pragma once
#include "utils.h"


char* ConvertCharToString(char character)
{
    char* string = malloc(sizeof (char) * 2);
    string[0] = character;
    return string;
}


void RedefinirTamanhoString(char* string, int novoTamanho)
{
    string = malloc(novoTamanho);
}


