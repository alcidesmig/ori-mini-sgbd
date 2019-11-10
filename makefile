main: main.c menu.c commands.c tools.c utils.c parser.c defines.c commandsTools.c commandsDefines.c searchStructs.c linenoise/*.cpp
	g++ -std=c++11 -o main btree/*.c main.c menu.c commands.c tools.c utils.c parser.c defines.c commandsTools.c commandsDefines.c searchStructs.c linenoise/*.cpp -I -O3 -fstrength-reduce -fthread-jumps -frerun-loop-opt
clean:
	rm main tables_dir/* tables_index/* tablesIndex.bin tables/* strings.bin stringsEmptyList.bin binaries.bin binariesEmptyList.bin
run:
	clear && ./main
