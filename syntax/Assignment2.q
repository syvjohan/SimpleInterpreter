:print("--------------Assignment2.q--------------");
/* Simulate a "buffer overflow"(use loop).. */

:alias str : #33 = "Hello World"; 
:stk.pushTop( 0 );

:do {

	:stk.pushAt( 0, :stk.getTop() + 6 );
	&str + :stk.getTop() = "again ";

} :while(&str + :stk.getTop() != 64); 
