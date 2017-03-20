:print("--------------Assignment1.q--------------");
/* Simulate  a "stack overflow"(use recursion). */

:sysMemAllocGlobal 64;
:sysCreateStack 32;

:call stackoverflow;

:subroutine stackoverflow {
	:stk.pushTop( 1 );
	:call stackoverflow; 
};
