#include <stdio.h>
#include <stdlib.h>
#include "Headers/stack.h"
#include "Headers/token.h"

int identifyDelimiter(char *str) {
    if (*str == '(') {
        return 1;
    } else if (*str == ')') {
        return 2;
    } else if (*str == '[') {
        return 1;
    } else if (*str == ']') {
        return 2;
    } else if (*str == '{') {
        return 1;
    } else if (*str == '}') {
        return 2;
    }
    return 0;
}

int delimiterWed(char *open, char *close) {
    if (*open == '(' && *close == ')') {
        return 1;
    } else if (*open == '[' && *close == ']') {
        return 1;
    } else if (*open == '{' && *close == '}') {
        return 1;
    }
    return 0;
}

int main() {
    char *removed;
    char *str, *s;
    int delimiter;
    FILE *file;
    TokenReader *reader;
    Stack *stack;

    file = fopen("Arquivos/expressao.txt", "r");
    s = (char*) calloc(1000, sizeof (char));
    fgets(s, 1000, file);

    reader = newTokenReader(s);
    stack = createStack();
    removed = (char*) malloc(sizeof (char));

    initializeStack(stack);

    while (hasMoreTokens(reader)) {
        str = nextToken(reader);
        delimiter = identifyDelimiter(str);
        if (delimiter == 1) {
            push(stack, *str);
        } else if (delimiter == 2) {
            pop(stack, removed);
            if (!delimiterWed(removed, str)) {
                printf("Token unexpected: %c\n", *str);
                free(str);
                freeTokenReader(reader);
                return 0;
            }
        }
        free(str);
    }
    if (isEmptyStack(stack)) {
        printf("Expressão válida\n");
    } else {
        printf("Missing arguments\n");
    }
    freeTokenReader(reader);

    fclose(file);
    return 0;
}