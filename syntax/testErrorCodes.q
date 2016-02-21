
/*CODE_10*/
/*:alias HeapOverflow : #64 = "k";*/

/*CODE_11*/
/*:stk.pushAt(32, 12345);*/

/*CODE_12*/
/*:sysCreateStack 65;*/

/*CODE_13*/
/*:sysCreateStack 0;*/

/*CODE_14*/
/*:sysMemAllocHeap 0;*/

/*CODE_15*/
/*:sysMemAllocHeap 1;*/

/*CODE_16*/
/*:sysCreateStack 1;*/

/*CODE_30*/
/*:alias aliasOnStack : #31 = "crash";*/

/*CODE_31*/
/*:alias s_ : #32 = 12345;*/
/*:alias _a : #32 = 12345;*/
/*:alias 1s : #32 = 12345;*/

/*CODE_32*/
/*:print(aliasName); Not working fix later!!!*/

/*CODE_34*/
/*:alias string #32 = "hej";*/

/*CODE_35*/
/*:alias var : 32;*/
/*:alias var : 32 = "hej;*/
/*:alias var : #gfd;*/
/*:alias var : #gfd = "hej";*/

/*CODE_3510*/
/*:alias var : #32 = hej;*/
/*:alias var : #32 = 12g4k5;*/
/*:alias var : #32;
var = 12g4k5;*/

/*CODE_3511*/
/*:alias var : #32 = 9999999999;*/

/*CODE_40*/
/*:struct {
	:alias var : offset(0);
};*/

/*CODE_41*/
/*:struct testStruct 
	:alias var : offset(0);
};*/

/*CODE_42*/
/*:struct testStruct {
	:alias var : offset(0);*/

/*CODE_43*/
/*:struct testStruct
	:alias var : offset(0);*/

/*CODE_50*/
/*:subroutine {
	:alias var : offset(0);
};*/

/*CODE_51*/
/*:subroutine testSub 
	:alias var : offset(0);
};*/

/*CODE_52*/
/*:subroutine testSub {
	:alias var : offset(0);*/

/*CODE_53*/
/*:subroutine testSub
	:alias var : offset(0);*/

/*CODE_66*/
/*:alias var : #32 = "Hello";
var = 12345;*/

/*CODE_71*/
/* = 12345;*/

/*CODE_72*/
/*12345 = ;*/

/*CODE_90*/
/*OK. Lexical.cpp ReadFile(const char*){}*/

/*CODE_90*/
/*:include("C:\Programmering\SimpleInterpreter\syntax\wrongPath.q");*/

/*CODE_91*/
/*OK*/

/*CODE_100*/
/*:if(2 == "2");*/






