# Mini SGBD
- Repositório para o projeto semestral da disciplina de Organização e Recuperação de Informação. UFSCar - 2019.2 ###


### Como usar
- make
- Linha de comando: ./main
- Printa os comandos: ./main -h
- Arquivo: ./main -f file.txt

## Organização

### Defines

Contém defines e constantes de comandos, arquivos e limites.

### Error

Contém defines de erros e a função raiseError, que gere os erros.

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

Cria o arquivo de indexação das tabelas e o diretório de arquivos de tabelas.

### Interpretação dos parâmetros

Mostra o menu se necessário ou lê o nome de um arquivo para execução.

### Execução

Enquanto há comandos, linhas de comandos são lidos, o comando é passado para o parser.

### parser()

Procura os comandos na linha, para cada comando faz sscanf's específicos para pegar os parâmetros que precisa, então chama a função espefífica.

- "%s" string normal
- "%*c" ignora um char
- "%[^:]" até antes de um ':'
- "%[^;]" até antes de um ';'
- "%[^\n]" até antes de um '\n'

### createTable()

Lê a quantidade de tabelas do arquivo de indexação, lê os nomes da tabelas, verifica de o novo nome existe, converte a tabela para o modo "rep", então escreve o noma da nova tabela no fim do arquivo, incrementa o número de tabelas no arquivo e escreve os metadados no arquivo próprio da tabela.

### apTable()

Lê os metadados da tabela e os printa para o usuário.

### listTables()

Lê a quantidade de tabelas do arquivo de indexação, então lê os nomes das tabelas e os printa para o usuário.

## Erro

A função raiseError pode ser chamada para finalizar o programa ou printar uma mensagem referente a algum erro.

## Metadados

### Index

Quantidade de tabelas nos 4 primeiros bytes. Nome da cada tabela em sequência.

### Arquivo da tabela

Metadados da tabela (TableWRep):
- Tamanho da row da tabela (bytes)
- Nome
- Tipos
- Nome das colunas
- Número de colunas
- Quantidade de rows
- As row em sequência
