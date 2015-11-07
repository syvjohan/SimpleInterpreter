#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Heap.h"
#include "Reg.h"

char *code = NULL;

char* readFile(const char *path) {
	FILE *file;
	file = fopen(path, "r");

	if (file == NULL) {
		perror(path);
		printf("Error: %d \n", errno);
		return "Error";
	}

	//get size of file.
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	//allocate sizeof file +1.
	char *code = (char *)malloc((fileSize + 1) * sizeof(char));

	//read file into string
	int c;
	int i = 0;
	while ((c = fgetc(file)) != EOF) {
		code[i] = (char)c;
		++i;
	}
	code[i] = '\0';

	fclose(file);

	return code;
}

void allocateMem(char *code) {
	char *start = strpbrk(code, "SYSMEMALLOCHEAP");
	start += 16;
	char *end = strpbrk(code, ";");
	char size[10];
	int len = strlen(start) - strlen(end);
	memcpy(size, start, len);
	size[len] = '\0';

	//Allocate heap.
	init((size_t)size);
}

//Call by reference changes the passed argument no need for returning a value.
void trimString(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ' ') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
	return cStr;
}

int strCmp(const char *cStr1, const char *cStr2) {
	int i = 0;
	while (cStr1[i] != '\0') {
		if (cStr1[i] != cStr2[i]) {
			return 0;
		}
		++i;
	}
	return 1;
}

void evalAlias(char *expression) {
	int len = strlen(expression);
	char *sep1 = strpbrk(expression, ":");
	char *sep2 = strpbrk(expression, "=");
	char *sep3 = strpbrk(expression, ";");
	char name[30];
	char address[20];
	char val[50];

	int lenName = len - strlen(sep1);
	memcpy(name, expression, lenName);
	name[lenName] = '\0';

	int lenAddress;
	if (sep2 != NULL) {
		lenAddress = strlen(sep1) - strlen(sep2) -2;
	} else if (sep3 == NULL) {
		//Something is wrong do CRASH!.
	} else {
		lenAddress = strlen(sep1) +1 - strlen(sep3) - 2;
	}
	
	memcpy(address, sep1 + 1, lenAddress);
	address[lenAddress] = '\0';

	if (sep2 != NULL) {
		int lenValue = strlen(sep2) - strlen(sep3) -1;
		memcpy(val, sep2 +1, lenValue);
		val[lenValue] = '\0';
	}
	
	trimString(name);
	trimString(address);
	if (val != NULL) {
		trimString(val);
		//send value for parsing.
	}
}

void evalDo(char *expression) {

}

void evalWhile(char *expression) {

}

void evalCall(char *expression) {

}

void evalSubroutine(char *expression) {

}

void evalPrintv(char *expression) {

}

void evalPrinta(char *expression) {

}

void evalCmp(char *expression) {

}

void tokenize(char *code) {
	allocateMem(code);

	int i = 0;
	while (code[i] != '\0') {
		if (code[i] == ':') {
			//get keyword
			char *end = strpbrk(code + i, " ");
			char keyword[20];
			int len = i + strlen(end) +1 - strlen(code);
			len = abs(len);//transform to positive value.
			memcpy(keyword, code + i +1, len);
			keyword[len] = '\0';

			//get expression.
			char *sc = strpbrk(code + i +1 +len, ";");
			char expression[50];
			int lenEx = strlen(end) - strlen(sc);
			memcpy(expression, end + 1, lenEx);
			expression[lenEx] = '\0';

			if (strCmp(keyword,"alias")) {
				evalAlias(expression);
			} else if (keyword == "do") {
				evalDo(expression);
			} else if (keyword == "while") {
				evalWhile(expression);
			} else if (keyword == "call") {
				evalCall(expression);
			} else if (keyword == "subroutine") {
				evalSubroutine(expression);
			} else if (keyword == "printv") {
				evalPrintv(expression);
			} else if (keyword == "printa") {
				evalPrinta(expression);
			} else if (keyword == "cmp") {
				evalCmp(expression);
			} 
		}
		++i;
	}
}

void freeMem(void) {
	free(code);
}
