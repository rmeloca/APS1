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
    int i, j, k;
    //Percorrer todo banco para verirficar todas tuplas a ser inseridas
    for (i = 0; banco->numeroTabelas > i; i++) {
        Tabela *tabela = banco->tabelas[i];
        for (j = 0; tabela->numeroTuplas > j; j++) {
            Tupla *tupla = tabela->tuplas[j];

            //mapa de bits
            int* mapaBits;
            mapaBits = calloc(((tabela->numeroCampos / 8) + 1), sizeof (int));

            int tamanho = calcTamanhoInserir(tupla, tabela->numeroCampos);
            int numeroDoBloco = obterBloco(tamanho, tabela->nomesArquivosBlocos, tabela->numeroBlocos, tabela->nome);
            if (numeroDoBloco != tabela->numeroBlocos) {
                tabela->numeroBlocos++;
            }

            int qtdRegistros, comecoRegistro, espacoLivre, novoEspacoLivre, qtdVar = -1, tamanhoVar, comecoVar = 0;

            FILE* file;
            file = fopen(tabela->nomesArquivosBlocos[numeroDoBloco], "w+");
            //Le no arq a qtd de registros no bloco
            fread(&qtdRegistros, sizeof (short int), 1, file);
            //Le a posicao do comeco do espaco livre
            fseek(file, 4, SEEK_SET);
            fread(&espacoLivre, sizeof (short int), 1, file);
            fseek(file, 4, SEEK_SET);
            //Escreve o novo espaco livre
            novoEspacoLivre = (espacoLivre - tamanho) - 1;
            fwrite(&novoEspacoLivre, sizeof (short int), 1, file);
            //Escreve ponteiro para o comeco do novo registro
            comecoRegistro = (espacoLivre - tamanho);
            fseek(file, (6 + (qtdRegistros * 2)), SEEK_SET);
            fwrite(&comecoRegistro, sizeof (short int), 1, file);

            //Insere o registro

            for (k = 0; tabela->numeroCampos > k; k++) {
                qtdVar++;
                Associacao* associacao = tupla->associacoes[k];
                Campo* campo = associacao->campo;
                if (campo->tipo == VARCHAR) {
                    tamanhoVar = 0;
                    if (associacao->valor) {
                        tamanho = strlen(associacao->valor);
                    }
                    if (comecoVar == 0) {
                        comecoVar = (tamanho - tamanhoVar) + 1;
                    } else {
                        comecoVar -= tamanhoVar;
                    }
                    //insere ponteiro e tamanho do varchar
                    comecoRegistro += qtdVar * 4;
                    fseek(file, comecoRegistro, SEEK_SET);
                    fwrite(&comecoVar, sizeof (short int), 1, file);
                    fwrite(&tamanhoVar, sizeof (short int), 1, file);
                    //inseri o varchar na posicao correta
                    fseek(file, comecoVar, SEEK_SET);
                    fwrite(&associacao->valor, tamanhoVar, 1, file);
                    setMapaBits(k, mapaBits);
                }

            }

            for (k = 0; tabela->numeroCampos > k; k++) {
                Associacao* associacao = tupla->associacoes[k];
                Campo* campo = associacao->campo;
                fseek(file, comecoRegistro, SEEK_SET);
                if ((campo->tipo) == INTEGER) {
                    fwrite(&(associacao->valor), sizeof (int), 1, file);
                } else if ((campo->tipo) == CHAR) {
                    fwrite(&(associacao->valor), sizeof (char), campo->bytes, file);
                } else if ((campo->tipo) == BOOLEAN) {
                    fwrite(&(associacao->valor), sizeof (char), 1, file);
                }
                setMapaBits(k, mapaBits);
            }
        }
    }
}

void setMapaBits(int posicao, int* mapaBits) {
    mapaBits[(posicao + 1) / 8] = pow(2,(posicao % 8));
}
//Devolve o numero do bloco para insercao

int obterBloco(int tamanho, char** nomesArquivosBlocos, int numeroBlocos, char* nomeTabela) {

    short int qtdRegistros, iniEspacoLivre, fimEspacoLivre;

    FILE *file;
    file = fopen(nomesArquivosBlocos[numeroBlocos], "r+");
    fread(&qtdRegistros, sizeof (short int), 1, file);

    fseek(file, 4, SEEK_SET);
    fread(&iniEspacoLivre, sizeof (short int), 1, file);

    fimEspacoLivre = (qtdRegistros * 2) + 6;

    if ((iniEspacoLivre - fimEspacoLivre) >= tamanho) {
        return numeroBlocos;
    }
    numeroBlocos++;
    char* temp = malloc(sizeof (char*));
    sprintf(temp, "%d", numeroBlocos);
    char* nomeArq = nomeTabela;
    strcat(nomeArq, temp);
    gerarBloco(nomeArq);
    return numeroBlocos;
}
