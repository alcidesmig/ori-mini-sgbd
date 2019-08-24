main: main.c menu.c tools.c commands.c parser.c defines.c error.c
	gcc -o main main.c menu.c tools.c commands.c parser.c defines.c error.c -I -O3 -fstrength-reduce -fthread-jumps -frerun-loop-opt
clean:
	rm main tables.bin
