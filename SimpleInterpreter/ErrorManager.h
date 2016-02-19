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
		void AddLine(int index);
	private:
		void PrintMessage(char *errorCode, char *msg);
		//int CalculateLinenumber(char *filename);
		char* FindFile();
		char instruction[INSTRUCTIONSIZE];
		int index = 0;
		int lines = 0;

		File_s files[MAXINCLUDEDFILES];
		int lenFiles = 0;
		int fileIndex = 0;

		Global global;
};

