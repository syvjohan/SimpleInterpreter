:print("--------------Assignment3.q--------------");

:sysMemAllocHeap 64;
:sysCreateStack 32;

:alias sum : #33;
:stk.pushTop( 2 );
:stk.pushTop( 3 );

:call add;
:print( sum );
:print( &sum ); 

:subroutine add {
	&sum = :stk.getTop() + :stk.getAt( 1 );
};

/* Below is only for debugg mode */
:if(sum == 5) { :print("Success"); } :else { :print("ERROR"); }
:if(&sum == #33) { :print("Success"); } :else { :print("ERROR"); }

:stk.popTop();
:stk.popTop();
