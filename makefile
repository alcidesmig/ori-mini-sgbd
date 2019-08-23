main: main.c menu.c tools.c commands.c parser.c defines.c
	gcc -o main main.c menu.c tools.c commands.c parser.c defines.c -I -O3 -fstrength-reduce -fthread-jumps -frerun-loop-opt
clean:
	rm main tables.bin
