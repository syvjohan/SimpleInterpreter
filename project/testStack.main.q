:print("--------------TestStack.q--------------");

:sysMemAllocHeap 64;
:sysCreateStack 64;

:stk.pushTop(1);
:stk.pushTop(2);
:stk.pushTop("hello");
:if( :stk.getTop() == "hello") { :print("Success"); } :else { :print("ERROR"); }

:stk.pop();
:if( :stk.getTop() == "hell") { :print("Success"); } :else { :print("ERROR"); }

:stk.popTop();
:if( :stk.getTop() == 2) { :print("Success"); } :else { :print("ERROR"); }
:print("NULL: ", :stk.getAt(2));

:stk.popTop();
:stk.popTop();
:stk.popTop();
:print("NULL: ", :stk.getTop())


:stk.pushTop(1);
:stk.pushAt(4, "hej");
:stk.pushTop(2);
:if( :stk.getAt(2) == "hej") { :print("Success"); } :else { :print("ERROR"); }

:stk.popTop();
:stk.popTop();
:stk.popTop();
:stk.popTop();


:stk.pushTop(11);
:stk.pushTop(12);
:stk.pushTop("hello again");

:if( :stk.getTop() == "hello again") { :print("Success"); } :else { :print("ERROR"); }
:if( :stk.getAt(2) == 11) { :print("Success"); } :else { :print("ERROR"); }

:stk.pop();
:if( :stk.getTop() == "hello agai") { :print("Success"); } :else { :print("ERROR"); }

:stk.popTop();
:if( :stk.getTop() == 12) { :print("Success"); } :else { :print("ERROR"); }
:print("NULL: ", :stk.getAt(2));

:stk.popTop();
:stk.popTop();
:stk.popTop();
:print("NULL: ", :stk.getTop());



