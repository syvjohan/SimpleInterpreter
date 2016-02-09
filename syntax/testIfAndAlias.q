:print("--------------TestIfAndAlias.q--------------");

:alias str : #32 = "hej";
:alias number : #35 = 10 + 5;

:print(number); /*skriver ut värdet 15*/
:print(str); /*skriver ut värdet "hej"*/

:print(&str); /*Skriver ut addresen 32*/
:print( &number ); /*Skriver ut addresen 35*/

:stk.pushTop(10);
:alias s : #39 = :stk.getTop() + 5;
:if(s == 15) { :print("Success"); } :else { :print("ERRROR"); }
:print("______________________________________________");


:if(#32 +1 == "e") { :print("Success"); } :else { :print("ERRROR"); }
:if(&str +1 == "e") { :print("Success"); } :else { :print("ERRROR"); }

/* < > Jämför längderna på textsträngarna eller storleken på heltal*/
:if("s" < "number") { :print("Success"); } :else { :print("ERRROR"); }

:if("100" > "s") { :print("Success"); } :else { :print("ERRROR"); }

:if("s" < "number") { :print("Success"); } :else { :print("ERROR"); }

:if(s < number) { :print("true"); } :else { :print("Success"); }

:if(100 > s) { :print("true"); } :else { :print("Success"); 

:if(s == number) { :print("Success"); } :else { :print("ERRROR"); } 

:if(s != number) { :print("true"); } :else { :print("Success"); }

:if("number" == "number") { :print("Success"); } :else { :print("ERRROR"); }

:if("s" != "number") { :print("Success"); } :else {	:print("ERRROR"); }

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