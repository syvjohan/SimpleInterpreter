/* Fibinacci implemented with while loop and alias */

:sysMemAllocHeap 64;
:sysCreateStack 32;

:alias sum : #32 = 0; /* Sum */
:alias firstNumber : #36 = 0; /* First number */
:alias secondNumber : #40 = 1; /* Second number */
:alias counter : #44 = 0; /* counter */

:print(firstNumber, secondNumber); /* Print first 2 numbers 0, 1 */

/* number of terms to print */
:while(counter != 36) {

	/* sum = first + second */
	&sum = firstNumber + secondNumber;

	/* first = second */
	&firstNumber = secondNumber;

	/* second = sum */
	&secondNumber = sum;

	/* ++counter */
	&counter = counter +1;

	:print(sum); /* print sum */
};

/* Result is: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169,  */