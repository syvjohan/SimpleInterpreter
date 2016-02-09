:print("--------------TestStack.q--------------");

:print("pushTop(Hello World): ");
:stk.pushTop("Hello World");
:print("");

:print("getTop(): ");
:print(:stk.getTop());

:print("","memory #0 - #10: ");
:print(#0);
:print("");

:print("pop(): ");
:stk.pop();

:print("Stack top element is now: ", :stk.getTop());
:print("");

:print("pushAt(#23 & 12345): ");
:stk.pushAt(23, 12345);
:print("");

:print("getAt(#23): ");
:print(:stk.getAt(23));
:print("");

:print("pop(): ");
:print("");