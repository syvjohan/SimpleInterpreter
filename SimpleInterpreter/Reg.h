#ifndef REG_H
#define REG_H

void init();

char* get(int mem);
void set(char *mem, char *val);
void add(char *mem, char *val);
void sub(char *mem, char *val);
void mul(char *mem, char *val);
void division(int *mem, char *val);

#endif //!REG_H