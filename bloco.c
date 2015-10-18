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

//Devolve o numero do bloco para insercao

int obterBloco(Tabela* tabela, int tamanhoRegistro) {
    short int qtdRegistros, iniEspacoLivre, fimEspacoLivre;
    FILE *file;

    file = fopen(tabela->nomesArquivosBlocos[tabela->numeroBlocos - 1], "r+b");
    fread(&qtdRegistros, sizeof (short int), 1, file);

    fseek(file, 4, SEEK_SET);
    fread(&iniEspacoLivre, sizeof (short int), 1, file);

    fimEspacoLivre = (qtdRegistros * 2) + 6;

    if ((iniEspacoLivre - fimEspacoLivre) >= tamanhoRegistro) {
        return tabela->numeroBlocos - 1;
    }

    printf("Criar novo bloco\n");

    char* temp = (char*) calloc(100, sizeof (char));
    char* nomeArquivo = (char*) calloc(100, sizeof (char));

    //criar bloco para cada tabela
    sprintf(temp, "_%d", tabela->numeroBlocos + 1);
    strcpy(nomeArquivo, "Arquivos/");
    strcat(nomeArquivo, tabela->nome);
    strcat(nomeArquivo, temp);
    strcat(nomeArquivo, ".dat");

    gerarBloco(nomeArquivo);
    adicionarBloco(tabela, nomeArquivo);

    return tabela->numeroBlocos;
}

int getTamanhoRegistro(Tupla *tupla, int qtdAssociacoes) {
    Associacao *associacao;
    int i, tamanho;

    tamanho = 0;
    //percorre todas associacoes
    for (i = 0; i < qtdAssociacoes; i++) {
        associacao = tupla->associacoes[i];

        if (associacao->campo->tipo == VARCHAR) {
            //ponteiros, ainda que campo nulo
            tamanho += 4;
            if (associacao->valor != NULL) {
                tamanho += strlen(associacao->valor);
            }
        } else {
            tamanho += associacao->campo->bytes;
        }
    }

    //soma o tamanho do mapa de bits
    if (qtdAssociacoes % 8 != 0) {
        tamanho++;
    }
    tamanho += qtdAssociacoes / 8;
    return tamanho;
}

//getMapaBits

void setMapaBits(int* mapaBits, int posicao) {
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

//preciso de tabela->numeroBlocos, portanto só se eu recebesse como parâmetro

int inserirRegistros(Tabela* tabela) {
    FILE* file;
    Tupla* tupla;
    Associacao* associacao;
    Campo* campo;
    int* mapaBits;
    int tamanhoRegistro;
    int numeroDoBloco;
    int quantidadeInsercoes;
    int i, j;
    int qtdRegistros, comecoRegistro, espacoLivre, novoEspacoLivre, qtdVarchar, tamanhoVar, comecoVar;

    if (!tabela) {
        return -1;
    }

    quantidadeInsercoes = 0;
    //Percorre todas as tuplas
    for (i = 0; i < i + tabela->numeroTuplas; i++) {
        tupla = tabela->tuplas[i];

        tamanhoRegistro = getTamanhoRegistro(tupla, tabela->numeroCampos);
        printf("Tamanho registro: %d\n", tamanhoRegistro);

        //Verifica se é possivel inserir o registro, devido ao tamanho dos blocos
        if (tamanhoRegistro >= 2040) {
            break;
        }

        //Descobre qual bloco deve inserir o registro
        numeroDoBloco = obterBloco(tabela, tamanhoRegistro);
        file = fopen(tabela->nomesArquivosBlocos[numeroDoBloco], "w+b");

        //mapa de bits
        mapaBits = (int*) calloc((tabela->numeroCampos / 8) + 1, sizeof (int));

        //Atualiza o cabeçalho do arquivo
        //Le no arq a qtd de registros no bloco
        fread(&qtdRegistros, sizeof (short int), 1, file);
        //Acrescenta um registro armazanado
        //O.O
        qtdRegistros++;
        fwrite(&qtdRegistros + 1, sizeof (int), 1, file);
        qtdRegistros--;
        //Le a posicao do comeco do espaco livre
        fseek(file, 4, SEEK_SET);
        fread(&espacoLivre, sizeof (short int), 1, file);
        fseek(file, 4, SEEK_SET);
        //Escreve o novo espaco livre
        novoEspacoLivre = (espacoLivre - tamanhoRegistro) - 1;
        fwrite(&novoEspacoLivre, sizeof (short int), 1, file);
        //Escreve ponteiro para o comeco do novo registro
        comecoRegistro = (espacoLivre - tamanhoRegistro);
        fseek(file, (6 + (qtdRegistros * 2)), SEEK_SET);
        fwrite(&comecoRegistro, sizeof (short int), 1, file);

        printf("numeroBloco: %d\n", numeroDoBloco);

        /*
         * Inserir apontadores varchar
         * Inserir o restante
         * Inserir mapa de bits
         * Inserir valores varchar
         */

        qtdVarchar = -1;
        comecoVar = 0;
        //Insere todos VARCHARs
        for (j = 0; j < tabela->numeroCampos - 1; j++) {
            qtdVarchar++;
            associacao = tupla->associacoes[j];
            campo = associacao->campo;
            if (campo->tipo == VARCHAR) {
                tamanhoVar = 0;
                if (associacao->valor) {
                    tamanhoRegistro = strlen(associacao->valor);
                }
                if (comecoVar == 0) {
                    comecoVar = (tamanhoRegistro - tamanhoVar) + 1;
                } else {
                    comecoVar -= tamanhoVar;
                }

                printf("Tamanho do var: %d\n", tamanhoRegistro);
                printf("Comeco do Var: %d\n", comecoVar);

                //insere ponteiro e tamanho do varchar
                comecoRegistro += qtdVarchar * 4;
                fseek(file, comecoRegistro, SEEK_SET);
                fwrite(&comecoVar, sizeof (short int), 1, file);
                fwrite(&tamanhoVar, sizeof (short int), 1, file);
                //inseri o varchar na posicao correta
                fseek(file, comecoVar, SEEK_SET);
                fwrite(&associacao->valor, tamanhoVar, 1, file);
                setMapaBits(mapaBits, j);
            }
        }
        //Insere o restante dos registros
        for (j = 0; tabela->numeroCampos - 1 > j; j++) {
            associacao = tupla->associacoes[j];
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
                setMapaBits(mapaBits, j);
            }
        }
        //inserir mapabits

        quantidadeInsercoes++;

        free(mapaBits);
        freeTupla(tabela, tupla);
        fclose(file);
    }
    return quantidadeInsercoes;
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