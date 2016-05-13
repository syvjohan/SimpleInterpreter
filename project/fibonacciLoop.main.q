/* Fibonacci implemented with while loop and Stack */

:sysMemAllocHeap 128;
:sysCreateStack 128;

:do {
	:if(:stk.getSize() == 0) {
		:stk.pushTop( 0 );
		:stk.pushTop( 1 );

	} :else {
		:stk.pushTop( :stk.getTop() + :stk.getAt( 1 ) );
	}
} :while(:stk.getSize() < 30);