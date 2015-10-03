/* 
 * File:   dados.c
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/valores.h"

//propagar retorno
//retornar tuplas gravadas
//param bloco

char* procurarEspaco(Tabela tabela, int tamanho) {
    int i = 0;
    char* nomeArq;
    while (tabela->nomesArquivosBlocos[i]) {
        nomeArq = tabela->nomesArquivosBlocos[i];
        FILE* file;
        file = fopen(nomeArq, "r+");
        short int qtdReg;
        fread(qtdReg, sizeof (short int), 1, file);
        fseek(file, 4, SEEK_SET);
        short int espacoLivre;
        fread(espacoLivre, sizeof (short int), 1, file);
        fclose(file);
        int inicio = qtdReg * 2;
        inicio += 6;
        if ((espacoLivre - inicio) >= tamanho) {
            return nomeArq;
        }
        i++;
    }
    //    nomeArq = tabela->nome;
    //    itoa(i, nomeArq, 10);
    //    gerarBloco()
}

int calcTamanhoInserir(Valores* valores) {
    int i, tamanho = 0;
    int qtdInt = 0;
    int qtdBoo = 0;
    int qtdVar = 0;
    int qtdCha = 0;
    Tipo tipo;
    for (i = 0; i < (valores->tabela->numeroCampos); i++) {
        tipo = (valores->tabela->campos[i]->tipo);
        if (tipo == INTEGER) {
            if (valores->integer[qtdInt] != NULL) {
                tamanho += 4;
            }
            qtdInt++;
        } else if (tipo == BOOLEAN) {
            if (valores->boolean[qtdBoo] != NULL) {
                tamanho += 1;
            }
            qtdBoo++;
        } else if (tipo == CHAR) {
            if (valores->chars[qtdCha] != NULL) {
                int j, h = 0;
                for (j = 0; j < valores->tabela->numeroCampos; j++) {
                    campo = valores->tabela->campos[j];
                    if (campo->tipo == CHAR) {
                        if (h == qtdCha) {
                            tamanho += campo->bytes;
                        }
                        h++;
                    }
                }
            }
            qtdCha++;
        } else if (tipo == VARCHAR) {
            if (valores->varchar[qtdVar] != NULL) {
                int j, h = 0;
                for (j = 0; j < valores->tabela->numeroCampos; j++) {
                    campo = tabela->campos[j];
                    if (campo->tipo == CHAR) {
                        if (h == qtdVar) {
                            tamanho += strlen(valores->varchar[qtdVar]);
                        }
                        h++;
                    }
                }
            }
            qtdVar++;
        }
    }
    return tamanho;
}

void inserirRegistro(Valores* valores) {
}