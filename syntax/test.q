:sysMemAllocHeap 64;
:sysCreateStack 32;

:include("C:\Programmering\SimpleInterpreter\syntax\testStack.q");
:include("C:\Programmering\SimpleInterpreter\syntax\testIfAndAlias.q");
:include("C:\Programmering\SimpleInterpreter\syntax\testMathExpression.q");
:include("C:\Programmering\SimpleInterpreter\syntax\testLoops.q");
:include("C:\Programmering\SimpleInterpreter\syntax\testSubroutines.q");
/*:include("C:\Programmering\SimpleInterpreter\syntax\testErrorCodes.q");*/

:alias kj : #32 = "Alias between includes";

:print(" ");
:print("End of test files!		");
