/* 
 * File:   dados.c
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

            int tamanho = calcTamanhoInserir(tupla, tabela->numeroCampos);
            int numeroDoBloco = obterBloco(tupla, tamanho, tabela->nomesArquivosBlocos, tabela->numeroBlocos, tabela->nome);
            if (numeroDoBloco != tabela->numeroBlocos) {
                tabela->numeroBlocos++;
            }

            int qtdRegistros, comecoRegistro, espacoLivre, novoEspacoLivre, qtdVar = 0, tamanhoVar;

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
                Associacao* associacao = tupla->associacoes[k];
                Campo* campo = associacao->campo;
                if (campo->tipo == VARCHAR) {
                    tamanhoVar = 0;
                    qtdVar++;
                    if (associacao->valor) {
                        tamanho = strlen(associacao->valor);
                    }
                    fseek(file,comecoRegistro,SEEK_SET);

                    //calcular tamnho sem os vars e escrever ...

                }

            }
        }
    }
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
