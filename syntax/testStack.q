:print("--------------TestStack.q--------------");

:print("pushTop(Hello World)");
:stk.pushTop("Hello World");
:print("");

:print("getTop(): ");
:print(:stk.getTop());

:print("","memory #0 - #10: ");
:print(#0);
:print("");

:print("pushTop(My name is creator)");
:stk.pushTop("My name is creator");
:print("");

:print("Stack top element is now: ", :stk.getTop());
:print("");

:print("popTop(): ");
:stk.popTop();
:print("");

:print("Stack top element is now: ", :stk.getTop());
:print("");

:print("pop(): ");
:stk.pop();
:print("");

:print("Stack top element is now: ", :stk.getTop());
:print("");

:print("pushTop(Hello again)");
:stk.pushTop("Hello again");
:print("");

:print("Stack top element is now: ", :stk.getTop());
:print("");

:print("pushAt(#4 & 3817409): ");
:stk.pushAt(4, 3817409);
:print("");

:print("getAt(#4): ");
:print(:stk.getAt(4));
:print("");

:print("pushTop(Star Wars)");
:stk.pushTop("Star Wars");
:print("");

:print("Stack top element is now: ", :stk.getTop());
:print("");

:print("pop(): ");
:stk.pop();
:print("");

:print("Stack top element is now: ", :stk.getTop());
:print("");

:print("pushAt(#23 & 12345): ");
:stk.pushAt(23, 12345);
:print("");

:print("getAt(#23): ");
:print(:stk.getAt(23));
:print("");

:print("pushAt(#21 & 6789): ");
:stk.pushAt(21, 6789);
:print("");

:print("getAt(#21): ");
:print(:stk.getAt(21));
:print("");