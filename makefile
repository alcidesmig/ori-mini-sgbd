main: main.c menu.c tools.c commands.c parser.c defines.c error.c data_struct.c
	gcc -o main main.c menu.c tools.c commands.c parser.c defines.c error.c data_struct.c -I -O3 -fstrength-reduce -fthread-jumps -frerun-loop-opt
clean:
	rm main tables_index.tbli tables_dir/*
