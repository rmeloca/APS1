/* 
 * File:   campo.h
 * Author: romulo
 *
 * Created on 2 de Outubro de 2015, 19:45
 */

typedef struct campo Campo;
typedef enum tipo Tipo;

enum tipo {
    INTEGER, BOOLEAN, CHAR, VARCHAR
};

struct campo {
    char* nome;
    Tipo tipo;
    int bytes;
};

Campo* criarCampo(char* nome, Tipo tipo, int bytes);
int isCamposEquals(Campo* campo1, Campo* campo2);