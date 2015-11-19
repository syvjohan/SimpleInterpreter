#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "Heap.h"
#include "Reg.h"
#include "Scope.h"
#include "Parser.h"

#define NAMESIZE 30
#define ADDRESSSIZE 20
#define VALUESIZE 50
#define INSTRUCTIONSIZE 100

char *code = NULL;

int index = 0;
int start = 0;
int end = 0;

int ignore = 0;

char keyword[NAMESIZE];
char expression[VALUESIZE];
int cmpResult = 0;

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
}

void removeParanthesis(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ')' && cStr[i] != '(') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

int strCmp(const char *cStr1, const char *cStr2) {
	int i = 0;
	if (strlen(cStr1) <= 0 || strlen(cStr2) <= 0) { return 0; }
	while (cStr1[i] != '\0' || cStr2[i] != '\0') {
		if (cStr1[i] != cStr2[i]) {
			return 0;
		}
		++i;
	}
	return 1;
}

char* getKeyword() {
	return keyword;
}

void setKeyword(char *cStr) {
	memset(keyword, 0, strlen(keyword));
	memcpy(keyword, cStr, strlen(cStr));
}

char* getExpression() {
	return expression;
}

void setExpression(char *cStr) {
	memset(expression, 0, strlen(expression));
	memcpy(expression, cStr, strlen(cStr));
}

void allocateMem() {
	char *expression = getExpression();
	trimString(expression);
	
	//Allocate heap memory.
	int s = atoi(expression);
	initializeHeap(s);
}

void evalAlias() {
	char *expression = getExpression();
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
	if (sep2) {
		lenAddress = strlen(sep1) - strlen(sep2) -1; //-2
	} else if (sep3) {
		//Something is wrong do CRASH!.
	} else {
		lenAddress = strlen(sep1) +1 - strlen(sep3) - 2;
	}
	
	memcpy(address, sep1 + 1, lenAddress);
	address[lenAddress] = '\0';

	if (sep2) {
		int lenValue = strlen(sep2) -3;
		memcpy(val, sep2 +2, lenValue);
		val[lenValue] = '\0';
	}
	
	trimString(name);
	trimString(address);
	if (val) {
		trimString(val);
		//parse val if it is an regular expression.
		parseManager(val, 0);
	}

	int a = atoi(address);
	insertAt(a, val, name);
}

void evalDo(int len) {
	end = index;
	index -= len;
	start = index;
	incrementScope(index, -1);
}

void evalName() {
	char *expression = getExpression();
	trimString(expression);

	char *equal = strstr(expression, "=");
	if (equal) {
		int lenRhs = strlen(equal) -3;
		int lenLhs = strlen(expression) - lenRhs;

		char lhs[VALUESIZE];
		char rhs[VALUESIZE];

		memcpy(lhs, expression, lenLhs);
		lhs[lenLhs] = '\0';

		memcpy(rhs, equal +2, lenRhs);
		rhs[lenRhs] = '\0';

		int isReferenceRhs = 0;
		int isReferenceLhs = 0;
		char *findReferenceRhs = strstr(rhs, "&");
		char *findReferenceLhs = strstr(lhs, "&");
		if (findReferenceRhs) {
			isReferenceRhs = 1;
		}

		if (findReferenceLhs) {
			isReferenceLhs = 1;
		}

		char *lhsRes = parseManager(lhs, isReferenceLhs);
		char *rhsRes = parseManager(rhs, isReferenceRhs);

		if (isReferenceLhs) {

			insertAt(atoi(lhsRes), rhsRes, "");
			
		} else {
			//Lhs (left hand sign) need to be a modifiable value(&), do CRASH!!!
		}

	} else {
		//Equal operator missing, do CRASH!!!
	}
}

void evalWhile() {
	char *expression = getExpression();
	char *bodyEnd = strpbrk(expression, ";");
	if (getCurrentScopeEnd() != index) {
		incrementScope(-1, index);
	}

	//eval if end or goto start.
	char *sepEqual = strstr(expression, "==");
	char *sepNotEqual = strstr(expression, "!=");
	int lenEx = strlen(expression);

	char lhs[VALUESIZE];
	char rhs[VALUESIZE];
	if (sepNotEqual) {
		int len = lenEx - strlen(sepNotEqual);
		memcpy(lhs, expression, len);
		lhs[len] = '\0';

		memcpy(rhs, expression + len +2, strlen(sepNotEqual));
		rhs[len] = '\0';

		//reference = 1, value = 0.
		int isReference = 0;
		char *bitwiseAnd = strstr(lhs, "&");
		char *hashtag = strstr(rhs, "#");

		if (bitwiseAnd && hashtag) {
			isReference = 1;
		} else if (bitwiseAnd) {
			//Wrong syntax cannot compare address with value, do CRASH!
		}

		//parse strings.
		char *tmpL = parseManager(lhs, isReference);
		memset(&lhs, 0, VALUESIZE);
		memcpy(lhs, tmpL, strlen(tmpL));

		char *tmpR = parseManager(rhs, isReference);
		//memset(&rhs, 0, 50);
		memcpy(rhs, tmpR, strlen(tmpR));

		printf("i = %s\n", lhs);
		//Values are always *char.
		if (strCmp(lhs, rhs) == 0) {
			start = getCurrentScopeStart();
			index = start;
			printf("Values are not equal\n");
			return;
		} else {
			decrementScope();
			printf("Values are equal\n");
		}

	} else if (sepEqual) {
		int len = lenEx - strlen(sepEqual);
		memcpy(lhs, expression, len);
		lhs[len] = '\0';

		memcpy(rhs, expression + len +2, strlen(sepEqual));
		rhs[len] = '\0';

		//reference = 1, value = 0.
		int isReference = 0;
		char *bitwiseAnd = strstr(lhs, "&");
		char *hashtag = strstr(lhs, "#");
		if (bitwiseAnd && hashtag) {
			isReference = 1;
		} else if (bitwiseAnd) {
			//Wrong syntax cannot compare address with value, do CRASH!
		}

		//parse strings.
		parseManager(lhs, isReference);
		parseManager(rhs, isReference);

		//Values are always *char.
		if (strCmp(lhs, rhs) == 1) {
			start = getCurrentScopeStart();
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

void evalCmp() {
	char *expression = getExpression();
	removeParanthesis(expression);
	char *opComma = strstr(expression, ",");
	

	int lenRhs = strlen(opComma);
	int lenLhs = strlen(expression) - lenRhs;
	char lhs[VALUESIZE];
	char rhs[VALUESIZE];
	if (opComma) {
		memcpy(lhs, expression, lenLhs);
		lhs[lenLhs] = '\0';
		memcpy(rhs, opComma + 1, lenRhs);
		rhs[lenRhs] = '\0';

		//check if they are alias.
		int val1 = getIndexAsInt(lhs);
		int val2 = getIndexAsInt(rhs);
		
		char *tmp;
		int len;
		if (val1 != -1) {
			tmp = getValueAt(val1);
			len = strlen(tmp);
			memcpy(lhs, tmp, len);
			lhs[len] = '\0';
			tmp = NULL;
		}
		
		if (val2 != -1) {
			tmp = getValueAt(val2);
			len = strlen(tmp);
			memcpy(rhs, tmp, len);
			rhs[len] = '\0';
		}
		
		cmpResult = strCmp(lhs, rhs);
	}
}

void evalReg() {
	parseReg(getKeyword(), getExpression());
}

void splitInstruction(char *instruction) {
	trimString(instruction);
	char tmp[INSTRUCTIONSIZE];
	char tmp2[INSTRUCTIONSIZE];
	int len = 0;

	char *WHILE = strstr(instruction, ":while");

	if (!WHILE) {
		//is alias
		char *isalias = NULL;
		char *name;
		int size = getHeapSize();
		int i;
		for (i = 0; i != size; ++i) {
			name = getName(i);
			isalias = strstr(instruction, name);
			if (isalias) {
				//if it is a text string instead of a variable name.
				char *str = strstr(isalias, "\"");
				if (str) {
					return;
				}
				setExpression(instruction);
				evalName();
				break;
			}
		}
	}

	char *sysMemAllocHeap = strstr(instruction, "sysMemAllocHeap");
	char *alias = strstr(instruction, ":alias");
	char *DO = strstr(instruction, ":do");
	char *reg = strstr(instruction, ":reg");
	char *cmp = strstr(instruction, ":cmp");
	char *nequal = strstr(instruction, "nequal");

	if (sysMemAllocHeap) {
		setKeyword("sysMemAllocHeap");
		len = strlen(sysMemAllocHeap) - 15;
		memcpy(tmp, sysMemAllocHeap + 15, len);
		tmp[len] = '\0';
		setExpression(tmp);
		allocateMem();
	} 

	if (alias) {
		setKeyword(":alias");
		len = strlen(alias) -6;
		memcpy(tmp, alias +6, len);
		tmp[len] = '\0';
		setExpression(tmp);
		evalAlias();
	} 

	if (DO) {
		int totLen = 0;
		char *bracket = strstr(DO, "{");
		if (bracket) {
			setKeyword(":do");
			totLen = strlen(bracket) -1;
		} else {
			// Open bracket missing do CRASH!!
		}

		//Filter do from rest of expression, recursion.
		char *rest = strstr(DO +3, ":");
		if (rest) {
			len = strlen(rest);
			memcpy(tmp, rest, len);
			tmp[len] = '\0';
			splitInstruction(rest);
		}

		evalDo(totLen);
	}

	if (reg) {
		memcpy(tmp2, reg +1, 4);
		tmp2[4] = '\0';
		setKeyword(tmp2);

		len = strlen(reg) - 5;
		memcpy(tmp, reg + 5, len);
		tmp[len] = '\0';
		setExpression(tmp);
		evalReg();
	}

	if (WHILE) {
		setKeyword(":while");
		len = strlen(WHILE) - 6;
		memcpy(tmp, WHILE + 7, len);
		tmp[len] = '\0';
		setExpression(tmp);
		evalWhile();
		return;
	}

	if (cmp) {
		setKeyword(":cmp");
		len = strlen(cmp) - 3;
		memcpy(tmp, cmp + 4, len);
		tmp[len] = '\0';
		setExpression(tmp);
		evalCmp();

		if (cmpResult) {
			ignore = 0;
		} else {
			//ignore :nequal.
			ignore = 1;
		}
	}

	if (nequal && cmpResult) {
		ignore = 1;
	}
}

void getInstructions(char *code) {
	int comment = 0;
	char instruction[INSTRUCTIONSIZE];

	while (code[index] != '\0') {	
		if (code[index] == '/' && code[index +1] == '*') {
				comment = 1;
		} else if (code[index] == '*' && code[index +1] == '/') {
			comment = 0;
			start = index + 1;
		}

		if (code[index] == '}') { 
			ignore = 0; 
		}

		if (code[index] == ';' || code[index] == '{') {
			if (comment == 0 && ignore == 0) {
				end = index;
				int len = end - start;
				len = abs(len);
				memcpy(instruction, code + start, len);
				instruction[len] = '\0';
				start = end + 1;
				splitInstruction(instruction);
				printf("%s", instruction);
			}
			start = index;
		}
		++index;
	}
}

void freeMem(void) {
	free(code);
}
