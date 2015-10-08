############################# Makefile ##########################
all: MRdb
MRdb: main.o token.o gerencia.o banco.o tabela.o campo.o inserir.o
	gcc -c main.c token.c gerencia.c banco.c tabela.c campo.c inserir.c
	gcc -o main main.o token.o gerencia.o banco.o tabela.o campo.o inserir.o
casamento: token.o casamentoDelimitadores.o stack.o

run:
	./main
clean:
	rm -rf *.o
mrproper: clean
	rm -rf main casamento
