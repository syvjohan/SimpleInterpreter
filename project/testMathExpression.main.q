:print("--------------TestMathExpressions.q--------------");

:sysMemAllocHeap 64;
:sysCreateStack 32;

:alias var : #32;
:print(#32 + 1 = 10, "Correct result is 10");

:print(#32 = 1 + 10, "Correct result is 11"); 

:print(&var = 1000 + 200, "Correct result is 1200");

:print(50 + 40 + 10 - 200, "Correct result is -100");

:print(2 * 4 * 7, "Correct result is 56");

:print(2 * 8 / 2, "Correct result is 8");

:print(2 == 4, "Correct result is false");