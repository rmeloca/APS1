/* 
 * File:   campo.c
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:40
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/campo.h"

Campo* criarCampo(char* nome, Tipo tipo, int bytes) {
    Campo* campo = (Campo*) malloc(sizeof (Campo));
    campo->nome = (char*) calloc(strlen(nome), sizeof (char));
    strcpy(campo->nome, nome);
    campo->tipo = tipo;
    campo->bytes = bytes;
    return campo;
}

int isCamposEquals(Campo* campo1, Campo* campo2) {
    return strcmp(campo1->nome, campo2->nome) == 0;
}

char* getTipo(Tipo tipo) {
    char* nomeTipo;
    if (tipo == INTEGER) {
        nomeTipo = (char*) calloc(7, sizeof (char));
        strcpy(nomeTipo, "INTEGER");
    } else if (tipo == BOOLEAN) {
        nomeTipo = (char*) calloc(7, sizeof (char));
        strcpy(nomeTipo, "BOOLEAN");
    } else if (tipo == CHAR) {
        nomeTipo = (char*) calloc(4, sizeof (char));
        strcpy(nomeTipo, "CHAR");
    } else if (tipo == VARCHAR) {
        nomeTipo = (char*) calloc(7, sizeof (char));
        strcpy(nomeTipo, "VARCHAR");
    }
    return nomeTipo;
}