#ifndef LEXICAL_H
#define LEXICAL_H

char* readFile(const char *path);
void tokenize(char *code);
void freeMem(void);

#endif //!LEXICAL_H