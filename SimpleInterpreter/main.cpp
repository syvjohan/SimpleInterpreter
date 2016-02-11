#include <stdio.h>
#include <stdlib.h>

#include "ErrorManager.h"
#include "Lexical.h"
#include "memoryLeak.h"

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Lexical lexical;

	const char *path = "C:\\Programmering\\SimpleInterpreter\\syntax\\test.q";
	char *code = lexical.readFile(path);

	lexical.setCode(code);
	code = lexical.registerAllIncludes();
	lexical.setCode(code);
	lexical.registerAllStructs();
	lexical.registerAllSubroutines();

	//Need to be set again because of struct and subroutines registration.
	lexical.setCode(code);
	lexical.getInstructions();

	system("pause");
	return 0;
}