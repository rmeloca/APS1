/*  
 * File:   main.c
 * Author: romulo
 *
 * Created on 23 de Setembro de 2015, 13:31
 * Adicionar
 * fazer commit
 * remoto->Adicionar
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
typedef struct dados Dados;

#define MAXIMO_TABELAS 10;
#define MAXIMO_CAMPOS 10;
#define MAXIMO_BLOCOS 10;

enum tipo {
    INTEGER, BOOLEAN, CHAR, VARCHAR
};

struct dados {
    Tabela tabela;
    char* boolean;
    char* chars;
    char* varchar;
    char* ordemIns;
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
    char** nomesArquivosBlocos;
    int numeroBlocos;
    int limiteBlocos;
};

struct banco {
    Tabela** tabelas;
    int numeroTabelas;
    int limiteTabelas;
};

struct persistencia {
    char* nomeArquivoBanco;
};

//encapsular corretamente em arquivos
Persistencia* criarPersistencia(char* nomeArquivoBanco);
Banco* criarBanco();
Tabela* criarTabela(char* nome);
Campo* criarCampo(char* nome, Tipo tipo, int bytes);
void adicionarTabela(Banco* banco, Tabela* tabela);
void adicionarCampo(Tabela* tabela, Campo* campo);
int persistirBanco(Banco* banco, char* nomeArquivoBanco);
Banco* carregarBanco(char* nomeArquivoBanco);
void interpretarCreateTable(char* nomeArquivo);
void gerarBloco(char* nomeArquivo);
void inicializarBloco(char* nomeArquivo);
void imprimirBanco(Banco* banco);
void lerInser();
void inserir(Dados dados);
int calcTamanhoInserir();
int procurarEspaco();
Tabela* getTabela(char* nome);
Campo* getCampo(char* nome);

Banco* banco;
Persistencia* persistencia;

int main() {
    char* nomeArquivo;

    persistencia = criarPersistencia("Arquivos/Banco.MRdb");
    //    banco = carregarBanco(persistencia->nomeArquivoBanco);
    if (banco == NULL) {
        banco = criarBanco();
    }

    int option;
    do {
        //        printf("-------------------\n");
        printf("1: Criar tabelas\t"
                "2: Inserir registros\t"
                "3: Listar registros\t"
                "4: Excluir registros\t"
                "0: Sair\t");
        //        printf("-------------------\n");
        printf("\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("Informe o nome do arquivo: ");
                scanf("%s", nomeArquivo);
                interpretarCreateTable("Arquivos/create.sql");
                //                interpretarCreateTable(nomeArquivo);
                persistirBanco(banco, persistencia->nomeArquivoBanco);
                printf("Tabelas criadas com sucesso\n");
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
            case 5:
                imprimirBanco(banco);
                break;
        }
        printf("\n");
    } while (option);

    return (EXIT_SUCCESS);
}

Persistencia* criarPersistencia(char* nomeArquivoBanco) {
    Persistencia* persistencia = (Persistencia*) malloc(sizeof (Persistencia));
    persistencia->nomeArquivoBanco = (char*) calloc(strlen(nomeArquivoBanco), sizeof (char));
    strcpy(persistencia->nomeArquivoBanco, nomeArquivoBanco);
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
    tabela->limiteBlocos = MAXIMO_BLOCOS;
    tabela->numeroBlocos = 0;
    tabela->campos = (Campo**) calloc(tabela->limiteCampos, sizeof (Campo*));
    tabela->numeroCampos = 0;
    tabela->nome = (char*) calloc(strlen(nome), sizeof (char));
    tabela->nomesArquivosBlocos = (char**) calloc(tabela->limiteBlocos, sizeof (char*));
    strcpy(tabela->nome, nome);
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
        return;
    }
    banco->tabelas[banco->numeroTabelas] = tabela;
    banco->numeroTabelas++;
}

void adicionarCampo(Tabela * tabela, Campo * campo) {
    if (tabela->numeroCampos == tabela->limiteCampos) {
        //dobra tamanho do vetor e realoca
        return;
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

                tabela->nomesArquivosBlocos[tabela->numeroBlocos] = (char*) calloc(strlen(tabela->campos[0]->nome), sizeof (char));
                strcpy(tabela->nomesArquivosBlocos[tabela->numeroBlocos], "tabela->campos[0]->nome");
                gerarBloco(tabela->nomesArquivosBlocos[tabela->numeroBlocos]);
                tabela->numeroBlocos++;
            }
        }
    }
    fclose(file);
}

void gerarBloco(char* nomeArquivo) {
    FILE* file = fopen(nomeArquivo, "w");
    int i, n = 0;
    for (i = 0; i < 500; i++) {
        fwrite(&n, sizeof (int), 1, file);
    }

    short int deslocamento = 2000;
    fseek(file, 4, 0);
    fwrite(&deslocamento, sizeof (short int), 1, file);

    fclose(file);
}

void lerInser() {
    inserir(Dados dados);
}

void inserir(Dados dados) {
}

int calcTamanhoInserir() {
}

int procurarEspaco() {
}

void imprimirBanco(Banco* banco) {
    Tabela* tabela;
    Campo* campo;
    printf("Número Tabelas: %d\n", banco->numeroTabelas);
    printf("Limite Tabelas: %d\n", banco->limiteTabelas);
    int i, j;
    for (i = 0; i < banco->numeroTabelas; i++) {
        tabela = banco->tabelas[i];
        printf("----%s----\n", tabela->nome);
        printf("Número Campos: %d\n", tabela->numeroCampos);
        printf("Limite Campos: %d\n", tabela->limiteCampos);
        for (j = 0; j < tabela->numeroCampos; j++) {
            campo = tabela->campos[j];
            printf("\n");
            printf("Campo: %s\n", campo->nome);
            printf("Tipo: %d\n", campo->tipo);
            printf("Bytes: %d\n", campo->bytes);
        }
    }
}

Tabela* getTabela(char* nome) {
}

Campo* getCampo(char* nome) {
}