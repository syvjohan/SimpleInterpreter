#ifndef PARSER_H
#define PARSER_H

#include "Heap.h"
#include "Stack"
#include "Register.h"
#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct operator_s {
	int pos;
	char op[2];
	int len;
};

class Parser {
	public:
		Parser();
		~Parser();
		//char* parseManager(char *cStr, int isReference);
		
		char* regularExpression(char *expression);
		char* recursiveParser(char *expression);
		Alias_s findOutSecret(char *expression);

		//char* parseReg(char *keyword, char *expression);
		//char* parseHeap(char *cStr, int isReference);
		void allocateMem();

		Heap heap;

		void createStack(size_t stackSize);
		void stackPop();
		int stackPushAt(char *cStr);
		int stackPushTop(char *cStr);
		Alias_s stackGetAt(char *cStr);
		Alias_s stackGetTop();

		operator_s findOperator(const char *cStr, const int startPos);
		bool isNegativeNumber(const char *cStr);

		bool isAdress = false;

	private:
		//char* parseRegArg(char *keyword, char *arg);

		char tmpStr[INSTRUCTIONSIZE];
		char tmpLhs[INSTRUCTIONSIZE];
		char tmpRhs[INSTRUCTIONSIZE];
};

#endif //!PARSER_H