/* 
 * File:   tabela.c
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:39
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/tabela.h"

Tabela* criarTabela(char* nome) {
    Tabela* tabela = (Tabela*) malloc(sizeof (Tabela));

    tabela->limiteCampos = MAXIMO_CAMPOS;
    tabela->limiteBlocos = MAXIMO_BLOCOS;

    tabela->campos = (Campo**) calloc(tabela->limiteCampos, sizeof (Campo*));
    tabela->nomesArquivosBlocos = (char**) calloc(tabela->limiteBlocos, sizeof (char*));
    tabela->nome = (char*) calloc(strlen(nome), sizeof (char));

    strcpy(tabela->nome, nome);

    tabela->numeroBlocos = 0;
    tabela->numeroCampos = 0;

    return tabela;
}

void adicionarCampo(Tabela* tabela, Campo* campo) {
    if (tabela->numeroCampos == tabela->limiteCampos) {
        //dobra tamanho do vetor e realoca
        return;
    }
    tabela->campos[tabela->numeroCampos] = campo;
    tabela->numeroCampos++;
}

void adicionarBloco(Tabela* tabela, char* nomeArquivo) {
    if (tabela->numeroBlocos == tabela->limiteBlocos) {
        //dobra tamanho do vetor e realoca
        return;
    }
    tabela->nomesArquivosBlocos[tabela->numeroBlocos] = (char*) calloc(strlen(nomeArquivo), sizeof (char));
    strcpy(tabela->nomesArquivosBlocos[tabela->numeroBlocos], nomeArquivo);
    tabela->numeroBlocos++;
}

Campo* getCampo(Tabela* tabela, char* nome) {
    Campo* campo;
    int i;
    for (i = 0; i < tabela->numeroCampos; i++) {
        campo = tabela->campos[i];
        if (!strcmp(campo->nome, nome)) {
            return campo;
        }
    }
    return NULL;
}