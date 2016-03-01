
#include <stdio.h>
#include <stdlib.h>

//Register
#define SYSCPUINTERNALMEM 4

typedef struct reg_s {
	char container[SYSCPUINTERNALMEM];
}reg_t;

reg_t reg[3];

void set(char *mem, char *value) {
	int len = strlen(value);
	if (len > 31) {
		//to many characters do CRASH!
	}

	if (strCmp(mem, "A")) {
		*reg[0].container = value;
		reg[0].container[len] = '\0';
	} else if (strCmp(mem, "B")) {
		*reg[1].container = value;
		reg[1].container[len] = '\0';
	} else if (strCmp(mem, "C")) {
		*reg[2].container = &value;
		reg[2].container[len] = '\0';
	} else if (strCmp(mem, "A")) {
		*reg[3].container = &value;
		reg[3].container[len] = '\0';
	} else {
		//Trying to acces a none existing register do CRASH!
	}
}

char* get(char *mem) {
	if (strCmp(mem, "A")) {
		return reg[0].container;
	} else if (strCmp(mem, "B")) {
		return reg[1].container;
	} else if (strCmp(mem, "C")) {
		return reg[2].container;
	} else if (strCmp(mem, "A")) {
		return reg[3].container;
	} else {
		//Trying to acces a none existing register do CRASH!
	}
}

void add(char *mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (strCmp(mem, "A")) {
		int v1 = atoi(reg[0].container);
		sum = v1 + v;
		sprintf(reg[0].container, "%i", sum);
	} else if (strCmp(mem, "B")) {
		int v2 = atoi(reg[1].container);
		sum = v2 + v;
		sprintf(reg[1].container, "%i", sum);
	} else if (strCmp(mem, "C")) {
		int v3 = atoi(reg[2].container);
		sum = v3 + v;
		sprintf(reg[2].container, "%i", sum);
	} else if (strCmp(mem, "D")) {
		int v4 = atoi(reg[3].container);
		sum = v4 + v;
		sprintf(reg[3].container, "%i", sum);
	} else {
		//Trying to acces a none existing register do CRASH!
	}
}

void sub(char *mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (strCmp(mem, "A")) {
		int v1 = atoi(reg[0].container);
		sum = v1 - v;
		sprintf(reg[0].container, "%i", sum);
	} else if (strCmp(mem, "B")) {
		int v2 = atoi(reg[1].container);
		sum = v2 - v;
		sprintf(reg[1].container, "%i", sum);
	} else if (strCmp(mem, "C")) {
		int v3 = atoi(reg[2].container);
		sum = v3 - v;
		sprintf(reg[2].container, "%i", sum);
	} else if (strCmp(mem, "D")) {
		int v4 = atoi(reg[3].container);
		sum = v4 - v;
		sprintf(reg[3].container, "%i", sum);
	} else {
		//Trying to acces a none existing register do CRASH!
	}
}

void mul(char *mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (strCmp(mem, "A")) {
		int v1 = atoi(reg[0].container);
		sum = v1 * v;
		sprintf(reg[0].container, "%i", sum);
	} else if (strCmp(mem, "B")) {
		int v2 = atoi(reg[1].container);
		sum = v2 * v;
		sprintf(reg[1].container, "%i", sum);
	} else if (strCmp(mem, "C")) {
		int v3 = atoi(reg[2].container);
		sum = v3 * v;
		sprintf(reg[2].container, "%i", sum);
	} else if (strCmp(mem, "D")) {
		int v4 = atoi(reg[3].container);
		sum = v4 * v;
		sprintf(reg[3].container, "%i", sum);
	} else {
		//Trying to acces a none existing register do CRASH!
	}
}

void division(char *mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (strCmp(mem, "A")) {
		int v1 = atoi(reg[0].container);
		sum = v1 / v;
		sprintf(reg[0].container, "%i", sum);
	} else if (strCmp(mem, "B")) {
		int v2 = atoi(reg[1].container);
		sum = v2 / v;
		sprintf(reg[1].container, "%i", sum);
	} else if (strCmp(mem, "C")) {
		int v3 = atoi(reg[2].container);
		sum = v3 / v;
		sprintf(reg[2].container, "%i", sum);
	} else if (strCmp(mem, "D")) {
		int v4 = atoi(reg[3].container);
		sum = v4 / v;
		sprintf(reg[3].container, "%i", sum);
	} else {
		//Trying to acces a none existing register do CRASH!
	}
}