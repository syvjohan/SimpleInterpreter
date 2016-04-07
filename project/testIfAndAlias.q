:print("--------------TestIfAndAlias.q--------------");

:alias str : #32 = "hej";
:alias number : #35 = 10 + 5;

:print(number); /*skriver ut värdet 15*/
:print(str); /*skriver ut värdet "hej"*/

:print(&str); /*Skriver ut addresen 32*/
:print( &number ); /*Skriver ut addresen 35*/

:stk.pushTop(10);
:alias n1 : #39 = :stk.getTop() + 5;
:if(n1 == 15) { :print("Success"); } :else { :print("ERRROR"); }
:print("______________________________________________");

:if(#32 +1 == "e") { :print("Success"); } :else { :print("ERROR"); }

/* < > Jämför längderna på textsträngarna eller storleken på heltal*/
:if("n1" < "number") { :print("Success"); } :else { :print("ERROR"); }

:if("100" > "n1") { :print("Success"); } :else { :print("ERROR"); }

:if(n1 < number) { :print("ERROR"); } :else { :print("Success"); }

:if(100 > n1) { :print("Success"); } :else { :print("ERROR"); 

:if(n1 == number) { :print("Success"); } :else { :print("ERRROR"); } 

:if(n1 != number) { :print("ERROR"); } :else { :print("Success"); }

:if("number" == "number") { :print("Success"); } :else { :print("ERRROR"); }

:if("n1" != "number") { :print("Success"); } :else {	:print("ERRROR"); }

:if("nej" == "hej") {
	:print("ERROR");
} :else {
	:if(66 == 88) {
		:print("ERROR");
	} :else {
		:if("success" == "success") {
			:print("success");
		} :else {
			:print("ERROR");
		}
	}
}
:print("______________________________________________");

:stk.popTop();