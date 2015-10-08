/* 
 * File:   dados.c
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Headers/inserir.h"

//propagar retorno
//retornar tuplas gravadas
//param bloco

//Falta procurar espaço dos excluidos

int calcTamanhoInserir(Tupla *tupla, int qtdAss) {
    int i, tamanho = 0;
    Associacao *associacao;
    //percorre todas associacoes
    for (i = 0; i < qtdAss; i++) {
        associacao = tupla->associacoes[i];

        if (associacao->campo->tipo == INTEGER) {
            tamanho += associacao->campo->bytes;
        } else if (associacao->campo->tipo == BOOLEAN) {
            tamanho += associacao->campo->bytes;
        } else if (associacao->campo->tipo == CHAR) {
            tamanho += associacao->campo->bytes;
        } else {
            char* varchar = associacao->valor;
            if (varchar != NULL) {
                tamanho += strlen(varchar);
            }
        }

    }

    //soma o tamnho do mapa de bits
    if ((qtdAss % 8) != 0) {
        tamanho++;
    }
    tamanho += qtdAss / 8;

    return tamanho;
}

int inserirRegistro(Banco* banco) {
}

char** procurarEspaco(Tabela *tabela, int tamanho) {
}

