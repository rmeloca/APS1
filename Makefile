############################# Makefile ##########################
all: MRdb
MRdb: main.o token.o gerencia.o banco.o tabela.o campo.o valores.o
casamento: token.o casamentoDelimitadores.o stack.o

run:
	./main
clean:
	rm -rf *.o
mrproper: clean
	rm -rf main casamento
