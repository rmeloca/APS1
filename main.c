/* 
 * File:   main.c
 * Author: romulo
 *
 * Created on 23 de Setembro de 2015, 13:31
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/token.h"

typedef struct campo Campo;
typedef struct tabela Tabela;
typedef struct banco Banco;
typedef struct persistencia Persistencia;
typedef enum tipo Tipo;

#define MAXIMO_TABELAS 10;
#define MAXIMO_CAMPOS 10;
#define MAXIMO_BLOCOS 10;

enum tipo {
    INTEGER, BOOLEAN, CHAR, VARCHAR
};

struct campo {
    char* nome;
    Tipo tipo;
    int bytes;
};

struct tabela {
    char* nome;
    Campo** campos;
    int numeroCampos;
    int limiteCampos;
};

struct banco {
    Tabela** tabelas;
    int numeroTabelas;
    int limiteTabelas;
};

struct persistencia {
    char** nomesArquivosBlocos;
    char* nomeArquivoBanco;
    int numeroBlocos;
    int limiteBlocos;
};

//encapsular corretamente
Persistencia* criarPersistencia(char* nomeArquivoBanco);
Banco* criarBanco();
Tabela* criarTabela(char* nome);
Campo* criarCampo(char* nome, Tipo tipo, int bytes);
void adicionarTabela(Banco* banco, Tabela* tabela);
void adicionarCampo(Tabela* tabela, Campo* campo);
int persistirBanco(Banco* banco, char* nomeArquivoBanco);
Banco* carregarBanco(char* nomeArquivoBanco);
void interpretarCreateTable(char* nomeArquivo);
void gerarBloco();

Banco* banco;
Persistencia* persistencia;

int main(int argc, char** argv) {
    char* nomeArquivo;

    persistencia = criarPersistencia("Banco.MRdb");
    banco = carregarBanco(persistencia->nomeArquivoBanco);
    if (banco == NULL) {
        banco = criarBanco();
    }

    int option = 1;
    while (option) {
        printf("-------------------\n");
        printf("1: Criar tabelas\n"
                "2: Inserir dados\n"
                "3: Listar dados\n"
                "2: Remover dados\n"
                "0: Sair do sistema\n");
        printf("-------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("Informe o nome do arquivo: ");
                scanf("%s", nomeArquivo);
                interpretarCreateTable(nomeArquivo);
                persistirBanco(banco, persistencia->nomeArquivoBanco);
                gerarBloco();
                break;
            case 2:
                printf("Informe o nome do arquivo: ");
                break;
            case 3:
                printf("Informe o nome do arquivo: ");
                break;
            case 4:
                printf("Informe o nome do arquivo: ");
                break;
        }
    }

    return (EXIT_SUCCESS);
}

Persistencia* criarPersistencia(char* nomeArquivoBanco) {
    Persistencia* persistencia = (Persistencia*) malloc(sizeof (Persistencia));
    persistencia->limiteBlocos = MAXIMO_BLOCOS;
    persistencia->nomeArquivoBanco = (char*) calloc(strlen(nomeArquivoBanco), sizeof (char));
    strcpy(persistencia->nomeArquivoBanco, nomeArquivoBanco);
    persistencia->numeroBlocos = 0;
    return persistencia;
}

Banco * criarBanco() {
    Banco* banco = (Banco*) malloc(sizeof (Banco));
    banco->limiteTabelas = MAXIMO_TABELAS;
    banco->tabelas = (Tabela**) calloc(banco->limiteTabelas, sizeof (Tabela*));
    banco->numeroTabelas = 0;
    return banco;
}

Tabela * criarTabela(char* nome) {
    Tabela* tabela = (Tabela*) malloc(sizeof (Tabela));
    tabela->limiteCampos = MAXIMO_CAMPOS;
    tabela->campos = (Campo**) calloc(tabela->limiteCampos, sizeof (Campo*));
    tabela->nome = (char*) calloc(strlen(nome), sizeof (char));
    strcpy(tabela->nome, nome);
    tabela->numeroCampos = 0;
    return tabela;
}

Campo * criarCampo(char* nome, Tipo tipo, int bytes) {
    Campo* campo = (Campo*) malloc(sizeof (Campo));
    campo->nome = (char*) calloc(strlen(nome), sizeof (char));
    strcpy(campo->nome, nome);
    campo->tipo = tipo;
    campo->bytes = bytes;
    return campo;
}

void adicionarTabela(Banco* banco, Tabela * tabela) {
    if (banco->numeroTabelas == banco->limiteTabelas) {
        //dobra tamanho do vetor e realoca
    }
    banco->tabelas[banco->numeroTabelas] = tabela;
    banco->numeroTabelas++;
}

void adicionarCampo(Tabela * tabela, Campo * campo) {
    if (tabela->numeroCampos == tabela->limiteCampos) {
        //dobra tamanho do vetor e realoca
    }
    tabela->campos[tabela->numeroCampos] = campo;
    tabela->numeroCampos++;
}

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

Banco * carregarBanco(char* nomeArquivoBanco) {
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

void interpretarCreateTable(char* nomeArquivo) {
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
            }
        }
    }
    fclose(file);
}

void gerarBloco() {

}