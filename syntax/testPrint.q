:print("--------------TestPrint.q--------------");
:print("hello world!");
:print("hello  again " , "who are 	you?");
:print(""); /*newline*/
:print("g");
:print(" "); /*print empty string*/

:alias test : #33 = 12345;
:print(test);
:print("Correct answer is 12345");
:print("");

:print(2 == 2, "true is Correct");
:print(2 != 2, "false is Correct");

:alias name : #33 = "creater";
:print("My name is: " , creater, " and my place in memory is: " , &name );
:print("Correct answer is: My name is: creater and my place in memory is: 33 ");