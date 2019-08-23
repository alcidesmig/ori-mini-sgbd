# Mini SGBD
- Repositório para o projeto semestral da disciplina de Organização e Recuperação de Informação. UFSCar - 2019.2 ###


### Como usar
- make
- Linha de comando: ./main
- Arquivo: ./main -f file.txt
- Arquivo: ./main --file file.txt

## Organização

### Defines

Contém defines e constantes de comandos, erros, arquivos e limites.

### Commands

Contém funções relativas aos comandos suportados pelo SGBD.

### Parser

Não é um parser, mas identifica o comando e os parâmetros de uma entrada, executando a respectiva função.

### Menu

Mostra os comandos e uma descrição.

### Tools

Funções auxiliares. Provavelmente será separado em Tools e Error.

## Lógica do Código

### init()

Cria o arquivo de indexação das tabelas.

### Interpretação dos parâmetros

Mostra o menu se necessário ou lê o nome de um arquivo para execução.

### fromFile()

Executa os comandos de um arquivo.

### commandLine()

Executa os comandos do terminal.

### Execução

Enquanto há comandos e não há erro, linhas de comandos são lidos, são removidos os espaços do começo e então o comando é passados para o parser.

### parser()

Procura os comandos com a função findl, então é usado um padrão da sscanf para pegar os parâmetros, nessa parte são, Nome da Tabela, Modificadores (se existir) e o primeiro valor(ou dupla de valores). Os dados são tratados. O ponteiro avança para o próximo ';' com a findl, então outros parâmetros são pegos em um loop. Então a respectiva função é chamada.

### createTable()

Lê a quantidade de tabelas do arquivo de indexação, lê os nomes da tabelas, verifica de o novo nome existe, converte a tabela para o modo "rep", então escreve a nova tabela no fim do arquivo e incrementa o número de tabelas no arquivo.

### listTables()

Lê a quantidade de tabelas do arquivo de indexação, lê todos os dados das tabelas de uma vez, converte uma por uma para o modo "tipo", então printa as informações.

## Erro

Existem três variáveis de erro:

### PRE_ERROR_CODE

Erros na interpretação dos parâmetros do programa. Interrompem o programa.

### CMD_ERROR_CODE

Erros na interpretação do comando. Interrompem o programa.

### EXEC_ERROR_CODE

Erros na execução dos comandos. Não interrompem o programa.

## Metadados

### Index

Quantidade de tabelas no 4 primeiros bytes. Struct de cada tabela uma atrás da outra.
