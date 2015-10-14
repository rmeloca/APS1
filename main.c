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

Banco* banco;

int main() {
    char* caminhoArquivo;

    //    banco = carregarBanco("Arquivos/Banco.MRdb");
    if (banco == NULL) {
        banco = criarBanco("Arquivos/Banco.MRdb");
    }

    int option;
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
                //                scanf("%s", caminhoArquivo);
                //                normalizarArquivo(caminhoArquivo);
                normalizarArquivo("Arquivos/create.sql");
                interpretarCreateTable(banco, "Arquivos/temp");
                persistirBanco(banco, banco->nomeArquivoBanco);
                printf("Tabelas criadas com sucesso\n");
                break;
            case 2:
                printf("Informe o caminho do arquivo: ");
                //                scanf("%s", caminhoArquivo);
                //                normalizarArquivo(caminhoArquivo);
                normalizarArquivo("Arquivos/insert.sql");
                interpretarInsertInto(banco, "Arquivos/temp");
                printf("Insert Processado %d tuplas inseridas\n", 0);
                break;
            case 3:
                printf("SELECT * FROM ");
                scanf("%s", caminhoArquivo);
                carregarRegistros(caminhoArquivo);
                //void imprimirTabela(Tabela* tabela);
                imprimirBanco(banco);
                break;
            case 4:
                printf("Informe o nome do arquivo: ");
                scanf("%s", caminhoArquivo);
                //                normalizarArquivo(banco, caminhoArquivo);
                //                interpretarDeleteFrom(banco, "Arquivos/temp");
                break;
            case 5:
                imprimirBanco(banco);
                break;
        }
        printf("\n");
    } while (option);

    return (EXIT_SUCCESS);
}