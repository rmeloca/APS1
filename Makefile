############################# Makefile ##########################
all: MRdb
MRdb: main.o token.o
	gcc -c main.c token.c
	gcc -o main main.o token.o

casamento: token.o casamentoDelimitadores.o stack.o
	gcc -c token.c casamentoDelimitadores.c stack.c
	gcc -o casamento token.o casamentoDelimitadores.o stack.o
run:
	./main
clean:
	rm -rf *.o
mrproper: clean
	rm -rf main casamento
