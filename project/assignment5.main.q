:print("--------------Assignment5.q--------------");

:sysMemAllocHeap 64;
:sysCreateStack 32;

:alias val : #32;

:print( val ); /* alias points to address #32 */
:print ( &val ); 