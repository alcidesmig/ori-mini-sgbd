main: main.c menu.c tools.c commands.c parser.c
	gcc -o main main.c menu.c tools.c commands.c parser.c -I.