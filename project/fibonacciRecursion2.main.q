/* Fibonacci implemented with recursion and alias */

:sysMemAllocHeap 64;

:alias sum : #33 = 0;
:alias firstNumber : #4 = 0; /* First number */
:alias secondNumber : #8 = 1; /* Second number */

:print(firstNumber, secondNumber); /* Print first 2 numbers 0, 1 */
:call calcFib; 

:subroutine calcFib {
	/* sum = first + second */
	&sum = firstNumber + secondNumber;

	/* if sum < stop number */
	:if(sum < 300) {
		:print(sum); /* print sum */

		/* first = second */
		&firstNumber = secondNumber;

		/* second = sum */
		&secondNumber = sum;

		:call calcFib;
	}
};

/* Result is: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233 */