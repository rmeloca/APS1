/* 
 * File:   dados.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:51
 */

#include "banco.h"

int inserirRegistro(Banco *banco);
int calcTamanhoInserir(Tupla *tupla, int qtdAss);
char** procurarEspaco(Tabela *tabela, int tamanho);