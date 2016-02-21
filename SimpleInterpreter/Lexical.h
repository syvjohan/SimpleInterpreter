#ifndef LEXICAL_H
#define LEXICAL_H

#include "Defines.h"
#include "Global.h"
#include "Parser.h"
#include "ErrorManager.h"

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

class Global;
class ErrorManager;

class Lexical {
	public:
		Lexical();
		~Lexical();

		char* ReadFile(const char *path);
		void SetCode(char *cStr);
		void GetInstructions();
		void SplitInstruction(char *instruction);

		void RegisterAllSubroutines(void);
		void RegisterAllStructs(void);
		char* RegisterAllIncludes(void);
		void RegisterFile(int start, int end, char *name);
		int CalculateLinenumbersInFile(int start, int end);

	private:
		void AllocateMem();
		void CreateStack();

		void ResetIndex();

		void UpdateSubroutinesIndexes();
		void ExpandSubroutineSize(void);
		void ExpandCallsSize(void);
		void TypedefSubroutines(char *searchName, char *extendName);
		void TypedefSubroutinesMembers(char *searchName, char *extendName);

		void UpdateStructsIndexes();
		void ExpandStructsSize();

		void EvalAlias();
		void EvalDo();
		void EvalName();
		void EvalWhile();
		void EvalCall(int len);
		void EvalIf();
		void EvalStk();
		void EvalPrint();
		void EvalCodeInsideStruct(char *code);
		void EvalExpressionWithoutKeyword();

		bool IsCorrectFileType(char *cStr);
		bool IsCorrectMainFileType(char *cStr);

		void ResetLoopArray();

		char *code;
		size_t fileSize;

		int index = 0;
		int startIndex = 0;
		int endIndex = 0;
		int instructionLen = 0;

		Loop_s loop[LOOPMAX];
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

		//track files for indexing.
		File_s files[MAXINCLUDEDFILES];
		int lenFiles = 0;

		Parser parser;
		Global global;
		ErrorManager errorManager;
};
#endif //!LEXICAL_H