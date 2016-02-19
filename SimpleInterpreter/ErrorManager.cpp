#include "ErrorManager.h"
#include "ErrorCodes.h"
#include "memoryLeak.h"
#include "Global.h"
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>

ErrorManager::ErrorManager() {}

ErrorManager::~ErrorManager() {}

void ErrorManager::PrintMessage(char *errorCode, char *msg) {
	printf("\nERROR: %s.", errorCode);

	char *name = FindFile();

	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hConsole, 100);
	//SetConsoleTitle("Interpreter Q");

	printf("\nIn file: %s", name);
	
	printf("\nLine: %i.", lines);
	printf("\nDescription: %s\n", msg);
	printf("\n%s\n\n", instruction);
	system("pause");
}

void ErrorManager::SetInstruction(char *inst, int i) {
	int len = strlen(inst);
	memcpy(instruction, inst, len);
	instruction[len] = '\0';

	index = i;
}

void ErrorManager::SetRegisteredFiles(File_s *f, int len) {
	memcpy(files, f, len * sizeof(File_s));
	lenFiles = len;
}

char* ErrorManager::FindFile() {
	for (int i = 0; i != lenFiles; ++i) {
		if (index > files[i].startPos && index < files[i].endPos) {
			return files[i].name;
		}
	}
	return "main.q";
}

void ErrorManager::AddLine(int index) {
	for (int i = fileIndex; i != lenFiles; ++i) {
		if (index >= files[i].startPos && index <= files[i].endPos) {
			++fileIndex;
			lines = 1; //restart counter because we enter a new file.
			break;
		}
	}
	++lines;
}

void ErrorManager::ErrorCode(ERRORCODES errorCode) {
	switch (errorCode) {
		case CODE_10:
			PrintMessage("CODE_10", "Overflow: Buff overflow, cannot access a memory address that has not been allocated.");
			break;
		case CODE_11:
			PrintMessage("CODE_11", "Overflow: Stackoverflow, cannot write to memory outside the Stack.");
			break;
		case CODE_12:
			PrintMessage("CODE_12", "Initialization Stack: Stack cannot be bigger then the Heap.");
			break;
		case CODE_13:
			PrintMessage("CODE_13", "Initialization Stack: You cannot create a Stack that is smaller then one byte.");
			break;
		case CODE_14:
			PrintMessage("CODE_14", "Initialization Heap: Heap need to be minimum one byte");
			break;
		case CODE_15:
			PrintMessage("CODE_15", "Heap: You can only initilize Heap once.");
			break;
		case CODE_16:
			PrintMessage("CODE_16", "Stack: You can only create one Stack.");
			break;
		case CODE_20:
			PrintMessage("CODE_20", "Alias and Stack: You cannot create alias on Stack.");
			break;
		case CODE_21:
			PrintMessage("CODE_21", "No code inserted");
			break;
		case CODE_22:
			PrintMessage("CODE_22", "No code inserted");
			break;
		case CODE_23:
			PrintMessage("CODE_23", "No code inserted");
			break;
		case CODE_24:
			PrintMessage("CODE_24", "No code inserted");
			break;
		case CODE_25:
			PrintMessage("CODE_25", "No code inserted");
			break;
		case CODE_26:
			PrintMessage("CODE_26", "No code inserted");
			break;
		case CODE_30:
			PrintMessage("CODE_30", "Alias: Can only be created on the Heap.");
			break;
		case CODE_31:
			PrintMessage("CODE_31", "Alias: Name need to contain least one character and start with a letter.");
			break;
		case CODE_32:
			PrintMessage("CODE_32", "Alias: Cannot find name on Heap.");
			break;
		case CODE_33:
			PrintMessage("CODE_33", "Alias: Cannot verify datatype.");
			break;
		case CODE_34:
			PrintMessage("CODE_34", "Alias: Missing ':'.");
			break;
		case CODE_35:
			PrintMessage("CODE_35", "Alias: address need to start with '#' and fallowing character need to be digits.");
			break;
		case CODE_3510:
			PrintMessage("CODE_3510", "Alias: Datatype int, Int cannot contain letters.");
			break;
		case CODE_3511:
			PrintMessage("CODE_3511", "Alias: Datatype int, Int need to be bigger than INT_MIN or less then INT_MAX.");
			break;
		case CODE_3512:
			PrintMessage("CODE_3512", "No code inserted");
			break;
		case CODE_3520:
			PrintMessage("CODE_3520", "Alias: Datatype string, Missing \" ");
			break;
		case CODE_3521:
			PrintMessage("CODE_3521", "No code inserted");
			break;
		case CODE_36:
			PrintMessage("CODE_36", "No code inserted");
			break;
		case CODE_40:
			PrintMessage("CODE_40", "Struct: Struct name does not exist.");
			break;
		case CODE_41:
			PrintMessage("CODE_41", "Struct: Missing open bracket.");
			break;
		case CODE_42:
			PrintMessage("CODE_42", "Struct: Missing close bracket.");
			break;
		case CODE_43:
			PrintMessage("CODE_43", "No code inserted");
			break;
		case CODE_44:
			PrintMessage("CODE_44", "No code inserted");
			break;
		case CODE_45:
			PrintMessage("CODE_45", "No code inserted");
			break;
		case CODE_46:
			PrintMessage("CODE_46", "No code inserted");
			break;
		case CODE_50:
			PrintMessage("CODE_50", "Subroutine: Subroutine name does not exist.");
			break;
		case CODE_51:
			PrintMessage("CODE_51", "Subroutine: Missing open bracket.");
			break;
		case CODE_52:
			PrintMessage("CODE_52", "Subroutine: Missing close bracket.");
			break;
		case CODE_53:
			PrintMessage("CODE_53", "No code inserted");
			break;
		case CODE_54:
			PrintMessage("CODE_54", "No code inserted");
			break;
		case CODE_55:
			PrintMessage("CODE_55", "No code inserted");
			break;
		case CODE_56:
			PrintMessage("CODE_56", "Loop and statement: Open bracket is missing.");
			break;
		case CODE_60:
			PrintMessage("CODE_60", "Loop and statement: Missing compare operator");
			break;
		case CODE_61:
			PrintMessage("CODE_61", "Loop and statement: Wrong syntax cannot compare address with value.");
			break;
		case CODE_62:
			PrintMessage("CODE_62", "No code inserted");
			break;
		case CODE_63:
			PrintMessage("CODE_63", "No code inserted");
			break;
		case CODE_64:
			PrintMessage("CODE_64", "No code inserted");
			break;
		case CODE_65:
			PrintMessage("CODE_65", "No code inserted");
			break;
		case CODE_66:
			PrintMessage("CODE_66", "Expression: Lhs need to be a modifiable value(&);");
			break;
		case CODE_70:
			PrintMessage("CODE_70", "Expression: Address can only contain digits and need to start with #");
			break;
		case CODE_71:
			PrintMessage("CODE_71", "Expression: Lhs is undefined.");
			break;
		case CODE_72:
			PrintMessage("CODE_72", "Expression: Rhs is undefined.");
			break;
		case CODE_73:
			PrintMessage("CODE_73", "No code inserted");
			break;
		case CODE_74:
			PrintMessage("CODE_74", "No code inserted");
			break;
		case CODE_75:
			PrintMessage("CODE_75", "No code inserted");
			break;
		case CODE_76:
			PrintMessage("CODE_76", "Undefined behavior: Unknown keyword");
			break;
		case CODE_80:
			PrintMessage("CODE_80", "Undefined behavior: Wrong syntax.");
			break;
		case CODE_81:
			PrintMessage("CODE_81", "Undefined behavior: Could not identify the error");
			break;
		case CODE_82:
			PrintMessage("CODE_82", "No code inserted");
			break;
		case CODE_83:
			PrintMessage("CODE_83", "No code inserted");
			break;
		case CODE_84:
			PrintMessage("CODE_84", "No code inserted");
			break;
		case CODE_85:
			PrintMessage("CODE_85", "No code inserted");
			break;
		case CODE_86:
			PrintMessage("CODE_86", "No code inserted");
			break;
		case 90:
			PrintMessage("CODE_90", "Wrong file path.");
			break;
		case 91:
			PrintMessage("CODE_91", "Can only include a maximum of 100 files in the same project.");
			break;
		case CODE_100:
			PrintMessage("CODE_100", "Compare: Cannot compare digits and string");
			break;
		case CODE_101:
			PrintMessage("CODE_73", "No code inserted");
			break;
		case CODE_102:
			PrintMessage("CODE_73", "No code inserted");
			break;
		case CODE_103:
			PrintMessage("CODE_73", "No code inserted");
			break;
		case CODE_104:
			PrintMessage("CODE_73", "No code inserted");
			break;
		case CODE_105:
			PrintMessage("CODE_73", "No code inserted");
			break;
		case CODE_106:
			PrintMessage("CODE_73", "No code inserted");
			break;
	}
}