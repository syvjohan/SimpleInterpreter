#include <stdio.h>
#include <stdlib.h>

//Register
#define SYSCPUINTERNALMEM 4

typedef struct reg_s {
	char container[SYSCPUINTERNALMEM];
}reg_t;

reg_t reg[3];

void set(int mem, char *value) {
	int len = strlen(value);
	if (len > 31) {
		//to many characters do CRASH!
	}

	if (mem == 1) {
		*reg[0].container = &value;
		reg[0].container[len] = '\0';
	} else if (mem == 2) {
		*reg[1].container = &value;
		reg[1].container[len] = '\0';
	} else if (mem == 3) {
		*reg[2].container = &value;
		reg[2].container[len] = '\0';
	} else {
		*reg[3].container = &value;
		reg[3].container[len] = '\0';
	}
}

char* get(int mem) {
	if (mem == 1) {
		return reg[0].container;
	} else if (mem == 2) {
		return reg[1].container;
	} else if (mem == 3) {
		return reg[2].container;
	} else {
		return reg[3].container;
	}
}

void add(int mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (mem == 1) {
		int v1 = atoi(reg[0].container);
		sum = v1 + v;
		sprintf(reg[0].container, "%i", sum);
	} else if (mem == 2) {
		int v2 = atoi(reg[1].container);
		sum = v2 + v;
		sprintf(reg[1].container, "%i", sum);
	} else if (mem == 3) {
		int v3 = atoi(reg[2].container);
		sum = v3 + v;
		sprintf(reg[2].container, "%i", sum);
	} else {
		int v4 = atoi(reg[3].container);
		sum = v4 + v;
		sprintf(reg[3].container, "%i", sum);
	}
}

void sub(int mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (mem == 1) {
		int v1 = atoi(reg[0].container);
		sum = v1 - v;
		sprintf(reg[0].container, "%i", sum);
	} else if (mem == 2) {
		int v2 = atoi(reg[1].container);
		sum = v2 - v;
		sprintf(reg[1].container, "%i", sum);
	} else if (mem == 3) {
		int v3 = atoi(reg[2].container);
		sum = v3 - v;
		sprintf(reg[2].container, "%i", sum);
	} else {
		int v4 = atoi(reg[3].container);
		sum = v4 - v;
		sprintf(reg[3].container, "%i", sum);
	}
}

void mul(int mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (mem == 1) {
		int v1 = atoi(reg[0].container);
		sum = v1 * v;
		sprintf(reg[0].container, "%i", sum);
	} else if (mem == 2) {
		int v2 = atoi(reg[1].container);
		sum = v2 * v;
		sprintf(reg[1].container, "%i", sum);
	} else if (mem == 3) {
		int v3 = atoi(reg[2].container);
		sum = v3 * v;
		sprintf(reg[2].container, "%i", sum);
	} else {
		int v4 = atoi(reg[3].container);
		sum = v4 * v;
		sprintf(reg[3].container, "%i", sum);
	}
}

void div(int mem, char *val) {
	int v = atoi(val);
	int sum = 0;
	if (mem == 1) {
		int v1 = atoi(reg[0].container);
		sum = v1 / v;
		sprintf(reg[0].container, "%i", sum);
	} else if (mem == 2) {
		int v2 = atoi(reg[1].container);
		sum = v2 / v;
		sprintf(reg[1].container, "%i", sum);
	} else if (mem == 3) {
		int v3 = atoi(reg[2].container);
		sum = v3 / v;
		sprintf(reg[2].container, "%i", sum);
	} else {
		int v4 = atoi(reg[3].container);
		sum = v4 / v;
		sprintf(reg[3].container, "%i", sum);
	}
}