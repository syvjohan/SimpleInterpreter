:sysMemAllocHeap 64;
:sysCreateStack 32;

:alias var : #32 = "Alias between includes";

:include("C:\Programmering\SimpleInterpreter\syntax\testMathExpression.q");

:include("C:\Programmering\SimpleInterpreter\syntax\testStack.q");
:alias var : #32 = "Alias between includes";

:include("C:\Programmering\SimpleInterpreter\syntax\testIfAndAlias.q"); 

:include("C:\Programmering\SimpleInterpreter\syntax\testLoops.q");
:alias var : #32 = "Alias between includes";

:stk.pushAt(5, 0);
:while(:stk.getAt(5) != 3) { 
	:stk.pushAt(5, :stk.getAt(5) +1);
}

:include("C:\Programmering\SimpleInterpreter\syntax\testSubroutines.q");
:alias var : #32 = "Alias between includes";

var = 1234;

/*:include("C:\Programmering\SimpleInterpreter\syntax\testErrorCodes.q");*/

:print(" ");
:print("End of test files!		");
