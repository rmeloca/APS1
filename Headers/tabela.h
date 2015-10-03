/* 
 * File:   tabela.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:45
 */

#include "campo.h"

typedef struct tabela Tabela;

struct tabela {
    char* nome;
    Campo** campos;
    int numeroCampos;
    int limiteCampos;
    char** nomesArquivosBlocos;
    int numeroBlocos;
    int limiteBlocos;
};

#define MAXIMO_CAMPOS 10;
#define MAXIMO_BLOCOS 10;

Tabela* criarTabela(char* nome);
void adicionarCampo(Tabela* tabela, Campo* campo);
Campo* getCampo(Tabela* tabela, char* nome);
void adicionarBloco(Tabela* tabela, char* nomeArquivo);