#ifndef LEXICAL_H
#define LEXICAL_H

#include "Defines.h"
#include "Global.h"
#include "Parser.h"

class Global;

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
	int type = -1;
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

		void resetIndex();

		void updateSubroutinesIndexes();
		void expandSubroutineSize(void);
		void expandCallsSize(void);
		void typedefSubroutines(char *searchName, char *extendName);
		void typedefSubroutinesMembers(char *searchName, char *extendName);

		void updateStructsIndexes();
		void expandStructsSize();

		void evalAlias();
		void evalDo();
		void evalName();
		void evalWhile();
		void evalCall(int len);
		void evalIf();
		void evalStk();
		void evalPrint();
		void evalInclude();
		void evalCodeInsideStruct(char *code);
		void evalExpressionWithoutKeyword();

		bool isCorrectFileType(char *cStr);
		bool isCorrectMainFileType(char *cStr);

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
		bool isInitializingStructs = false;

		Parser parser;
		Global global;
};
#endif //!LEXICAL_H