main: main.c menu.c aux.c commands.c parser.c
	gcc -o main main.c menu.c aux.c commands.c parser.c -I.