########################
##### BD Simulator #####
########################

Marco Cezar Moreira de Mattos, 1137085
Rômulo Manciola Meloca, 1137131

1. De posse dos arquivos fonte, realizar a compilação com os comandos
    make mrproper
    make all
2. Para executá-lo neste mesmo diretório basta comandar
    make run
3. Uma vez na interface do sistema é possível criar tabelas dentro do banco, inserir registros, recuperá-los ou excluí-los.
    É também possível listar a estrutura do banco criado.
    Apenas um banco é admitido no sistema.
    
    1. Criar tabelas
        Para criar tabelas, basta acessar a opção 1 que pedirá ao usuário o caminho absoluto ou relativo do arquivo contendo as instruções válidas.
        O sistema suporta os tipos CHAR, VARCHAR, INTEGER, BOOLEAN
        Podem ser informadas várias sentenças.
    2. Inserir registros
        Para inserir registros, opção 2 onde deverá ser informado o caminhodo sql.
        Podem ser informadas mais de uma tabela.
        Funcionalidade ainda comprometida.
    3. Listar registros
        Nesta opção deverá ser informado a tabela será realizado o select.
        Este comando retornará, em todos os casos, todos os registros persistidos no banco.
        Funcionalidade comprometida, devido requisito anterior.
    4. Excluir registros
        Para exclusão, deverá ser informado o caminho do arquivo sql contendo instruções de exclusão.
        Podem ser informadas mais de uma tabela.
        Funcionalidade comprometida, devido requisito anterior.
    5. Imprimir banco
        Esta funcionalidade simplesmente imprime a estrutura do banco de dados

4. Para sair do sistema basta informar o número 0
    Após a saída do sistema, o banco somente será hábil a recuperar os dados persistidos anteriormente, se a opção 1 for novamente ativada,
    uma vez que ao encerramento do programa, sua estrutura será perdida
5. Observa-se que o código compilado, independente do diretório em que se encontra, será hábil a desempenhar todas as funções normalmente.