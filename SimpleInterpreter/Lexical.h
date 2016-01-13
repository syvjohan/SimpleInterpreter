#ifndef LEXICAL_H
#define LEXICAL_H

#include <iostream>
#include <errno.h>

#include "Global.h"
#include "memoryLeak.h"
#include "Scope.h"
#include "Parser.h"

struct subroutine_s {
	char name[NAMESIZE];
	int startPos;
	int endPos;
};

struct Pair {
	int start = -1;
	int end = -1;
	int stop = -1;
};

class Lexical {
	public:
		Lexical();
		~Lexical();

		char* readFile(const char *path);
		void setCode(char *cStr);
		void getInstructions(void);
		void splitInstruction(char *instruction);

		void registerAllSubroutines(void);

	private:
		void allocateMem();
		void createStack();
		void initlizeCurrentSubroutine();
		void expandSubroutineSize(void);

		void evalAlias();
		void evalDo(int len);
		void evalName();
		void evalWhile();
		void evalCall();
		void evalIf();
		void evalReg();
		void evalStk();
		void evalPrint();
		void evalAssert();
		void evalInclude();
		void evalExpressionWithoutKeyword();

		char *code;
		size_t fileSize;

		int index = 0;
		int startIndex = 0;
		int endIndex = 0;
		int instructionLen = 0;

		Pair loop[200];
		int loopLen = 0;

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