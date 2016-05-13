:print("--------------TestLoops.q--------------");

:sysMemAllocHeap 64;
:sysCreateStack 32;


/*While loops*/
:print("thirdWhileLoop");
:stk.pushTop(0);
:while(:stk.getTop() != 1) {
		:print("thirdInnerWhileLoop");
		
	:print(:stk.getTop());
	:print("______________________________________________");

		:stk.pushTop(0);
		:while(:stk.getTop() != 2) {
			:print(:stk.getTop());
			:stk.pushAt(0, :stk.getTop() +1);
		}

	:stk.popTop();
	:stk.pushAt(0, :stk.getTop() +1);
}
:print("______________________________________________");
:stk.popTop();



:print("firstWhileLoop");
:stk.pushTop(0);
:while(:stk.getTop() != 5) {
	:print(:stk.getTop());
	:stk.pushAt(0, :stk.getTop() +1);
}
:print("______________________________________________");
:stk.popTop();



:print("secondWhileLoop");
:stk.pushTop(0);
:while(:stk.getTop() != 3) {
	:print(:stk.getTop());
	:stk.pushAt(0, :stk.getTop() +1);
}
:print("______________________________________________");
:stk.popTop();



:print("secondDoWhile");
:stk.pushTop(0);
:do { 
	:print(:stk.getTop());
	:stk.pushTop( :stk.getTop() +1);

} :while(:stk.getTop() != 3);
:print("______________________________________________");
:stk.popTop();



:print("thirdDoWhile");
:stk.pushTop(0);
:do { 
	:print(:stk.getTop());
	:stk.pushTop(:stk.getTop() +1);

} :while(:stk.getTop() != 3);
:print("______________________________________________");
:stk.popTop();


/*Do while loops*/
:print("firstDoWhile");
:stk.pushTop(0);
:do { 
	:print("firstInnerDoWhile");
	:stk.pushTop(0);
	:do { 
		:print(:stk.getTop());
		:stk.pushAt(0, :stk.getTop() +1);

	} :while(:stk.getTop() != 3);

	:stk.popTop();
	:print(:stk.getTop());
	:print("______________________________________________");
	:stk.pushAt(0, :stk.getTop() +1);

} :while(:stk.getTop() != 3);
:print("______________________________________________");
:stk.popTop();


