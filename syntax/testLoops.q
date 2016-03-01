:print("--------------TestLoops.q--------------");

/*While loops*/
:print("thirdWhileLoop");
:stk.pushAt(0, 0);
:while(:stk.getAt(0) != 1) {
		:print("thirdInnerWhileLoop");
		:stk.pushAt(5, 0);
		:while(:stk.getAt(5) != 3) {
			:print(:stk.getAt(5));
			:stk.pushAt(5, :stk.getAt(5) +1);
		}
		:print("______________________________________________");
	:print(:stk.getAt(0));
	:stk.pushAt(0, :stk.getAt(0) +1);
}
:print("______________________________________________");

:print("firstWhileLoop");
:stk.pushAt(0, 0);
:while(:stk.getAt(0) != 5) {
	:print(:stk.getAt(0));
	:stk.pushAt(0, :stk.getAt(0) +1);
}
:print("______________________________________________");

:print("secondwhileLoop");
:stk.pushAt(5, 0);
:while(:stk.getAt(5) != 3) {
	:print(:stk.getAt(5));
	:stk.pushAt(5, :stk.getAt(5) +1);
}
:print("______________________________________________");


/*Do while loops*/
:print("firstDoWhile");
:stk.pushAt(0, 0);
:do { 
	:print("firstInnerDoWhile");
	:stk.pushAt(5, 0);
	:do { 
		:print(:stk.getAt(5));
		:stk.pushAt(5, :stk.getAt(5) +1);

	} :while(:stk.getAt(5) != 3);
	:print("______________________________________________");

	:print(:stk.getAt(0));
	:stk.pushAt(0, :stk.getAt(0) +1);

} :while(:stk.getAt(0) != 3);
:print("______________________________________________");

:print("secondDoWhile");
:stk.pushAt(0, 0);
:do { 
	:print(:stk.getAt(0));
	:stk.pushAt(0, :stk.getAt(0) +1);

} :while(:stk.getAt(0) != 3);
:print("______________________________________________");

:print("thirdDoWhile");
:stk.pushAt(5, 0);
:do { 
	:print(:stk.getAt(5));
	:stk.pushAt(5, :stk.getAt(5) +1);

} :while(:stk.getAt(5) != 3);
:print("______________________________________________");