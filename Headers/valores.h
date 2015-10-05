/* 
 * File:   dados.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:51
 */

#include "banco.h"

typedef struct valores Valores;

struct valores {
    Tabela* tabela;
    int** integer;
    char** boolean;
    char** chars;
    char** varchar;
};

int inserirRegistro(Valores *valores);
int calcTamanhoInserir(Valores *valores);
char** procurarEspaco(Tabela *tabela, int tamanho);