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

    tabela->nome = (char*) calloc(strlen(nome), sizeof (char));
    strcpy(tabela->nome, nome);

    tabela->limiteBlocos = MAXIMO_BLOCOS;
    tabela->nomesArquivosBlocos = (char**) calloc(tabela->limiteBlocos, sizeof (char*));
    tabela->numeroBlocos = 0;

    tabela->limiteCampos = MAXIMO_CAMPOS;
    tabela->campos = (Campo**) calloc(tabela->limiteCampos, sizeof (Campo*));
    tabela->numeroCampos = 0;

    tabela->limiteTuplas = MAXIMO_TUPLAS;
    //nota-se que ao persistir o arquivo auxiliar, inclui-se as tuplas inicializadas
    tabela->tuplas = (Tupla**) calloc(tabela->limiteTuplas, sizeof (Tupla*));
    tabela->numeroTuplas = 0;
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

Tupla* criarTupla(Tabela* tabela) {
    Campo* campo;
    Tupla* tupla = (Tupla*) malloc(sizeof (Tupla));
    tupla->associacoes = (Associacao**) calloc(tabela->numeroCampos, sizeof (Associacao*));
    int i;
    //cria tupla ordenado segundo os campos da tabela
    for (i = 0; i < tabela->numeroCampos; i++) {
        campo = tabela->campos[i];
        tupla->associacoes[i] = criarAssociacao(campo);
    }

    return tupla;
}

void adicionarTupla(Tabela* tabela, Tupla* tupla) {
    if (tabela->numeroTuplas == tabela->limiteTuplas) {
        return;
    }
    tabela->tuplas[tabela->numeroTuplas] = tupla;
    tabela->numeroTuplas++;
}

Associacao* criarAssociacao(Campo* campo) {
    Associacao* associacao = (Associacao*) malloc(sizeof (Associacao));
    //observa-se que não pede-se memória para campo, uma vez que ele é apenas uma referência ao item já persistido
    associacao->campo = campo;
    return associacao;
}

Associacao* findAssociacao(Tabela* tabela, Tupla* tupla, Campo* campo) {
    int i;
    Associacao* associacao;
    for (i = 0; i < tabela->numeroCampos; i++) {
        associacao = tupla->associacoes[i];
        if (isCamposEquals(associacao->campo, campo)) {
            return associacao;
        }
    }
    return associacao;
}

void associarValor(Associacao* associacao, void* valor) {
    int tamanho;
    if (associacao->campo->tipo == VARCHAR) {
        tamanho = strlen((char*) valor);
        if (tamanho > associacao->campo->bytes) {
            return;
        }
    } else {
        tamanho = associacao->campo->bytes;
    }
    associacao->valor = malloc(tamanho);
    associacao->valor = valor;
}

void imprimirTabela(Tabela* tabela) {
    printf("%s\n", tabela->nome);
}

void freeTuplas(Tabela* tabela) {
    int i;
    int j;
    Tupla* tupla;
    Associacao* associacao;
    //cada tupla
    for (i = 0; i < tabela->numeroTuplas; i++) {
        tupla = tabela->tuplas[i];
        //cada associacao
        for (j = 0; j < tabela->numeroCampos; j++) {
            associacao = tupla->associacoes[i];
            //observa-se que não libera-se o espaço dos campos
            free(associacao->valor);
            free(associacao);
        }
        free(tupla);
    }
    tabela->numeroTuplas = 0;
}

void freeTupla(Tabela* tabela, Tupla* tupla) {
    Associacao* associacao;
    int i;
    for (i = 0; i < tabela->numeroCampos; i++) {
        associacao = tupla->associacoes[i];
        //observa-se que não libera-se o espaço dos campos
        free(associacao->valor);
        free(associacao);
    }
    free(tupla);
    tabela->numeroTuplas--;
}