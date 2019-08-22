main: main.c menu.c tools.c commands.c parser.c
	gcc -o main main.c menu.c tools.c commands.c parser.c -I -O3 -fstrength-reduce -fthread-jumps -frerun-loop-opt

clean:
	rm main