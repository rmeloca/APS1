/* 
 * File:   main.c
 * Author: romulo
 *
 * Created on 23 de Setembro de 2015, 13:31
 */

#include <stdio.h>
#include <stdlib.h>
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
int persistirBanco(Banco* banco);
Banco* carregarBanco();
void lerArquivo();
Banco* criarBanco();
Tabela* criarTabela(char* nome);
Campo* criarCampo(char* nome, Tipo tipo, int bytes);
void adicionarTabela(Banco* banco, Tabela* tabela);
void adicionarCampo(Tabela* tabela, Campo* campo);

/*
 * 
 */
int main(int argc, char** argv) {
    FILE* file;
    TokenReader* tokenReader;
    char* linha;
    char* token;

    file = fopen("Arquivos/create.sql", "r");
    linha = (char*) calloc(1000, sizeof (char));

    while (fgets(linha, 1000, file) != NULL) {
        tokenReader = newTokenReader(linha);
        while ((token = nextToken(tokenReader)) != NULL) {
            if (!strcasecmp(token, "CREATE")) {
                printf("%s\n", token);
                token = nextToken(tokenReader);
                printf("%s\n", token);
                token = nextToken(tokenReader);
                printf("%s\n", token);
            }
            //            if (strcmp(token, ' ') == 0) {
            //            if (*token == '') {
            //                printf("%s\n", token);
            //                printf("ESPAÇO\n");
            //            }
        }
    }
    //read & create
    fclose(file);

    //*********************
    Banco* banco;
    Tabela* tabela;
    Campo* campo;

    banco = criarBanco();

    tabela = criarTabela("empregados");
    adicionarTabela(banco, tabela);

    campo = criarCampo("Dnome", VARCHAR, 20);
    adicionarCampo(tabela, campo);

    campo = criarCampo("Dnumero", INTEGER, 4);
    adicionarCampo(tabela, campo);

    persistirBanco(banco);
    banco = carregarBanco();


    //*********************
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

Banco* criarBanco() {
    Banco* banco = (Banco*) malloc(sizeof (Banco));
    banco->limiteTabelas = MAXIMO_TABELAS;
    banco->tabelas = (Tabela**) calloc(banco->limiteTabelas, sizeof (Tabela*));
    banco->numeroTabelas = 0;
    return banco;
}

Tabela* criarTabela(char* nome) {
    Tabela* tabela = (Tabela*) malloc(sizeof (Tabela));
    tabela->limiteCampos = MAXIMO_CAMPOS;
    tabela->campos = (Campo**) calloc(tabela->limiteCampos, sizeof (Campo*));
    tabela->nome = nome;
    tabela->numeroCampos = 0;
    return tabela;
}

Campo* criarCampo(char* nome, Tipo tipo, int bytes) {
    Campo* campo = (Campo*) malloc(sizeof (Campo));
    campo->nome = nome;
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

void adicionarCampo(Tabela * tabela, Campo* campo) {
    if (tabela->numeroCampos == tabela->limiteCampos) {
        //dobra tamanho do vetor e realoca
    }
    tabela->campos[tabela->numeroCampos] = campo;
    tabela->numeroCampos++;
}

int persistirBanco(Banco* banco) {
    FILE* file;
    file = fopen("Banco.MRdb", "wb");
    if (file == NULL) {
        return 0;
    }
    fwrite(banco, sizeof (Banco), 1, file);
    fclose(file);
    return 1;
}

Banco* carregarBanco() {
    FILE* file;
    file = fopen("Banco.MRdb", "rb");
    if (file == NULL) {
        return NULL;
    }
    Banco* banco;
    fread(banco, sizeof (Banco), 1, file);
    fclose(file);
    return banco;
}