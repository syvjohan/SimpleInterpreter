#include <stdio.h>
#include <stdlib.h>

#include "Lexical.h"
#include "Defs.h"

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Lexical lexical;

	const char *path = "C:\\Programmering\\SimpleInterpreter\\syntax\\do_while.txt";
	char *cStr = lexical.readFile(path);
	lexical.setCode(cStr);
	lexical.getAllSubroutines();
	lexical.getInstructions();

	//freeMem();
	system("pause");
	return 0;
}