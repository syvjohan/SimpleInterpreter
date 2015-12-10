#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "Global.h"
#include "Defs.h"
#include "Scope.h"
#include "Parser.h"

#define NAMESIZE 30
#define ADDRESSSIZE 20
#define VALUESIZE 50
#define INSTRUCTIONSIZE 100

struct subroutine_s {
	char name[NAMESIZE];
	int startPos;
	int endPos;
};

class Lexical {
	public:
		Lexical();
		~Lexical();

		char* readFile(const char *path);
		void setCode(char *cStr);
		void getInstructions(void);
		void splitInstruction(char *instruction);

		void getAllSubroutines(void);

	private:
		void trimString(char *cStr);
		void removeParanthesis(char *cStr);
		char* getKeyword();
		void setKeyword(char *cStr);
		char* getExpression();
		void setExpression(char *cStr);
		void allocateMem();
		void initlizeCurrentSubroutine();
		void expandSubroutineSize(void);
		void evalAlias();
		void evalDo(int len);
		void evalName();
		void evalWhile();
		void evalCall();
		void evalSubroutine(char *expression);
		void evalPrintv(char *expression);
		void evalPrinta(char *expression);
		void evalIf();
		void evalReg();


		char *code = NULL;
		size_t fileSize;

		int index = 0;
		int startIndex = 0;
		int endIndex = 0;

		int ignore = 0;

		char keyword[NAMESIZE];
		char expression[VALUESIZE];
		int cmpResult = 0;

		subroutine_s *subroutines;
		int subroutinesLen = 0;
		int subroutinesMax = 0;

		subroutine_s currentSubroutine;
		int oldIndex = -1;

		Parser parser;
		Scope scope;
};

#endif //!LEXICAL_H