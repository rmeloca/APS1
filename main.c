/*  
 * File:   main.c
 * Author: romulo
 *
 * Created on 23 de Setembro de 2015, 13:31
 * Adicionar
 * fazer commit
 * remoto->Adicionar
 */

#include <stdio.h>
#include <stdlib.h>
#include "Headers/gerencia.h"
#include <sys/dir.h>

Banco* banco;

int main() {
    char* caminhoArquivo;
    char* nomeTabela;
    Tabela* tabela;
    int option;
    int tuplasInseridas;

    /*
     * Em c, é preciso alocar memória para todos os atributos variáveis.
     * Em 17/10 descobri que isso também é uma necessidade para fread,
     * de modo que não é possível ler um arquivo binário apenas com fread
     * sem saber o tamanho de cada atributo da estrutura.
     * Em outras linguagens a serialização de objetos lida automaticamente
     * com tais questões de memória
     * :(
     * banco = carregarBanco("Arquivos/Banco.MRdb");
     */

    if (!banco) {
        mkdir("Arquivos");
        banco = criarBanco("Arquivos/Banco.MRdb");
    }
    caminhoArquivo = (char*) calloc(1000, sizeof (char));
    nomeTabela = (char*) calloc(1000, sizeof (char));

    do {
        printf("1: Criar tabelas\t"
                "2: Inserir registros\t"
                "3: Listar registros\t"
                "4: Excluir registros\t"
                "5: Imprimir banco\t"
                "0: Sair\t");
        printf("\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("Informe o caminho do arquivo: ");
                scanf("%s", caminhoArquivo);
                if (!normalizarArquivo(caminhoArquivo)) {
                    break;
                }
                interpretarCreateTable(banco, "Arquivos/temp");
                persistirBanco(banco, banco->nomeArquivoBanco);
                printf("Tabelas criadas com sucesso\n");
                break;
            case 2:
                printf("Informe o caminho do arquivo: ");
                scanf("%s", caminhoArquivo);
                if (!normalizarArquivo(caminhoArquivo)) {
                    break;
                }
                tuplasInseridas = interpretarInsertInto(banco, "Arquivos/temp");
                printf("Insert Processado %d tuplas inseridas\n", tuplasInseridas);
                break;
            case 3:
                printf("SELECT * FROM ");
                scanf("%s", nomeTabela);
                tabela = getTabela(banco, nomeTabela);
                if (!tabela) {
                    break;
                }
                carregarRegistros(tabela);
                imprimirTabela(tabela);
                break;
            case 4:
                printf("Informe o nome do arquivo: ");
                scanf("%s", caminhoArquivo);
                if (!normalizarArquivo(caminhoArquivo)) {
                    break;
                }
                interpretarDeleteFrom(banco, "Arquivos/temp");
                break;
            case 5:
                imprimirBanco(banco);
                break;
        }
        printf("\n");
    } while (option);

    return (EXIT_SUCCESS);
}