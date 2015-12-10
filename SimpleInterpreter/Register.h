#ifndef REG_H
#define REG_H

#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Register
#define SYSCPUINTERNALMEM 4

struct Reg_s {
	char container[SYSCPUINTERNALMEM];
};

class Register {
	public:
		Register();
		~Register();

		void init();

		char* get(char *mem);
		void set(char *mem, char *val);
		void add(char *mem, char *val);
		void sub(char *mem, char *val);
		void mul(char *mem, char *val);
		void division(char *mem, char *val);

	private:
		Reg_s reg[3];
};

#endif //!REG_H