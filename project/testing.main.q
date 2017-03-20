:sysMemAllocGlobal 32;
:sysCreateStack 16;

:alias val : #16 = 2;
:alias val2 : #20 = 3;

:alias sum : #25;
&sum = val + val2;

:print(sum);

:stk.pushTop(1);
:print(:stk.getTop());
