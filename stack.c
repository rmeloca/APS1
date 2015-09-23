#include <stdio.h>
#include <stdlib.h>
#include "Headers/stack.h"

Stack* createStack() {
    Stack *stack = (Stack*) malloc(sizeof (Stack));
    return stack;
}

void initializeStack(Stack* stack) {
    stack->length = 0;
    stack->top = NULL;
}

int push(Stack *stack, ItemType e) {
    Node *node = (Node*) malloc(sizeof (Node));
    node->data = e;
    node->next = stack->top;
    if (stack->length == 0) {
        node->next = NULL;
    }
    stack->top = node;
    stack->length++;
    return 1;
}

int pop(Stack* stack, ItemType* e) {
    if (stack->top) {
        Node *node = (Node*) malloc(sizeof (Node));
        node = stack->top;
        stack->top = node->next;
        *e = node->data;
        free(node);
        stack->length--;
        return 1;
    }
    return 0;
}

int peek(Stack* stack, ItemType* e) {
    if (stack->top) {
        *e = stack->top->data;
        return 1;
    }
    return 0;
}

int contains(Stack* stack, ItemType* e) {
    Node *node = stack->top;
    while (node) {
        if (node->data == *e) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

int sizeStack(Stack* stack) {
    return stack->length;
}

int isEmptyStack(Stack* stack) {
    return stack->length == 0;
}

void printStack(Stack* stack) {
    int i = stack->length - 1;
    Node *node = stack->top;
    printf("\n");
    printf("#\tItem\tEndereço\n");
    while (node) {
        printf("%d\t", i);
        printf("%c\t", node->data);
        printf("%p\n", node);
        node = node->next;
        i--;
    }
    printf("\n");
}