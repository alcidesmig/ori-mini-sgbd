#ifndef TOOLS_H
#define TOOLS_H

char ** split(char * str, char splitter);

void toUpperCase(char * str);

char *findl(char *haystack, const char *needle, int limit);

char *find(char *haystack, const char *needle);

char *stripStart(char *command);

void errorHandler(int error);

#endif /* TOOLS_H */