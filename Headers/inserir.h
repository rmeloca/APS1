/* 
 * File:   dados.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:51
 */

#include "banco.h"

int calcTamanhoInserir(Tupla *tupla, int qtdAss);
int inserirRegistro(Banco* banco);
int obterBloco(int tamanho, char** nomesArquivosBlocos, int numeroBlocos, char* nomeTabela);