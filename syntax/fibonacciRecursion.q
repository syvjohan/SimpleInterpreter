/* Fibinacci implemented with recursion and Stack */

:stk.pushTop( 0 ); /* Sum */
:stk.pushAt(4, 0); /* First number */
:stk.pushAt(9, 1); /* Second number */

:print(:stk.getAt(4), :stk.getAt(9)); /* Print first 2 numbers 0, 1 */
:call calcFib; 

:subroutine calcFib {
	/* sum = first + second */
	:stk.pushAt(0, :stk.getAt(4) + :stk.getAt(9));

	/* if sum < stop number */
	:if(:stk.getAt(0) < 300) {
		:print(:stk.getAt(0)); /* print sum */

		/* first = second */
		:stk.pushAt(4, :stk.getAt(9));

		/* second = sum */
		:stk.pushAt(9, :stk.getAt(0));

		:call calcFib;
	}
};

/* Result is: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233 */