
:stk.pushTop( 1 );
:stk.pushAt(  4, 0 );

:call calcFib; 

:subroutine calcFib {
	:print( :stk.getTop() );
	:stk.pushTop( :stk.getAt( 4 ) + :stk.getTop() );
	:stk.pushAt( 4, :stk.getAt(4) +1);

	if( :stk.getAt( 4 ) < 300 ) {
	 :call calcFib;
	} :else {
		:stk.popTop();
		:stk.popTop();
	}
};

:stk.popTop();
:stk.popTop();