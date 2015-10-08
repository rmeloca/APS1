/* 
 * File:   banco.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:37
 */

#include "tabela.h"

typedef struct banco Banco;

#define MAXIMO_TABELAS 10;

struct banco {
    Tabela** tabelas;
    int numeroTabelas;
    int limiteTabelas;
    char* nomeArquivoBanco;
};

Banco* criarBanco(char* nomeArquivoBanco);
void adicionarTabela(Banco* banco, Tabela* tabela);
Tabela* getTabela(Banco* banco, char* nome);
void imprimirBanco(Banco* banco);