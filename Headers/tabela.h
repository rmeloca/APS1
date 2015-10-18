/* 
 * File:   tabela.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:45
 */

#include "campo.h"

typedef struct tabela Tabela;
typedef struct tupla Tupla;
typedef struct associacao Associacao;

struct tabela {
    char* nome;
    Campo** campos;
    Tupla** tuplas;
    char** nomesArquivosBlocos;
    int numeroCampos;
    int numeroTuplas;
    int numeroBlocos;
    int limiteCampos;
    int limiteTuplas;
    int limiteBlocos;
};

struct tupla {
    Associacao **associacoes;
};

struct associacao {
    Campo *campo;
    void* valor;
};

#define MAXIMO_CAMPOS 10;
#define MAXIMO_BLOCOS 10;
#define MAXIMO_TUPLAS 10;

Tabela* criarTabela(char* nome);
void adicionarCampo(Tabela* tabela, Campo* campo);
void adicionarBloco(Tabela* tabela, char* nomeArquivo);
Campo* getCampo(Tabela* tabela, char* nome);
Tupla* criarTupla(Tabela* tabela);
void adicionarTupla(Tabela* tabela, Tupla* tupla);
Associacao* criarAssociacao(Campo* campo);
Associacao* findAssociacao(Tabela* tabela, Tupla* tupla, Campo* campo);
void associarValor(Associacao* associacao, void* valor);
void imprimirTabela(Tabela* tabela);
void freeTuplas(Tabela* tabela);
void freeTupla(Tabela* tabela, Tupla* tupla);