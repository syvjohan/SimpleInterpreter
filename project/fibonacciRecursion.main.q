/* Fibonacci implemented with recursion and Stack */

:sysMemAllocHeap 128;
:sysCreateStack 128; 

:call fibonacci;

:subroutine fibonacci {

	:if( :stk.getSize() == 0 ) {
		:stk.pushTop( 0 );
		:stk.pushTop( 1 );
	} :else {
		:stk.pushTop( :stk.getTop() + :stk.getAt( 1 ) );
	}

	:if( :stk.getSize() < 30 ) {
		:call fibonacci; /* Recursive call */
	}
};