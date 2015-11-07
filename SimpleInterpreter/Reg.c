#include <stdio.h>
#include <stdlib.h>

//Register
#define SYSCPUINTERNALMEMA 4
#define SYSCPUINTERNALMEMB 4
#define SYSCPUINTERNALMEMC 4
#define SYSCPUINTERNALMEMD 4

char *regA;
char *regB;
char *regC;
char *regD;

void init() {
	regA[SYSCPUINTERNALMEMA];
	regB[SYSCPUINTERNALMEMB];
	regC[SYSCPUINTERNALMEMC];
	regD[SYSCPUINTERNALMEMD];
}

void insert(int mem, char *value) {
	if (mem == 1) {
		regA[0] = value;
	} else if (mem == 2) {
		regB[0] = value;
	} else if (mem == 3) {
		regC[0] = value;
	} else {
		regD[0] = value;
	}
}

char* get(int mem) {
	if (mem == 1) {
		return regA;
	} else if (mem == 2) {
		return regB;
	} else if (mem == 3) {
		return regC;
	} else {
		return regD;
	}
}