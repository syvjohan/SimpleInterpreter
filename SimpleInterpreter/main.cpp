#include "Manager.h"
#include "HelpHeaders.h"

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Manager manager;
	manager.Initialize();

	return 0;
}
