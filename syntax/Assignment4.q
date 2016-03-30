:print("--------------Assignment4.q--------------");

:alias str : #36 = "this is a test str";
:alias len : #32;

:call strlen;

:subroutine strlen {
	:stk.pushTop( 0 );
	:while( str + :stk.getTop() != "\0" ) { 
		:stk.pushAt( 0, :stk.getTop() +1 );
	}

&len = :stk.getTop();
:stk.popTop();
};

/* Below is only for debugg mode */
:if(len == 19) { :print("Success"); } :else { :print("ERROR"); }
