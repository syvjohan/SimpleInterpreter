#ifndef LEXICAL_H
#define LEXICAL_H

#include "Defines.h"
#include "Parser.h"
#include "HelpStructs.h" //TODO! Remove this include Helpstructs.h are forward declared but it aint working for some strange reason....

namespace Global {
	struct File_s;
	struct CallableUnit_s;
	struct Call_s;
	struct Loop_s;
}

namespace Partitioning {
	class Lexical {
	public:
		Lexical();
		~Lexical();

		void ReadFile( char *buffer, const char *path );
		bool IsMainFile( const char *path );
		size_t CalculateFileSize( const char *path );
		void SetCode( const char *cStr );
		void GetInstructions();
		void SplitInstruction( char *instruction );

		void RegisterAllSubroutines();
		void RegisterAllStructs();
		char* RegisterAllIncludes();
		void RegisterFile( const int start, const int end, const char *name );
		int CalculateLinenumbersInFile( const int start, const int end );

		size_t fileSize;
	private:
		void AllocateMem();
		void CreateStack();

		void ResetIndex();

		void UpdateSubroutinesIndexes();
		void ExpandSubroutineSize();
		void ExpandCallsSize();
		void TypedefSubroutines( const char *searchName, const char *extendName );
		void TypedefSubroutinesMembers( const char *searchName, const char *extendName );

		void UpdateStructsIndexes();
		void ExpandStructsSize();

		void EvalAlias();
		void EvalDo();
		void EvalName();
		void EvalWhile();
		void EvalCall( const int len );
		void EvalIf();
		void EvalStk();
		void EvalPrint();
		void EvalCodeInsideStruct( const char *code );
		void EvalExpressionWithoutKeyword();

		bool IsCorrectFileType( const char *cStr );

		void ResetLoopArray();

		char *code;

		int index = 0;
		int startIndex = 0;
		int endIndex = 0;
		int instructionLen = 0;

		Global::Loop_s loop[ LOOPMAX ];
		int loopLen = 0;

		int ignore = 0;

		char *keyword;
		char *expression;
		int cmpResult = 0;

		//Before parsing: store subroutines indexes.
		Global::CallableUnit_s *subroutines;
		int subroutinesLen = 0;
		int subroutinesMax = 0;

		//while parsing: calling subroutines.
		Global::CallableUnit_s currentSubroutine;
		Global::Call_s *calls;
		int callsMax = 0;
		int callsLen = -1;

		//Before parsing: store structs indexes.
		Global::CallableUnit_s *structs;
		int structsLen = 0;
		int structsMax = 0;
		char currentStructName[ NAMESIZE ];
		bool isInitializingStructs = false;

		//track files for indexing.
		Global::File_s files[ MAXINCLUDEDFILES ];
		int lenFiles = 0;

		Partitioning::Parser parser;
	};
}
#endif //!LEXICAL_H