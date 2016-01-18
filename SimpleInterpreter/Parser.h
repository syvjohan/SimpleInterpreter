#ifndef PARSER_H
#define PARSER_H

#include "Heap.h"
#include "Stack"
#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Operator_s {
	int pos;
	char op[2];
	int len;
};

class Parser {
	public:
		Parser();
		~Parser();
		
		char* regularExpression(char *expression);
		char* calculateResult(char *expression);
		Alias_s parseKeywords(char *expression);

		void allocateMem();

		Heap heap;

		void createStack(size_t stackSize);
		void stackPop();
		int stackPushAt(char *cStr);
		int stackPushTop(char *cStr);
		Alias_s stackGetAt(char *cStr);
		Alias_s stackGetTop();

		Operator_s findOperator(const char *cStr, const int startPos);
		bool isNegativeNumber(const char *cStr);

		bool isAdress = false;

	private:
		char tmpStr[INSTRUCTIONSIZE];
		char tmpLhs[INSTRUCTIONSIZE];
		char tmpRhs[INSTRUCTIONSIZE];
};

#endif //!PARSER_H