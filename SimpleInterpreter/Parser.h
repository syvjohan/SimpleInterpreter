#ifndef PARSER_H
#define PARSER_H

#include "Heap.h"
#include "Register.h"
#include "Global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct operator_s {
	int pos;
	char op;
};

class Parser {
	public:
		Parser();
		~Parser();
		char* parseManager(char *cStr, int isReference);
		char* parseReg(char *keyword, char *expression);
		char* parseHeap(char *cStr, int isReference);
		int checkForAlpha(const char *cStr);
		int checkForDigits(const char *cStr);
		void allocateMem();
		Heap heap;

	private:
		operator_s findOperator(const char *cStr, const int startPos);
		void trim(char *cStr);
		void trimHeap(char *cStr);
		char* parseRegArg(char *keyword, char *arg);

		Register reg;
};

#endif //!PARSER_H