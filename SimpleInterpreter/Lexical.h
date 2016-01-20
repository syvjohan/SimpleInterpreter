#ifndef LEXICAL_H
#define LEXICAL_H

#include <iostream>
#include <errno.h>

#include "Global.h"
#include "memoryLeak.h"
#include "Parser.h"

struct CallableUnit_s {
	char name[NAMESIZE];
	int startPos = -1;
	int endPos = -1;
};

struct Call_s {
	int pos;
	char name[NAMESIZE];
};

struct Loop_s {
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
		void getInstructions();
		void splitInstruction(char *instruction);

		void registerAllSubroutines(void);
		void registerAllStructs(void);

	private:
		void allocateMem();
		void createStack();

		void updateSubroutinesIndexes();
		void updateStructsIndexes();

		void expandSubroutineSize(void);
		void expandStructsSize();
		void expandCallsSize(void);

		void evalAlias();
		void evalDo(int len);
		void evalName();
		void evalWhile();
		void evalCall(int len);
		void evalIf();
		void evalStk();
		void evalPrint();
		void evalInclude();
		void evalCodeInsideStruct(char *code);
		void evalExpressionWithoutKeyword();

		char *code;
		size_t fileSize;

		int index = 0;
		int startIndex = 0;
		int endIndex = 0;
		int instructionLen = 0;

		Loop_s loop[200];
		int loopLen = 0;

		int ignore = 0;

		char *keyword;
		char *expression;
		int cmpResult = 0;

		//Before parsing: store subroutines indexes.
		CallableUnit_s *subroutines;
		int subroutinesLen = 0;
		int subroutinesMax = 0;

		//while parsing: calling subroutines.
		CallableUnit_s currentSubroutine;
		Call_s *calls;
		int callsMax = 0;
		int callsLen = -1;

		//Before parsing: store structs indexes.
		CallableUnit_s *structs;
		int structsLen = 0;
		int structsMax = 0;
		char currentStructName[NAMESIZE];

		Parser parser;
};
#endif //!LEXICAL_H