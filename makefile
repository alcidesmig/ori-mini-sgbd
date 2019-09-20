main: main.c menu.c commands.c tools.c utils.c parser.c defines.c linenoise/*.cpp
	g++ -o main main.c menu.c commands.c tools.c utils.c parser.c defines.c linenoise/*.cpp -I -O3 -fstrength-reduce -fthread-jumps -frerun-loop-opt
clean:
	rm main .history
