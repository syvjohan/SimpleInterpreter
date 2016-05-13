:print("--------------Assignment4.q--------------");

:sysMemAllocHeap 64;
:sysCreateStack 32;

:alias str : #36 = "this is a test string";
:alias len : #32 = 0;

:call strlen;

:subroutine strlen {
	:stk.pushTop(len);
	:stk.pushTop(str);

	:while(:stk.getSize() == 2) {
		:stk.pushAt(1, :stk.getAt(1) +1);
		:stk.pop();
	}

	&len = :stk.getTop();
};


/* Below is only for debugg mode */
/*:if(len == 21) { :print("Success"); } :else { :print("ERROR"); }*/
