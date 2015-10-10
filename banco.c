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
    Tupla* tupla;
    Associacao* associacao;
    Campo* campo;
    char* nome;
    int i, j, k;

    printf("Tabelas: %d/%d\n", banco->numeroTabelas, banco->limiteTabelas);
    printf("Nome Arquivo Banco: %s\n", banco->nomeArquivoBanco);

    for (i = 0; i < banco->numeroTabelas; i++) {
        tabela = banco->tabelas[i];

        printf("---- %s ----\n", tabela->nome);
        printf("Campos: %d/%d\n", tabela->numeroCampos, tabela->limiteCampos);
        printf("Tuplas: %d/%d\n", tabela->numeroTuplas, tabela->limiteTuplas);
        printf("Blocos: %d/%d\n", tabela->numeroBlocos, tabela->limiteBlocos);

        for (j = 0; j < tabela->numeroBlocos; j++) {
            nome = tabela->nomesArquivosBlocos[j];
            printf("%s\n", nome);
        }
        printf("\n");

        for (j = 0; j < tabela->numeroCampos; j++) {
            campo = tabela->campos[j];
            printf("[%s %s(%d)]\n", campo->nome, getTipo(campo->tipo), campo->bytes);
        }

        printf("\n");

        for (j = 0; j < tabela->numeroTuplas; j++) {
            tupla = tabela->tuplas[j];
            for (k = 0; k < tabela->numeroCampos; k++) {
                associacao = tupla->associacoes[k];
                printf("%s: %s\n", associacao->campo->nome, (char*) associacao->valor);
            }
            printf("\n");
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