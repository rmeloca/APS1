/**********************************
Algoritmos e Estrutura de Dados 1
Prof. Rafael Liberato
-----------------------------------
Exercício: Leitor de Token
Aluno: Rômulo Manciola Meloca
Data: 07/11/14
 ***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Headers/token.h"

void setTokenString(TokenReader *reader, char* str) {
    reader->length = strlen(str);
    reader->pos = 0;
    reader->str = (char*) calloc(reader->length, sizeof (char));
    reader->str = str;
}

TokenReader* newTokenReader(char *str) {
    TokenReader *tokenReader = (TokenReader*) calloc(1, sizeof (TokenReader));
    setTokenString(tokenReader, str);
    return tokenReader;
}

void freeTokenReader(TokenReader *reader) {
    free(reader);
}

int hasMoreTokens(TokenReader *reader) {
    while (reader->str[reader->pos] == ' ') {
        reader->pos++;
    }
    return reader->pos < reader->length;
}

char* substring(char *str, int inicio, int fim) {
    char* sub = (char*) calloc(fim - inicio, sizeof (char));
    memcpy(sub, str + inicio, fim - inicio);
    return sub;
}

char* nextToken(TokenReader *reader) {
    if (!hasMoreTokens(reader)) {
        return NULL;
    }
    int i = reader->pos;
    char *token;
    while (isalnum(reader->str[i]) || reader->str[i] == '_') {
        i++;
    }
    if (i == reader->pos) {
        i++;
    }
    token = substring(reader->str, reader->pos, i);
    reader->pos = i;
    return token;
}