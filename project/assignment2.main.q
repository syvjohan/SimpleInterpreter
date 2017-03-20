:print("--------------Assignment2.q--------------");
/* Create a "buffer overflow"(use loop).. */

:sysMemAllocGlobal 32;

:alias str : #0 = "Hello World "; 

:do {
	&str = str + "again ";
	:print(str);

} :while(#33 != "a"); 
