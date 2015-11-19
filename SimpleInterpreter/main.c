#include <stdio.h>
#include <stdlib.h>

#include "Lexical.h"
#include "Defs.h"


int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	const char *path = "C:\\Programmering\\SimpleInterpreter\\syntax\\file.txt";
	char *cStr = readFile(path);
	getInstructions(cStr);

	//freeMem();
	system("pause");
	return 0;
}