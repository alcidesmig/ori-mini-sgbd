main: main.cpp menu.cpp commands.cpp tools.cpp utils.c parser.cpp defines.c commandsTools.cpp commandsDefines.c searchStructs.c linenoise/*.cpp
	g++ -std=c++11 -o main btree/*.cpp hash/*.c main.cpp menu.cpp commands.cpp tools.cpp utils.c parser.cpp defines.c commandsTools.cpp commandsDefines.c searchStructs.c linenoise/*.cpp -I -O3 -fstrength-reduce -fthread-jumps -frerun-loop-opt
clean:
	rm -f main tables_dir/* tables_index/* tablesIndex.bin tables/* strings.bin stringsEmptyList.bin binaries.bin binariesEmptyList.bin
run:
	clear && ./main
