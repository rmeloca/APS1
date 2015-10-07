/* 
 * File:   gerencia.c
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:42
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/gerencia.h"

int persistirBanco(Banco * banco, char* nomeArquivoBanco) {
    FILE* file;
    file = fopen(nomeArquivoBanco, "wb");
    if (file == NULL) {
        return 0;
    }
    fwrite(banco, sizeof (Banco), 1, file);
    fclose(file);
    return 1;
}

Banco* carregarBanco(char* nomeArquivoBanco) {
    FILE* file;
    file = fopen(nomeArquivoBanco, "rb");
    if (file == NULL) {
        return NULL;
    }
    Banco* banco;
    fread(banco, sizeof (Banco), 1, file);
    fclose(file);
    return banco;
}

//corrigir tabulações

void interpretarCreateTable(Banco* banco, char* nomeArquivo) {
    FILE* file;
    TokenReader* tokenReader;
    Tabela* tabela;
    Campo* campo;
    char* linha;
    char* token;
    char* nomeCampo;
    Tipo tipoCampo;
    int bytesCampo;

    file = fopen(nomeArquivo, "r");
    linha = (char*) calloc(1000, sizeof (char));

    //Para todo arquivo
    while (fgets(linha, 1000, file) != NULL) {
        tokenReader = newTokenReader(linha);

        //Para cada CREATE TABLE
        while (hasMoreTokens(tokenReader)) {
            token = nextToken(tokenReader);

            if (!strcasecmp(token, "TABLE")) {
                token = nextToken(tokenReader);
                tabela = criarTabela(token);
                adicionarTabela(banco, tabela);

                //Para cada campo
                while (1) {
                    fgets(linha, 1000, file);
                    setTokenString(tokenReader, linha);

                    nomeCampo = nextToken(tokenReader);
                    if (!strcasecmp(nomeCampo, ")")) {
                        break;
                    }

                    token = nextToken(tokenReader);
                    if (!strcasecmp(token, "INTEGER")) {
                        tipoCampo = INTEGER;
                        bytesCampo = 4;
                    } else if (!strcasecmp(token, "BOOLEAN")) {
                        tipoCampo = BOOLEAN;
                        bytesCampo = 1;
                    } else {
                        if (!strcasecmp(token, "CHAR")) {
                            tipoCampo = CHAR;
                        } else if (!strcasecmp(token, "VARCHAR")) {
                            tipoCampo = VARCHAR;
                        }
                        token = nextToken(tokenReader);
                        token = nextToken(tokenReader);
                        bytesCampo = atoi(token);
                    }
                    campo = criarCampo(nomeCampo, tipoCampo, bytesCampo);
                    adicionarCampo(tabela, campo);
                }
                token = (char*) calloc(100, sizeof (char));
                strcpy(token, "Arquivos/");
                strcat(token, tabela->nome);
                sprintf(nomeCampo, "_%d", tabela->numeroBlocos);
                strcat(token, nomeCampo);
                strcat(token, ".dat");
                gerarBloco(token);
                adicionarBloco(tabela, token);
            }
        }
    }
    fclose(file);
}

void interpretarInsertInto(Banco* banco, char* nomeArquivo) {
    FILE* file;
    TokenReader* tokenReader;
    Tabela* tabela;
    Campo* campo;
    Valores* valores;
    char* linha;
    char* token;
    char* concatenacao;

    file = fopen(nomeArquivo, "r");
    linha = (char*) calloc(1000, sizeof (char));

    //Para todo arquivo
    while (fgets(linha, 1000, file) != NULL) {
        tokenReader = newTokenReader(linha);
        token = nextToken(tokenReader);
        //Para cada INSERT
        while (hasMoreTokens(tokenReader)) {
            if (!strcasecmp(token, "TABLE")) {
                token = nextToken(tokenReader);
                tabela = getTabela(banco, token);
                if (!tabela) {
                    return;
                }
                token = nextToken(tokenReader);

                //Para cada atributo
                while (1) {
                    token = nextToken(tokenReader);
                    if (!strcmp(token, ")")) {
                        break;
                    }
                    printf("campo: %s\n", token);
                    campo = getCampo(tabela, token);
                    //lista de campos
                }

                fgets(linha, 1000, file);
                setTokenString(tokenReader, linha);
                token = nextToken(tokenReader);
                token = nextToken(tokenReader);

                //Para cada valor
                while (hasMoreTokens(tokenReader)) {
                    token = nextToken(tokenReader);
                    if (!strcmp(token, ")")) {
                        break;
                    }
                    //concatenar tokens, retirar apóstrofes
                    if (!strcmp(token, "'")) {
                        strcat(concatenacao, token);
                        token = nextToken(tokenReader);
                        while (strcmp(token, "'")) {
                            strcat(concatenacao, " ");
                            strcat(concatenacao, token);
                        }
                    }
                }

            }

        }
    }
    inserirRegistro(valores);
}

void gerarBloco(char* nomeArquivo) {
    FILE* file = fopen(nomeArquivo, "w");
    int i, n = 0;
    for (i = 0; i < 500; i++) {
        fwrite(&n, sizeof (int), 1, file);
    }

    short int deslocamento = 2000;
    fseek(file, 4, SEEK_SET);
    fwrite(&deslocamento, sizeof (short int), 1, file);

    fclose(file);
}