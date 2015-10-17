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
#include "Headers/bloco.h"

//Falta procurar espaço dos excluidos

void gerarBloco(char* nomeArquivo) {
    FILE* file = fopen(nomeArquivo, "w");
    int i, n = 0;
    for (i = 0; i < 512; i++) {
        fwrite(&n, sizeof (int), 1, file);
    }

    short int deslocamento = 2048;
    fseek(file, 4, SEEK_SET);
    fwrite(&deslocamento, sizeof (short int), 1, file);

    fclose(file);
}

int calcTamanhoInserir(Tupla *tupla, int qtdAss) {
    int i, tamanho = 0;
    Associacao *associacao;
    //percorre todas associacoes
    for (i = 0; i < qtdAss; i++) {
        associacao = tupla->associacoes[i];

        if (associacao->campo->tipo == INTEGER) {
            //            tamanho += associacao->campo->bytes;
            tamanho += 4;
        } else if (associacao->campo->tipo == BOOLEAN) {
            //            tamanho += associacao->campo->bytes;
            tamanho += 1;
        } else if (associacao->campo->tipo == CHAR) {
            tamanho += associacao->campo->bytes;
        } else if (associacao->campo->tipo == VARCHAR) {
            char* varchar = associacao->valor;
            if (varchar != NULL) {
                tamanho += strlen(varchar);
                tamanho += 4;
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

//trocando para inserir tabela e não tupla a fim de não abrir demasiadas vezes o arquivo

int inserirRegistros(Tabela* tabela) {
    FILE* file;
    Tupla* tupla;
    int* mapaBits;
    Associacao* associacao;
    Campo* campo;
    int tamanho;
    int numeroDoBloco;
    int quantidadeInsercoes;
    int j, k, l;
    int qtdRegistros, comecoRegistro, espacoLivre, novoEspacoLivre, qtdVar = -1, tamanhoVar, comecoVar = 0;

    if (!tabela) {
        return -1;
    }

    quantidadeInsercoes = 0;
    //Percorre todas as tuplas de cada tabela   
    for (j = 0; tabela->numeroTuplas > j; j++) {
        tupla = tabela->tuplas[j];
        //mapa de bits
        mapaBits = calloc(((tabela->numeroCampos / 8) + 1), sizeof (int));
        tamanho = calcTamanhoInserir(tupla, tabela->numeroCampos);
        //Verifica se é possivel inserir o registro, devido ao tamanho dos blocos
        if (tamanho >= 2040) {
            break;
        }
        //Descobre qual bloco deve inserir o registro
        numeroDoBloco = obterBloco(tamanho, tabela->nomesArquivosBlocos, tabela->numeroBlocos, tabela->nome);
        if (numeroDoBloco != tabela->numeroBlocos) {
            tabela->numeroBlocos++;
        }

        qtdVar = -1;
        comecoVar = 0;

        //------
        //Atualiza o cabeçalho do arquivo
        //------
        file = fopen(tabela->nomesArquivosBlocos[numeroDoBloco], "w+b");
        //Le no arq a qtd de registros no bloco
        fread(&qtdRegistros, sizeof (short int), 1, file);
        //Acrescenta um registro armazanado
        qtdRegistros++;
        fwrite(&qtdRegistros + 1, sizeof (int), 1, file);
        qtdRegistros--;
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

        //Insere todos VARCHARs
        //break;
        for (k = 0; tabela->numeroCampos - 1 > k; k++) {
            qtdVar++;
            associacao = tupla->associacoes[k];
            campo = associacao->campo;
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
                printf("Tamanho do var: %d.\nComeco do Var: %d\n ", tamanho, comecoVar);
                //insere ponteiro e tamanho do varchar
                comecoRegistro += qtdVar * 4;
                fseek(file, comecoRegistro, SEEK_SET);
                fwrite(&comecoVar, sizeof (short int), 1, file);
                fwrite(&tamanhoVar, sizeof (short int), 1, file);
                //inseri o varchar na posicao correta
                fseek(file, comecoVar, SEEK_SET);
                fwrite(&associacao->valor, tamanhoVar, 1, file);
                setMapaBits(k, mapaBits);
                printf("Tamanho do\n");
            }

        }
        //Insere o restante dos registros
        for (l = 0; tabela->numeroCampos - 1 > l; l++) {
            associacao = tupla->associacoes[l];
            campo = associacao->campo;
            fseek(file, comecoRegistro, SEEK_SET);
            if (associacao->valor && ((campo->tipo) != VARCHAR)) {
                if ((campo->tipo) == INTEGER) {
                    fwrite(&(associacao->valor), sizeof (int), 1, file);
                } else if ((campo->tipo) == CHAR) {
                    fwrite(&(associacao->valor), sizeof (char), campo->bytes, file);
                } else if ((campo->tipo) == BOOLEAN) {
                    fwrite(&(associacao->valor), sizeof (char), 1, file);
                }
                setMapaBits(l, mapaBits);
            }
        }

        //inserir mapabits
        quantidadeInsercoes++;
        free(mapaBits);
        free(tupla);
    }
    fclose(file);
    return quantidadeInsercoes;
}

void setMapaBits(int posicao, int* mapaBits) {
    int i, bit = 2;
    if ((posicao % 8) == 0) {
        mapaBits[(posicao / 8)] += 1;
    } else {
        for (i = 1; i < (posicao % 8); i++) {
            bit *= bit;
        }
        mapaBits[(posicao / 8)] += bit;
    }
}

//Devolve o numero do bloco para insercao

int obterBloco(int tamanho, char** nomesArquivosBlocos, int numeroBlocos, char* nomeTabela) {
    short int qtdRegistros, iniEspacoLivre, fimEspacoLivre;
    numeroBlocos--;
    FILE *file;
    file = fopen(nomesArquivosBlocos[numeroBlocos], "r+b");
    fread(&qtdRegistros, sizeof (short int), 1, file);

    fseek(file, 4, SEEK_SET);
    fread(&iniEspacoLivre, sizeof (short int), 1, file);

    fimEspacoLivre = (qtdRegistros * 2) + 6;

    if ((iniEspacoLivre - fimEspacoLivre) >= tamanho) {
        return numeroBlocos;
    }
    numeroBlocos++;
    char* temp = (char*) malloc(sizeof (char));
    sprintf(temp, "%d", numeroBlocos);
    char* nomeArq = nomeTabela;
    strcat(nomeArq, temp);

    gerarBloco(nomeArq);
    return numeroBlocos;
}

void carregarRegistros(Tabela* tabela) {
    FILE* file;
    Tupla* tupla;
    Campo* campo;
    Associacao* associacao;
    void* valor;
    char* nomeArquivoBloco;
    int i, j;
    short int numeroRegistros, deslocamentoRegistro, deslocamentoVarchar, tamanho;

    //liberar as tuplas
    freeTuplas(tabela);

    if (tabela->numeroCampos > 8) {
        //2kb mapa de bits
    }

    //olhar em todos os blocos
    for (i = 0; i < tabela->numeroBlocos; i++) {
        nomeArquivoBloco = tabela->nomesArquivosBlocos[i];
        file = fopen(nomeArquivoBloco, "rb");

        fread(&numeroRegistros, sizeof (numeroRegistros), 1, file);
        fseek(file, 6, SEEK_SET);

        //ler todos os registros
        while (tabela->numeroTuplas < numeroRegistros) {
            tupla = criarTupla(tabela);
            //pula os primeiros bytes e aponta para o próximo registro
            fseek(file, 6 + (tabela->numeroTuplas * 2), SEEK_SET);
            fread(&deslocamentoRegistro, sizeof (deslocamentoRegistro), 1, file);
            fseek(file, deslocamentoRegistro, SEEK_SET);

            //para cada registro
            //encontrar todos campos varchar
            for (j = 0; j < tabela->numeroCampos; j++) {
                associacao = tupla->associacoes[j];
                campo = associacao->campo;
                if (campo->tipo == VARCHAR) {
                    fread(&deslocamentoVarchar, sizeof (deslocamentoVarchar), 1, file);
                    fread(&tamanho, sizeof (tamanho), 1, file);
                    fseek(file, deslocamentoVarchar, SEEK_CUR);

                    valor = calloc(tamanho, sizeof (char));
                    fread(valor, tamanho, 1, file);
                    associarValor(associacao, valor);

                    //mapa de bits?

                    //aponta para próximo campo
                    deslocamentoRegistro += 4;
                    fseek(file, deslocamentoRegistro, SEEK_SET);
                    free(valor);
                }
            }

            for (j = 0; j < tabela->numeroCampos; j++) {
                associacao = tupla->associacoes[j];
                campo = associacao->campo;
                if (campo->tipo != VARCHAR) {
                    valor = malloc(campo->bytes);
                    fread(valor, campo->bytes, 1, file);
                    associarValor(associacao, valor);

                    //aponta para próximo campo
                    deslocamentoRegistro += campo->bytes;
                    fseek(file, deslocamentoRegistro, SEEK_SET);
                    free(valor);
                }

            }

            //populá-los nas tuplas
            adicionarTupla(tabela, tupla);
        }
    }
    fclose(file);
}

int remover(Tabela* tabela, Campo* campo, char operador, void* valor) {
    return 0;
}