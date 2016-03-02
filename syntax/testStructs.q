:print("--------------TestStructs.q--------------");

:struct Pair {
	:alias first : offset(0);
	:alias second : offset(9);
};

:struct Calculator {
	:alias res : offset(0);
	:alias valFirst : offset(4);
	:alias valSecond : offset(8);

	:subroutine add { &res = valFirst + valSecond; };
};

:struct String {
	:alias len : offset(0); /*offset 0*/
	:alias cStr : offset(4); /*offset 4*/
	:alias pPair : Pair : offset(8); /*offset 8*/ /*Transforms to String/pPair/first & String/pPair/second*/

}; /*No typedef needed underlying layer will automaticle create a typedef String*/


:alias string : #33 = String;
&string.pPair.first = "firstPair";
&string.pPair.second = "secondPair";
&string.cStr = "hej";
&string.len = 3;

:print(string.pPair);
:print(string.pPair.second);
:print(string.pPair.first);
:print(string);
:print(string.cStr);
:print(string.len);

:if(string.cStr == "hej") { :print("Success"); } :else { :print("ERROR"); }
:if(string.len == 3) { :print("Success"); } :else { :print("ERROR"); }
:if(string == 3) { :print("Success"); } :else { :print("ERROR"); }
:if(string.pPair == "firstPair") { :print("Success"); } :else { :print("ERROR"); }
:if(string.pPair.second == "secondPair") { :print("Success"); } :else { :print("ERROR"); }
:if(string.pPair.first == "firstPair") { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");

:alias calc : #33 = Calculator;
&calc.valFirst = 2;
&calc.valSecond = 3;
:call calc.add;

:print(calc.res);
:print(calc.valFirst);
:print(calc.valSecond);
:print(calc);

:if(calc.res == 5) { :print("Success"); } :else { :print("ERROR"); }
:if(calc.valFirst == 2) { :print("Success"); } :else { :print("ERROR"); }
:if(calc.valSecond == 3) { :print("Success"); } :else { :print("ERROR"); }
:if(calc == 5) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");


:alias pair : #33 = Pair;
&pair.second = "test";
&pair.first = 67890;

:print(pair.first);
:print(pair.second);
:print(pair);

:if(pair.second == "test") { :print("Success"); } :else { :print("ERROR"); }
:if(pair.first == 67890) { :print("Success"); } :else { :print("ERROR"); }
:if(pair == 67890) { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");


:alias pairNext : #43 = Pair; 
&pairNext.second = 12345;
&pairNext.first = "testAgain";

:print(pairNext.first);
:print(pairNext.second);
:print(pairNext);


:if(pairNext.first == "testAgain") { :print("Success"); } :else { :print("ERROR"); }
:if(pairNext.second == 12345) { :print("Success"); } :else { :print("ERROR"); }
:if(pairNext == "testAgain") { :print("Success"); } :else { :print("ERROR"); }
:print("______________________________________________");