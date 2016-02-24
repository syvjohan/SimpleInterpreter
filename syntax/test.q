:sysMemAllocHeap 64;
:sysCreateStack 32;

:include("C:\Programmering\SimpleInterpreter\syntax\testErrorCodes.q");
:alias kj : #32 = "Alias between includes";

:include("C:\Programmering\SimpleInterpreter\syntax\testIfAndAlias.q");

:include("C:\Programmering\SimpleInterpreter\syntax\testStack.q");
:alias kj : #32 = "Alias between includes";

:include("C:\Programmering\SimpleInterpreter\syntax\testStructs.q");
:alias kj : #32 = "Alias between includes";

:include("C:\Programmering\SimpleInterpreter\syntax\testLoops.q");
:include("C:\Programmering\SimpleInterpreter\syntax\testSubroutines.q");

:print(" ");
:print("End of test files!		");
