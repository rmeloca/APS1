/* 
 * File:   gerencia.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:46
 */

#include "token.h"
#include "bloco.h"

int persistirBanco(Banco * banco, char* caminhoArquivoBanco);
Banco* carregarBanco(char* caminhoArquivoBanco);
void normalizarArquivo(char* caminhoArquivo);
void interpretarCreateTable(Banco* banco, char* caminhoArquivo);
void interpretarInsertInto(Banco* banco, char* caminhoArquivo);
void interpretarDeleteFrom(Banco* banco, char* caminhoArquivo);