#ifndef LEXICAL_H
#define LEXICAL_H

#include <iostream>
#include <errno.h>

#include "Global.h"
#include "Defs.h"
#include "Scope.h"
#include "Parser.h"

#define NAMESIZE 30
#define ADDRESSSIZE 20
#define VALUESIZE 50
#define TYPESIZE 7
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
		void allocateMem();
		void createStack();
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
		void evalStk();
		void evalPrintv();
		void evalPrinta();

		char *code = NULL;
		size_t fileSize;

		int index = 0;
		int startIndex = 0;
		int endIndex = 0;

		int ignore = 0;

		char *keyword;
		char *expression;
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