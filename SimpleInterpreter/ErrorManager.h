#pragma once

#include "Defines.h"

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
			static int GetLines();
			static bool KeyPressed();

			static char instruction[INSTRUCTIONSIZE];
			static int lines;

			static Global::File_s files[MAXINCLUDEDFILES];
			static int lenFiles;
			static int fileIndex;	

	};
}