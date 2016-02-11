#pragma once

#include "Defines.h"
#include "Global.h"

enum ERRORCODES;

class ErrorManager {
	public:
		ErrorManager();
		~ErrorManager();

		void ErrorCode(ERRORCODES errorCode);
		void SetInstruction(char *instruction, int index);
		void SetRegisteredFiles(File_s *files, int len);
	private:
		void PrintMessage(char *errorCode, char *msg);
		char* FindFile();
		char instruction[INSTRUCTIONSIZE];
		int index = 0;

		File_s files[MAXINCLUDEDFILES];
		int lenFiles = 0;
};

