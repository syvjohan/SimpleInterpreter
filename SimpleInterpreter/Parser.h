#ifndef PARSER_H
#define PARSER_H

char* parseManager(char *cStr, int isReference);
char* parseReg(char *cStr);
char* parseHeap(char *cStr, int isReference);

#endif //!PARSER_H