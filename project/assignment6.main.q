:print("--------------Assignment6.q--------------");

:sysMemAllocHeap 64;
:sysCreateStack 32;

:struct myStruct {
		:alias len : offset( 0 );
		:alias str : offset( 4 );
};

:alias pStruct : #32 = myStruct;
:call init;

:print( #32 ); /* it will print number 11. */ 
:print( pStruct.str + 4 ); /* it will print "o world". */
:print( &pStruct + 5 ); /* it will print #37  */

 :subroutine init {
 	&pStruct.str = "hello world";
 	&pStruct.len = 11;
 };
