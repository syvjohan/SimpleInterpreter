#include "Lexical.h"
#include "HelpHeaders.h"
#include "Console.h"

bool KeyPressed();

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SetConsoleSize();

	Partitioning::Lexical lexical;

	const char *path = "C:\\Programmering\\SimpleInterpreter\\syntax\\test.q";
	char *code = lexical.ReadFile(path);

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
