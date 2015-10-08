/* 
 * File:   gerencia.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:46
 */

#include "token.h"
#include "inserir.h"

int persistirBanco(Banco * banco, char* nomeArquivoBanco);
Banco* carregarBanco(char* nomeArquivoBanco);
void normalizarArquivo(char* nomeArquivo);
void interpretarCreateTable(Banco* banco, char* nomeArquivo);
void interpretarInsertInto(Banco* banco, char* nomeArquivos);
void gerarBloco(char* nomeArquivo);