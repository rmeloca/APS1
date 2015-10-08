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