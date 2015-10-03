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

//Falta procurar espaço dos excluidos

char** procurarEspaco(Tabela *tabela, int tamanho) {
    char** retorno;
    short int espacoLivre;
    retorno = calloc(2, sizeof (char));
    retorno[0] = malloc(sizeof (short int));
    retorno[1] = malloc(sizeof (char*));

    int i = 0;
    char* nomeArq;
    while ((tabela->nomesArquivosBlocos[i])) {
        nomeArq = tabela->nomesArquivosBlocos[i];
        FILE* file;
        file = fopen(nomeArq, "r+");
        short int qtdReg = 0;
        fread(&qtdReg, sizeof (short int), 1, file);
        fseek(file, 4, SEEK_SET);
        fread(&espacoLivre, sizeof (short int), 1, file);
        fclose(file);
        int inicio = qtdReg * 2;
        inicio += 6;
        if ((espacoLivre - inicio) >= tamanho) {
            itoa(espacoLivre, retorno[0]);
            retorno[1] = nomeArq;
            return retorno;
        }
        i++;
    }
    nomeArq = tabela->nome;

    char* temp = malloc(sizeof (char*));
    itoa(i, temp, 10);
    strcat(nomeArq, temp);
    gerarBloco(nomeArq);
    itoa(espacoLivre, retorno[0]);
    retorno[1] = nomeArq;
    return retorno;
}

int calcTamanhoInserir(Valores *valores) {
    int i, tamanho = 0;
    int qtdInt = 0;
    int qtdBoo = 0;
    int qtdVar = 0;
    int qtdCha = 0;
    Tipo tipo;
    Campo *campo;
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
                    campo = valores->tabela->campos[j];
                    if (campo->tipo == CHAR) {
                        if (h == qtdVar) {
                            tamanho += strlen(valores->varchar[qtdVar]);
                            tamanho += 4;
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

int inserirRegistro(Valores *valores) {
    int tamanho, posicao;
    int qtdReg = 0;
    char** destino;
    char* nomeArq;
    
    tamanho = calcTamanhoInserir(valores);
    
    if (tamanho > 1992) {
        return -1;
    }
    destino = procurarEspaco(valores->tabela, tamanho);
    nomeArq = destino[1];
    posicao =atoi(destino[0]);
    
    FILE* file;
    file = fopen(nomeArq, "r+");
    fread(&qtdReg, sizeof (short int), 1, file);
    qtdReg++;

    fseek(file, 4, SEEK_SET);

    return 1;
}