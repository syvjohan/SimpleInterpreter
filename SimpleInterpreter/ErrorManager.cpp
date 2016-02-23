#include "ErrorManager.h"
#include "ErrorCodes.h"
#include "memoryLeak.h"
#include "Console.h"
#include "HelpStructs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace Error {
	char ErrorManager::instruction[INSTRUCTIONSIZE];
	int ErrorManager::index = 0;
	int ErrorManager::lines = 0;

	Global::File_s ErrorManager::files[MAXINCLUDEDFILES];
	int ErrorManager::lenFiles = 0;
	int ErrorManager::fileIndex = 0;

	ErrorManager::ErrorManager() {}

	ErrorManager::~ErrorManager() {}

	void ErrorManager::PrintMessage(const char *errorCode, const char *msg) {
		ConsoleBackgroundAndTextColors(4, 0);
		printf("\nERROR: %s.", errorCode);

		char *name = ErrorManager::FindFile();

		printf("\nIn file: %s", name);
		printf("\nLine: %i.", ErrorManager::lines);

		printf("\nDescription: %s\n", msg);
		printf("\n%s\n\n", ErrorManager::instruction);

		system("pause");
	}

	void ErrorManager::SetInstruction(const char *inst, const int i) {
		int len = strlen(inst);
		memcpy(ErrorManager::instruction, inst, len);
		ErrorManager::instruction[len] = '\0';

		ErrorManager::index = i;
	}

	void ErrorManager::SetRegisteredFiles(const Global::File_s *f, const int len) {
		memcpy(ErrorManager::files, f, len * sizeof(Global::File_s));
		ErrorManager::lenFiles = len;
	}

	char* ErrorManager::FindFile() {
		/*for (int i = 0; i != lenFiles; ++i) {
			if (index > files[i].startPos && index < files[i].endPos) {
			return files[i].name;
			}
			}
			return "main.q";*/

		if (ErrorManager::fileIndex == -1) {
			return ".main.q";
		}

		return ErrorManager::files[fileIndex - 1].name;

	}

	void ErrorManager::AddLine(const int index) {
		for (int i = ErrorManager::fileIndex; i != ErrorManager::lenFiles; ++i) {
			if (index >= ErrorManager::files[i].startPos && index <= ErrorManager::files[i].endPos) {
				ErrorManager::fileIndex++;
				ErrorManager::lines = 1; //restart counter because we enter a new file.
				break;
			}
		}
		ErrorManager::lines++;
	}

	void ErrorManager::ErrorCode(const ERRORCODES errorCode) {
		switch (errorCode) {
			case CODE_10:
				ErrorManager::PrintMessage("CODE_10", "Overflow: Buff overflow, cannot access a memory address that has not been allocated.");
				break;
			case CODE_11:
				ErrorManager::PrintMessage("CODE_11", "Overflow: Stackoverflow, cannot write to memory outside the Stack.");
				break;
			case CODE_12:
				ErrorManager::PrintMessage("CODE_12", "Initialization Stack: Stack cannot be bigger then the Heap.");
				break;
			case CODE_13:
				ErrorManager::PrintMessage("CODE_13", "Initialization Stack: You cannot create a Stack that is smaller then one byte.");
				break;
			case CODE_14:
				ErrorManager::PrintMessage("CODE_14", "Initialization Heap: Heap need to be minimum one byte");
				break;
			case CODE_15:
				ErrorManager::PrintMessage("CODE_15", "Heap: You can only initilize Heap once.");
				break;
			case CODE_16:
				ErrorManager::PrintMessage("CODE_16", "Stack: You can only create one Stack.");
				break;
			case CODE_20:
				ErrorManager::PrintMessage("CODE_20", "No code inserted");
				break;
			case CODE_21:
				ErrorManager::PrintMessage("CODE_21", "No code inserted");
				break;
			case CODE_22:
				ErrorManager::PrintMessage("CODE_22", "No code inserted");
				break;
			case CODE_23:
				ErrorManager::PrintMessage("CODE_23", "No code inserted");
				break;
			case CODE_24:
				ErrorManager::PrintMessage("CODE_24", "No code inserted");
				break;
			case CODE_25:
				ErrorManager::PrintMessage("CODE_25", "No code inserted");
				break;
			case CODE_26:
				ErrorManager::PrintMessage("CODE_26", "No code inserted");
				break;
			case CODE_30:
				ErrorManager::PrintMessage("CODE_30", "Alias: You cannot create alias on Stack.");
				break;
			case CODE_31:
				ErrorManager::PrintMessage("CODE_31", "Alias: Name need to contain least two character and start with a letter, symbols are not allowed.");
				break;
			case CODE_32:
				ErrorManager::PrintMessage("CODE_32", "Alias: Cannot find name on Heap.");
				break;
			case CODE_33:
				ErrorManager::PrintMessage("CODE_33", "No code inserted");
				break;
			case CODE_34:
				ErrorManager::PrintMessage("CODE_34", "Alias: Missing ':'.");
				break;
			case CODE_35:
				ErrorManager::PrintMessage("CODE_35", "Alias: address need to start with '#' and fallowing character need to be digits.");
				break;
			case CODE_3510:
				ErrorManager::PrintMessage("CODE_3510", "Alias: Datatype int, Int cannot contain letters.");
				break;
			case CODE_3511:
				ErrorManager::PrintMessage("CODE_3511", "Alias: Datatype int, Int cant be bigger than INT_MAX.");
				break;
			case CODE_3512:
				ErrorManager::PrintMessage("CODE_3512", "No code inserted");
				break;
			case CODE_3520:
				ErrorManager::PrintMessage("CODE_3520", "No code inserted");
				break;
			case CODE_3521:
				ErrorManager::PrintMessage("CODE_3521", "No code inserted");
				break;
			case CODE_36:
				ErrorManager::PrintMessage("CODE_36", "No code inserted");
				break;
			case CODE_40:
				ErrorManager::PrintMessage("CODE_40", "Struct: Struct name missing.");
				break;
			case CODE_41:
				ErrorManager::PrintMessage("CODE_41", "Struct: Missing open bracket.");
				break;
			case CODE_42:
				ErrorManager::PrintMessage("CODE_42", "Struct: Missing close bracket.");
				break;
			case CODE_43:
				ErrorManager::PrintMessage("CODE_43", "Struct: Missing open and close brackets.");
				break;
			case CODE_44:
				ErrorManager::PrintMessage("CODE_44", "Struct: struct name length is maximum 50 characters, or there is no open and close brackets.");
				break;
			case CODE_45:
				ErrorManager::PrintMessage("CODE_45", "No code inserted");
				break;
			case CODE_46:
				ErrorManager::PrintMessage("CODE_46", "No code inserted");
				break;
			case CODE_50:
				ErrorManager::PrintMessage("CODE_50", "Subroutine: Subroutine name missing.");
				break;
			case CODE_51:
				ErrorManager::PrintMessage("CODE_51", "Subroutine: Missing open bracket.");
				break;
			case CODE_52:
				ErrorManager::PrintMessage("CODE_52", "Subroutine: Missing close bracket.");
				break;
			case CODE_53:
				ErrorManager::PrintMessage("CODE_53", "No code inserted");
				break;
			case CODE_54:
				ErrorManager::PrintMessage("CODE_54", "Subroutine: struct name length is maximum 50 characters, or there is no open and close brackets.");
				break;
			case CODE_55:
				ErrorManager::PrintMessage("CODE_55", "No code inserted");
				break;
			case CODE_56:
				ErrorManager::PrintMessage("CODE_56", "No code inserted");
				break;
			case CODE_60:
				ErrorManager::PrintMessage("CODE_60", "No code inserted");
				break;
			case CODE_61:
				ErrorManager::PrintMessage("CODE_61", "No code inserted");
				break;
			case CODE_62:
				ErrorManager::PrintMessage("CODE_62", "No code inserted");
				break;
			case CODE_63:
				ErrorManager::PrintMessage("CODE_63", "No code inserted");
				break;
			case CODE_64:
				ErrorManager::PrintMessage("CODE_64", "No code inserted");
				break;
			case CODE_65:
				ErrorManager::PrintMessage("CODE_65", "No code inserted");
				break;
			case CODE_66:
				ErrorManager::PrintMessage("CODE_66", "Expression: Lhs need to be a modifiable value(&);");
				break;
			case CODE_70:
				ErrorManager::PrintMessage("CODE_70", "No code inserted");
				break;
			case CODE_71:
				ErrorManager::PrintMessage("CODE_71", "Expression: Lhs is undefined.");
				break;
			case CODE_72:
				ErrorManager::PrintMessage("CODE_72", "Expression: Rhs is undefined.");
				break;
			case CODE_73:
				ErrorManager::PrintMessage("CODE_73", "No code inserted");
				break;
			case CODE_74:
				ErrorManager::PrintMessage("CODE_74", "No code inserted");
				break;
			case CODE_75:
				ErrorManager::PrintMessage("CODE_75", "No code inserted");
				break;
			case CODE_76:
				ErrorManager::PrintMessage("CODE_76", "Undefined behavior: Unknown keyword");
				break;
			case CODE_80:
				ErrorManager::PrintMessage("CODE_80", "Undefined behavior: Wrong syntax.");
				break;
			case CODE_81:
				ErrorManager::PrintMessage("CODE_81", "Undefined behavior: Could not identify the error");
				break;
			case CODE_82:
				ErrorManager::PrintMessage("CODE_82", "No code inserted");
				break;
			case CODE_83:
				ErrorManager::PrintMessage("CODE_83", "No code inserted");
				break;
			case CODE_84:
				ErrorManager::PrintMessage("CODE_84", "No code inserted");
				break;
			case CODE_85:
				ErrorManager::PrintMessage("CODE_85", "No code inserted");
				break;
			case CODE_86:
				ErrorManager::PrintMessage("CODE_86", "No code inserted");
				break;
			case CODE_90:
				ErrorManager::PrintMessage("CODE_90", "Wrong file path.");
				break;
			case CODE_91:
				ErrorManager::PrintMessage("CODE_91", "Can only include a maximum of 100 files in the same project.");
				break;
			case CODE_100:
				ErrorManager::PrintMessage("CODE_100", "Compare: Cannot compare digits and string");
				break;
			case CODE_101:
				ErrorManager::PrintMessage("CODE_101", "No code inserted");
				break;
			case CODE_102:
				ErrorManager::PrintMessage("CODE_102", "No code inserted");
				break;
			case CODE_103:
				ErrorManager::PrintMessage("CODE_103", "No code inserted");
				break;
			case CODE_104:
				ErrorManager::PrintMessage("CODE_104", "No code inserted");
				break;
			case CODE_105:
				ErrorManager::PrintMessage("CODE_105", "No code inserted");
				break;
			case CODE_106:
				ErrorManager::PrintMessage("CODE_106", "No code inserted");
				break;
		}
	}
}