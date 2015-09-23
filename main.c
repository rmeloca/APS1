/* 
 * File:   main.c
 * Author: romulo
 *
 * Created on 23 de Setembro de 2015, 13:31
 */

#include <stdio.h>
#include <stdlib.h>
#include "Headers/token.h"

struct {
    char *tipo;
    int bytes;
    short int isFixo;
} Campo;

struct {
    char *nome;
    Campo *campos;
} Tabela;

struct {
    Tabela *tabelas;
} Banco;

struct {
    FILE *blocos;
    FILE *banco;
} Persistencia;

#define boolean short int;
//encapsular corretamente
//encapsular corretamente

int persistirBanco(Banco *banco) {
}

int carregarBanco(FILE *banco) {
}

/*
 * 
 */
int main(int argc, char** argv) {
    FILE *file;
    file = fopen("Arquivos/create.sql", "r");
    TokenReader *tokenReader;
    char *linha;
    char *token;

    while (1) {
        linha = (char*) calloc(1000, sizeof (char));
        fgets(linha, 1000, file);
        tokenReader = newTokenReader(linha);

        token = nextToken(tokenReader);
    }
    //copy file
    //read & create
    fclose(file);

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

