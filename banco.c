/* 
 * File:   dados.c
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/banco.h"

Banco* criarBanco(char* nomeArquivoBanco) {
    Banco* banco = (Banco*) malloc(sizeof (Banco));

    banco->limiteTabelas = MAXIMO_TABELAS;

    banco->tabelas = (Tabela**) calloc(banco->limiteTabelas, sizeof (Tabela*));
    banco->nomeArquivoBanco = (char*) calloc(strlen(nomeArquivoBanco), sizeof (char));

    strcpy(banco->nomeArquivoBanco, nomeArquivoBanco);

    banco->numeroTabelas = 0;
    return banco;
}

void adicionarTabela(Banco* banco, Tabela* tabela) {
    if (banco->numeroTabelas == banco->limiteTabelas) {
        //dobra tamanho do vetor e realoca
        return;
    }
    banco->tabelas[banco->numeroTabelas] = tabela;
    banco->numeroTabelas++;
}

void imprimirBanco(Banco* banco) {
    Tabela* tabela;
    Campo* campo;
    printf("Número Tabelas: %d\n", banco->numeroTabelas);
    printf("Limite Tabelas: %d\n", banco->limiteTabelas);
    int i, j;
    for (i = 0; i < banco->numeroTabelas; i++) {
        tabela = banco->tabelas[i];
        printf("----%s----\n", tabela->nome);
        printf("Número Campos: %d\n", tabela->numeroCampos);
        printf("Limite Campos: %d\n", tabela->limiteCampos);
        for (j = 0; j < tabela->numeroCampos; j++) {
            campo = tabela->campos[j];
            printf("\n");
            printf("Campo: %s\n", campo->nome);
            printf("Tipo: %d\n", campo->tipo);
            printf("Bytes: %d\n", campo->bytes);
        }
    }
}

Tabela* getTabela(Banco* banco, char* nome) {
    Tabela* tabela;
    int i;
    for (i = 0; i < banco->numeroTabelas; i++) {
        tabela = banco->tabelas[i];
        if (!strcmp(tabela->nome, nome)) {
            return tabela;
        }
    }
    return NULL;
}