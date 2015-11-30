/* 
 * File:   dados.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 18:51
 */

#include "banco.h"

void gerarBloco(char* nomeArquivo);
int obterBloco(Tabela* tabela, int tamanhoRegistro);
int getTamanhoRegistro(Tupla *tupla, int qtdAss);
void* setMapaBits(void* mapaBits, int posicao);
int inserirRegistros(Tabela* tabela);
void carregarRegistros(Tabela* tabela);
int remover(Tabela* tabela, Campo* campo, char operador, void* valor);