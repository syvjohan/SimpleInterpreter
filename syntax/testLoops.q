:print("outerDoWhileLoop");
:stk.pushAt(0, 0); /*Pushes 0 to memory address 4*/
:do { 

	:print("innerDoWhile");
	:stk.pushAt(5, 0);
	:do { 

		:print("2innerDoWhile");
		:stk.pushAt(10, 0);
		:do { 
			:print(:stk.getAt(10));
			:stk.pushAt(10, :stk.getAt(10) +1);

		} :while(:stk.getAt(10) != 3);
		:print("______________________________________________");

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


:print("WhileLoop");
:stk.pushAt(0, 0); /*Pushes 0 to memory address 4*/
:while(:stk.getAt(0) != 5) {

		:print("whileInnerLoop");
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