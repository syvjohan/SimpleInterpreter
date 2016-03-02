#include "Lexical.h"
#include "HelpHeaders.h"
#include "ErrorHeaders.h"
#include "Console.h"

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	SetConsoleSize();

	Partitioning::Lexical lexical;

	const char *path = "C:\\Programmering\\SimpleInterpreter\\syntax\\test.q";
	
	const size_t size = lexical.CalculateFileSize(path);
	lexical.fileSize = size;
	char *code = DBG_NEW char[size]; //deleted inside Lexical.cpp destructor.
	
	lexical.ReadFile(code, path);

	lexical.SetCode(code);
	code = lexical.RegisterAllIncludes();
	lexical.SetCode(code);

	lexical.RegisterAllStructs();

	lexical.RegisterAllSubroutines();

	//Need to be set again because of includes, struct and subroutines registration.
	lexical.SetCode(code);
	lexical.GetInstructions();

	system("pause");
	return 0;
}
