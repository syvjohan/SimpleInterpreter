#ifndef REG_H
#define REG_H

void init();

char* get(int mem);
void set(int mem, char *val);
void add(int mem, char *val);
void sub(int mem, char *val);
void mul(int mem, char *val);
void div(int mem, char *val);

#endif //!REG_H