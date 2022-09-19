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


int IndexOf(char* string, char character)
{
    int index = -1;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == character)
        {
            index = i;
            break;
        }
    }

    return index;
    
}


