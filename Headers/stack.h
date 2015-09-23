typedef char ItemType;
typedef struct node Node;

struct node {
    ItemType data;
    Node *next;
};

typedef struct {
    Node *top;
    unsigned int length;
} Stack;

Stack *createStack(); // Criar a pilha
int push(Stack* stack, ItemType e); // Inserir um elemento na pilha;
void initializeStack(Stack *stack); // Inicializa a pilha
int pop(Stack* stack, ItemType* e); // Remover um elemento da pilha
int peek(Stack* stack, ItemType* e); // Recuperar informações do topo da pilha
int contains(Stack* stack, ItemType *e); // Verificar se um elemento está na pilha
int sizeStack(Stack* stack); // Verificar quantos elementos existem na pilha;
int isEmptyStack(Stack* stack); // Verificar se a pilha está vazia
void printStack(Stack* stack); // Ver todo o conteúdo da pilha.