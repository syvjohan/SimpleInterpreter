:print("--------------TestSubroutines.q--------------");

:subroutine sub { &sum = val2 - val1; };

:subroutine add { &sum = val1 + val2; };

:subroutine mul { &sum = val2 * val1; };

:subroutine div { &sum = val2 / val1; };

:alias val1 : #43 = 3;
:alias val2 : #47 = 8; 
:alias val2Negative : #51 = -8;
:alias sum : #55 = 0;


:call sub;
:print("sub: ");
:if(sum == 5) { :print("Success"); } :else { :print("ERROR"); }
:if(val2 == 8) { :print("Success"); } :else { :print("ERROR"); }
:if(val1 == 3) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");

:call add;
:print("add: ");
:if(sum == 11) { :print("Success"); } :else { :print("ERROR"); }
:if(val2 == 8) { :print("Success"); } :else { :print("ERROR"); }
:if(val1 == 3) { :print("Success"); }  :else { :print("ERROR"); }
:print("______________________________________________");

:call mul;
:print("mul: ");
:if(sum == 24) { :print("Success"); }  :else { :print("ERROR"); }
:if(val2 == 8) { :print("Success"); } :else { :print("ERROR"); } 
:if(val1 == 3) { :print("Success"); }  :else { :print("ERROR"); }
:print("______________________________________________");

:call div;
:print("div: ");
:if(sum == 2) { :print("Success"); } :else { :print("ERROR"); } 
:if(val2 == 8) { :print("Success"); } :else { :print("ERROR"); }
:if(val1 == 3) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");


:subroutine subReverseOrder { &sum = val1 - val2; };

:subroutine addReverseOrder { &sum = val1 + val2; };

:subroutine mulReverseOrder { &sum = val1 * val2; };

:subroutine divReverseOrder { &sum = val1 / val2; };

:call subReverseOrder;
:print("subReverseOrder");
:if(sum == -5) { :print("Success"); } :else { :prin("ERROR"); }
:print("______________________________________________");


:call addReverseOrder;
:print("addReverseOrder");
:if(sum == 11) { :print("Success"); } :else { :prin("ERROR"); }
:print("______________________________________________");


:call mulReverseOrder;
:print("mulReverseOrder");
:if(sum == 24) { :print("Success"); } :else { :prin("ERROR"); }
:print("______________________________________________");


:call divReverseOrder;
:print("divReverseOrder");
:if(sum == 0) { :print("Success"); } :else { :prin("ERROR"); }
:print("______________________________________________");


:subroutine subNegative { &sum = val1 - val2Negative; };

:subroutine addNegative { &sum = val1 + val2Negative; };

:subroutine mulNegative { &sum = val1 * val2Negative; };

:subroutine divNegative { &sum = val1 / val2Negative; };

:call subNegative;
:print("subNegative");
	/* 3 - -8 */
:if(#55 == 11) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");


:call addNegative;
:print("addNegative");
:if(#55 == -5) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");


:call mulNegative;
:print("mulNegative");
:if(#55 == -24) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");


:call divNegative;
:print("divNegative");
:if(#55 == 0) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");


:subroutine nestedSubroutineCall {
		:call sub;
		:stk.pushAt(4, sum);
		:call add;
		&sum = sum + :stk.getAt(4); /*5 + 11 = 16*/
};

:print("nestedSubroutineCall: ");
:call nestedSubroutineCall;

:if(sum == 16) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");