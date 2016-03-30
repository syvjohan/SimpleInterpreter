:print("--------------Assignment1.q--------------");
/* Simulate  a "stack overflow"(use recursion). */

:call stackoverflow;

:subroutine stackoverflow {
	:stk.pushTop( 1 );
	:call stackoverflow; 
};
