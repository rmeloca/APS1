CREATE TABLE Departamento(
	Dnome varchar(20),
	Dnumero integer,
	Cpf_gerente char(15),
	Data_inicio_gerente boolean
);

CREATE TABLE Funcionario (
	Pnome varchar(20),
	Minicial varchar(20),
	Unome varchar(20),
	Cpf char(15),
	Datanasc boolean,
	Endereco varchar(30),
	Sexo char(10),
	Salario integer,
	Cpf_supervisor char(15),
	Dnr INTEGER
);

CREATE TABLE Localizacao_dep(
	Dnumero integer,
        Dlocal varchar(20)
);

CREATE TABLE Projeto(
        Projnome varchar(20),
	Projnumero integer,
        Projlocal varchar(20),
	Dnum integer
);

CREATE TABLE Trabalha_em(
        Fcpf char(15),
        Pnr integer,
        Horas BOOLEAN
);

CREATE TABLE Dependente(
        Fcpf char(15),
        Nome_dependente varchar(30),
	Sexo char(10),
        Datanasc boolean,
	Parentesco varchar(30)
);