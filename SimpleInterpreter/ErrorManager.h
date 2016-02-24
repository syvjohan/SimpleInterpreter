#pragma once

#include "HelpClass.h"

namespace Global {
	struct File_s;
}

namespace Error {
	enum ERRORCODES;

	class ErrorManager {
		public:
			ErrorManager();
			~ErrorManager();

			static void ErrorCode(const ERRORCODES errorCode);
			static void SetInstruction(const char *instruction, const int index);
			static void SetRegisteredFiles(const Global::File_s *files, const int len);
			static void AddLine(const int index);

		private:
			static void PrintMessage(const char *errorCode, const char *msg);
			static char* FindFile();
			static int GetLines(char *name);
			static bool KeyPressed();

			static int index;
			static char instruction[INSTRUCTIONSIZE];
			static int linesInclude;
			static int linesMain;

			static Global::File_s files[MAXINCLUDEDFILES];
			static int lenFiles;
			static int fileIndex;	
			static int lenFileIndexes;
	};
}