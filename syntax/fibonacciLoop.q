/* Fibinacci implemented with while loop and Stack */

:stk.pushTop( 0 ); /* Sum */
:stk.pushAt(4, 0); /* First number */
:stk.pushAt(9, 1); /* Second number */
:stk.pushAt(13, 0); /* counter */

:print(:stk.getAt(4), :stk.getAt(9)); /* Print first 2 numbers 0, 1 */

/* number of terms to print */
:while(:stk.getAt(13) != 36) {

	/* sum = first + second */
	:stk.pushAt(0, :stk.getAt(4) + :stk.getAt(9));

	/* first = second */
	:stk.pushAt(4, :stk.getAt(9));

	/* second = sum */
	:stk.pushAt(9, :stk.getAt(0));

	/* ++counter */
	:stk.pushAt(13, :stk.getAt(13) +1);

	:print(:stk.getAt(0)); /* print sum */

	}
};

/* Result is: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169,  */