#ifndef LEXICAL_H
#define LEXICAL_H

char* readFile(const char *path);
void getInstructions(char *code);
void splitInstruction(char *instruction);

void freeMem(void);

#endif //!LEXICAL_H