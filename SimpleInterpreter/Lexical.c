#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Heap.h"
#include "Reg.h"
#include "Scope.h"

#define NAMESIZE 30
#define ADDRESSSIZE 20
#define VALUESIZE 50

char *code = NULL;
int index = 0;

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

	//Allocate heap memory.
	int s = atoi(size);
	initializeHeap(s);
}

//Call by reference changes the passed argument no need for returning a value.
void trimString(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ' ' && cStr[i] != '#') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
	return cStr;
}

int strCmp(const char *cStr1, const char *cStr2) {
	int i = 0;
	if (strlen(cStr1) <= 0 || strlen(cStr2) <= 0) { return 0; }
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
	char name[NAMESIZE];
	char address[ADDRESSSIZE];
	char val[VALUESIZE];

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
		//parse val if it is an regular expression.
		parseManager(val);
	}

	int a = atoi(address);
	insertAt(a, val, name);
}

void evalDo(char *expression) {
	char *bodyStart = strpbrk(expression, "{");
	if (bodyStart == NULL) {
		//Missing start body tag do CRASH!.
	}

	incrementScope(index -1, NULL);
}

void evalWhile(char *expression) {
	char *bodyEnd = strpbrk(expression, ";");
	incrementScope(NULL, index);

	//eval if end or goto start.
	char *sepEqual = strstr(expression, "==");
	char *sepNotEqual = strstr(expression, "!=");
	int lenEx = strlen(expression);

	char lhs[50];
	char rhs[50];
	if (sepNotEqual != NULL) {
		int len = lenEx - strlen(sepNotEqual);
		memcpy(lhs, expression, len);
		lhs[len] = '\0';

		memcpy(rhs, expression + len +2, strlen(sepNotEqual));
		rhs[len] = '\0';

		//parse strings.
		parseManager(lhs);
		parseManager(rhs);
		//Values are always *char.
		if (strCmp(lhs, rhs) == 0) {
			int start = getCurrentScopeStart();
			index = start;
			return;
		} else {
			decrementScope();
		}


	} else if (sepEqual != NULL) {
		int len = lenEx - strlen(sepEqual);
		memcpy(lhs, expression, len);
		lhs[len] = '\0';

		memcpy(rhs, expression + len +2, strlen(sepEqual));
		rhs[len] = '\0';

		//parse strings.
		parseManager(lhs);
		parseManager(rhs);
		//Values are always *char.
		if (strCmp(lhs, rhs) == 1) {
			int start = getCurrentScopeStart();
			index = start;
			return;
		} else {
			decrementScope();
		}

	} else {
		// Missing compare operator do CRASH.
	}
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

void evalReg(char *expression, char *keyword) {
	keyword[4] = '.';
	parseReg(strcat(keyword, expression));
}

void tokenize(char *code) {
	allocateMem(code);

	while (code[index] != '\0') {
		if (code[index] == ':') {
			//get keyword
			char *end1 = strpbrk(code + index, " ");
			char *end2 = strpbrk(code + index, ".");
			char *end = end2;
			if (strlen(end1) > strlen(end2)) {
				end = end1;
			}

			char keyword[20];
			int len = index + strlen(end) + 1 - strlen(code);
			len = abs(len);//transform to positive value.
			memcpy(keyword, code + index + 1, len);
			keyword[len] = '\0';

			//get expression.
			char *sc = strpbrk(code + index + 1 + len, ";");
			char expression[50];
			int lenEx = strlen(end) - strlen(sc);
			memcpy(expression, end + 1, lenEx);
			expression[lenEx] = '\0';

			if (strCmp(keyword,"alias")) {
				evalAlias(expression);
			} else if (strCmp(keyword, "do")) {
				evalDo(expression);
			} else if (strCmp(keyword, "while")) {
				evalWhile(expression);
			} else if (strCmp(keyword, "call")) {
				evalCall(expression);
			} else if (strCmp(keyword, "subroutine")) {
				evalSubroutine(expression);
			} else if (strCmp(keyword, "printv")) {
				evalPrintv(expression);
			} else if (strCmp(keyword, "printa")) {
				evalPrinta(expression);
			} else if (strCmp(keyword, "cmp")) {
				evalCmp(expression);
			} else if (strCmp(keyword, "regA")) {
				evalReg(expression, keyword);
			} else if (strCmp(keyword, "regB")) {
				evalReg(expression, keyword);
			} else if (strCmp(keyword, "regC")) {
				evalReg(expression, keyword);
			} else if (strCmp(keyword, "regD")) {
				evalReg(expression, keyword);
			}
			else {

			}

			printf("%s \n", keyword);

			//update index operator
			//index += lenEx;
		}
		++index;
	}
}

void freeMem(void) {
	free(code);
}
