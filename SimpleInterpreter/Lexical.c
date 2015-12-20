#include "Lexical.h"

Lexical::Lexical() {}

Lexical::~Lexical() {
	if (code) {
		delete [] code;
		code = NULL;
	}

	if (subroutines) {
		delete [] subroutines;
		subroutines = NULL;
	}
}

char* Lexical::readFile(const char *path) {
	FILE *file;
	file = fopen(path, "r");

	if (file == NULL) {
		perror(path);
		printf("Error: %d \n", errno);
		return "Error";
	}

	//get size of file.
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	//allocate sizeof file +1.
	code = DBG_NEW char[fileSize + 1];

	//read file into string
	int c;
	int i = 0;
	while ((c = fgetc(file)) != EOF) {
		code[i] = (char)c;
		++i;
	}
	code[i] = '\0';

	fclose(file);

	return code;
}

void Lexical::setCode(char *cStr) {
	code = cStr;
}

void Lexical::allocateMem() {
	trimText(expression);
	
	//Allocate heap memory.
	int s = atoi(expression);
	parser.heap.initializeHeap(s);
}

void Lexical::createStack() {
	trimText(expression);

	//Create stack on heap
	int s = atoi(expression);
	parser.heap.createStack(s);
}

void Lexical::initlizeCurrentSubroutine() {
	currentSubroutine.endPos = -1;
	currentSubroutine.startPos = -1;
}

void Lexical::expandSubroutineSize(void) {
	int multiplier = 2;
	subroutinesMax *= multiplier;

	if (subroutines == NULL) {
		subroutinesMax = 1;
		subroutines = DBG_NEW subroutine_s[subroutinesMax];
	} else {
		subroutine_s *tmpArr = subroutines;

		subroutines = DBG_NEW subroutine_s[multiplier * sizeof(subroutine_s)];
		memcpy(subroutines, tmpArr, subroutinesLen * sizeof(subroutine_s));

		free(tmpArr);
	}
}

//Makes it possible to call a subroutine that has been declared after it is called.
void Lexical::getAllSubroutines(void) {
	initlizeCurrentSubroutine();
	int i = 0;
	do {
		char *findSub = strstr(code + i, ":subroutine");
		if (findSub) {
			char *findOpenBracket = strstr(findSub, "{");
			int len = strlen(findSub) - strlen(findOpenBracket) - 11;
			if (findOpenBracket) {
				char name[NAMESIZE];
				memcpy(name, findSub + 11, len);
				name[len] = '\0';
				trimText(name);

				char *ret = strstr(findSub, "};");
				if (ret) {
					subroutine_s subroutine;
					subroutine.endPos = ret - code;
					subroutine.startPos = findOpenBracket - code;
					memcpy(subroutine.name, name, len);
					subroutine.name[len] = '\0';

					if (subroutinesLen == subroutinesMax) {
						expandSubroutineSize();
					}

					subroutines[subroutinesLen] = subroutine;
                                     					++subroutinesLen;

					i = subroutine.endPos; //sets the program counter to a new value.
				} else {
					//Missing end of subroutine }; do CRASH!!
				}
			} else {
				//Missing subroutine open bracket, do CRASH!!!
			}
		} else {
			//No more subroutines to find end.
			return;
		}
	} while (i < fileSize);
}

void Lexical::evalAlias() {
	int len = strlen(expression);
	char *sep1 = strstr(expression, ":");
	char *sep2 = strstr(expression, "=");
	char *identifyType = strstr(expression, "\"");
	
	char address[ADDRESSSIZE];
	char val[VALUESIZE];
	Alias_s alias;

	int lenName = len - strlen(sep1);
	memcpy(alias.name, expression, lenName);
	alias.name[lenName] = '\0';

	int lenAddress;
	if (sep2) {
		lenAddress = strlen(sep1) - strlen(sep2) - 2;
	} else {
		lenAddress = len - strlen(sep1) -2;
	}
	
	memcpy(address, sep1 + 2, lenAddress);
	address[lenAddress] = '\0';

	if (sep2) {
		//check datatype and get val.
		if (identifyType) {
			memcpy(alias.type, "string", 6);
			alias.type[6] = '\0';

			alias.len = strlen(sep2) - 3;
			memcpy(val, sep2 + 2, alias.len);
			val[alias.len] = '\0';

		} else {
			memcpy(alias.type, "int", 3);
			alias.type[3] = '\0';

			len = strlen(sep2);
			alias.len = len;

			memcpy(val, sep2 + 1, len);
			val[len] = '\0';
	
			if (checkForDigits(alias.value) == -1) {
				//do CRASH!!!
			}
		}
	} else {
		//if No definition
		memcpy(alias.type, "", 1);
		memcpy(val, "", 1);
		alias.len = 0;
	}

	//alias name need to contain at least one letter.
	if (alias.len < 1) {
		//do CRASH!!!
	}
	
	//alias name can only contain letters.
	if (checkForAlpha(alias.name) == -1) {
		//do CRASH!!!
	}

	//alias name need to contain at least one letter.
	if (strlen(address) < 1) {
		//do CRASH!!!
	}
	
	//address can only contain digits.
	if (checkForDigits(address) == -1) {
			//do CRASH!!!
	}

	char *parserVal = val;
	if (!identifyType) {
		//regular expression.
		parserVal = parser.regularExpression(val);
		len = strlen(parserVal);
		memcpy(alias.value, parserVal, len);
		alias.value[len] = '\0';
		alias.len = len;
	} else {
		alias.len = strlen(parserVal);
		memcpy(alias.value, parserVal, alias.len);
		alias.value[alias.len] = '\0';
	}

	//Insert.
	int i = atoi(address);
	parser.heap.insertAliasAt(i, alias);
}

//void Lexical::evalName() {
//	trimString(expression);
//
//	char *equal = strstr(expression, "=");
//	if (equal) {
//		int lenRhs = strlen(equal) -3;
//		int lenLhs = strlen(expression) - lenRhs;
//
//		char lhs[VALUESIZE];
//		char rhs[VALUESIZE];
//
//		memcpy(lhs, expression, lenLhs);
//		lhs[lenLhs] = '\0';
//
//		memcpy(rhs, equal +2, lenRhs);
//		rhs[lenRhs] = '\0';
//
//		int isReferenceRhs = 0;
//		int isReferenceLhs = 0;
//		char *findReferenceRhs = strstr(rhs, "&");
//		char *findReferenceLhs = strstr(lhs, "&");
//		if (findReferenceRhs) {
//			isReferenceRhs = 1;
//		}
//
//		if (findReferenceLhs) {
//			isReferenceLhs = 1;
//		}
//
//		char *lhsRes = parser.parseManager(lhs, isReferenceLhs);
//		char *rhsRes = parser.parseManager(rhs, isReferenceRhs);
//
//		if (isReferenceLhs) {
//			Alias_s alias;
//			memcpy(alias.name, rhsRes, strlen(alias.name));
//			memcpy(alias.value, "", 1);
//			alias.len = 0;
//			memcpy(alias.type, "string", 6); //TODO check datatype.
//			parser.heap.insertAliasAt(atoi(lhsRes), alias);
//			
//		} else {
//			//Lhs (left hand sign) need to be a modifiable value(&), do CRASH!!!
//		}
//
//	} else {
//		//Equal operator missing, do CRASH!!!
//	}
//}

void Lexical::evalDo(int len) {
	//endIndex = index;
	//index -= len;
	//startIndex = index;
	//scope.incrementScope(index, -1);
}

void Lexical::evalWhile() {
	char *res = parser.regularExpression(expression);
	if (strCmp(res, "true")) {
		if (loop[loopLen -1] == startIndex - 1 && loop[loopLen] == (startIndex - instructionLen - 1)) {
			return;
		}

		++loopLen;
		loop[loopLen] = startIndex - 1;
		++loopLen;
		loop[loopLen] = startIndex - instructionLen - 1;
	} else if (strCmp(res, "false")) {
		loopLen -= 2;
	}
}

//	char *bodyEnd = strpbrk(expression, ";");
//	if (scope.getCurrentScopeEnd() != index) {
//		scope.incrementScope(-1, index);
//	}
//
//	//eval if end or goto start.
//	char *sepEqual = strstr(expression, "==");
//	char *sepNotEqual = strstr(expression, "!=");
//	int lenEx = strlen(expression);
//
//	char lhs[VALUESIZE];
//	char rhs[VALUESIZE];
//	if (sepNotEqual) {
//		int len = lenEx - strlen(sepNotEqual);
//		memcpy(lhs, expression, len);
//		lhs[len] = '\0';
//
//		memcpy(rhs, expression + len +2, strlen(sepNotEqual));
//		rhs[len] = '\0';
//
//		//reference = 1, value = 0.
//		int isReference = 0;
//		char *bitwiseAnd = strstr(lhs, "&");
//		char *hashtag = strstr(rhs, "#");
//
//		if (bitwiseAnd && hashtag) {
//			isReference = 1;
//		} else if (bitwiseAnd) {
//			//Wrong syntax cannot compare address with value, do CRASH!
//		}
//
//		//parse strings.
//		char *tmpL = parser.parseManager(lhs, isReference);
//		memset(&lhs, 0, VALUESIZE);
//		memcpy(lhs, tmpL, strlen(tmpL));
//
//		char *tmpR = parser.parseManager(rhs, isReference);
//		//memset(&rhs, 0, 50);
//		memcpy(rhs, tmpR, strlen(tmpR));
//
//		printf("i = %s\n", lhs);
//		//Values are always *char.
//		if (strCmp(lhs, rhs) == 0) {
//			startIndex = scope.getCurrentScopeStart();
//			index = startIndex;
//			printf("Values are not equal\n");
//			return;
//		} else {
//			scope.decrementScope();
//			printf("Values are equal\n");
//		}
//
//	} else if (sepEqual) {
//		int len = lenEx - strlen(sepEqual);
//		memcpy(lhs, expression, len);
//		lhs[len] = '\0';
//
//		memcpy(rhs, expression + len +2, strlen(sepEqual));
//		rhs[len] = '\0';
//
//		//reference = 1, value = 0.
//		int isReference = 0;
//		char *bitwiseAnd = strstr(lhs, "&");
//		char *hashtag = strstr(lhs, "#");
//		if (bitwiseAnd && hashtag) {
//			isReference = 1;
//		} else if (bitwiseAnd) {
//			//Wrong syntax cannot compare address with value, do CRASH!
//		}
//
//		//parse strings.
//		parser.parseManager(lhs, isReference);
//		parser.parseManager(rhs, isReference);
//
//		//Values are always *char.
//		if (strCmp(lhs, rhs) == 1) {
//			startIndex = scope.getCurrentScopeStart();
//			index = startIndex;
//			return;
//		} else {
//			scope.decrementScope();
//		}
//
//	} else {
//		// Missing compare operator do CRASH.
//	}



void Lexical::evalCall() {
	int found = 0;
	int i;
	for (i = 0; i != subroutinesLen; ++i) {
		if (strCmp(subroutines[i].name, expression)) {
			oldIndex = index;
			index = subroutines[i].startPos;
			currentSubroutine = subroutines[i];
			break;
		}
	}
	
	if (!found) {
		//Subroutine name does not exist, do CRASH!!!
	}
}

void Lexical::evalStk() {
	trimCloseParanthes(expression);
	char *pop = strstr(expression, ".pop(");
	char *pushAt = strstr(expression, ".pushAt(");
	char *pushTop = strstr(expression, ".pushTop(");
	char *getAt = strstr(expression, ".getAt(");
	char *getTop = strstr(expression, ".getTop(");

	char tmp[INSTRUCTIONSIZE];
	int len;
	if (pop) {
		parser.stackPop();

	} else if (pushAt) {
		len = strlen(pushAt) - 8;
		memcpy(tmp, pushAt + 8, len);
		tmp[len] = '\0';
		
		parser.stackPushAt(tmp);

	} else if (pushTop) {
		len = strlen(pushTop) - 9;
		memcpy(tmp, pushTop + 9, len);
		tmp[len] = '\0';

		parser.stackPushTop(tmp);

	} else if (getAt) {
		len = strlen(getAt) - 7;
		memcpy(tmp, getAt + 7, len);
		tmp[len] = '\0';

		parser.stackGetAt(tmp);

	} else if (getTop) {
		parser.stackGetTop();
	}
}

void Lexical::evalSubroutine(char *expression) {

}

void Lexical::evalAssert() {
	char *out = parser.regularExpression(expression);
}

void Lexical::evalPrint() {
	//char *newLine = strstr(expression, "\"/n\"");
	//if (newLine) {
	//	int len;

	//	char rhs[INSTRUCTIONSIZE];
	//	len = strlen(newLine) -4;
	//	memcpy(rhs, newLine +4, len);
	//	rhs[len] = '\0';

	//	char lhs[INSTRUCTIONSIZE];
	//	len = strlen(expression) - 4 - strlen(rhs);
	//	memcpy(lhs, expression, len);
	//	lhs[len] = '\0';
	//}

	char *out = parser.regularExpression(expression);
	char s[VALUESIZE];
	int len = strlen(out);
	memcpy(s, out, len);
	s[len] = '\0';

	if (len == 0) {
		printf("\nNULL");
	} else {
		printf("\n%s", s);
	}
}

void Lexical::evalIf() {
	trimBothParanthesis(expression);

	char *res = parser.regularExpression(expression);

	if (strCmp(res, "true")) {
		cmpResult = 1;
	} else if (strCmp(res, "false")) {
		cmpResult = 0;
	} else {
		//Something went completly wrong, DO CRASH!!!
	}
}

void Lexical::evalReg() {
//	parser.parseReg(keyword, expression);
}

void Lexical::splitInstruction(char *instruction) {
	trimWhitespaces(instruction);
	char tmp[INSTRUCTIONSIZE];
	char tmp2[INSTRUCTIONSIZE];
	int len = 0;

	char *WHILE = strstr(instruction, ":while");

	//if (!WHILE) {
	//	//is alias
	//	//char *isalias = NULL;
	//	//char *name;
	//	//int size = parser.heap.getHeapSize();
	//	//int i;
	//	//for (i = 0; i != size; ++i) {
	//		Alias_s alias = parser.heap.getAlias(instruction);
	//		if (alias.name != NULL) {
	//			//if it is a text string instead of a variable name.
	//			char *str = strstr(instruction, "\"");
	//			if (str) {
	//				return;
	//			}
	//			memcpy(&expression,instruction, strlen(instruction));
	//			//evalName();
	//			//break;
	//		}
	//	//}
	//}

	char *sysMemAllocHeap = strstr(instruction, ":sysMemAllocHeap");
	char *sysCreateStack = strstr(instruction, ":sysCreateStack");
	char *alias = strstr(instruction, ":alias");
	char *DO = strstr(instruction, ":do");
	char *reg = strstr(instruction, ":reg");
	char *IF = strstr(instruction, ":if");
	char *ELSE = strstr(instruction, ":else");
	char *call = strstr(instruction, ":call");
	char *subroutine = strstr(instruction, ":subroutine");
	char *stk = strstr(instruction, ":stk.");
	char *print = strstr(instruction, ":print(");

	if (sysMemAllocHeap) {
		keyword = ":sysMemAllocHeap";
		len = strlen(sysMemAllocHeap) - 16;
		memcpy(tmp, sysMemAllocHeap + 16, len);
		tmp[len] = '\0';
		expression = tmp;
		allocateMem();
	} 

	if (sysCreateStack) {
		keyword = ":sysCreateStack";
		len = strlen(sysCreateStack) - 15;
		memcpy(tmp, sysCreateStack + 15, len);
		tmp[len] = '\0';
		expression = tmp;
		createStack();
	}

	if (alias) {
		keyword = ":alias";
		len = strlen(alias) -6;
		memcpy(tmp, alias +6, len);
		tmp[len] = '\0';
		expression = tmp;
		evalAlias();
	} 

	if (DO) {
		int totLen = 0;
		char *bracket = strstr(DO, "{");
		if (bracket) {
			keyword = ":do";
			totLen = strlen(bracket) -1;
		} else {
			// Open bracket missing do CRASH!!
		}

		//Filter do from rest of expression, recursion.
		char *rest = strstr(DO +3, ":");
		if (rest) {
			len = strlen(rest);
			memcpy(tmp, rest, len);
			tmp[len] = '\0';
			splitInstruction(rest);
		}

		evalDo(totLen);
	}

	if (reg) {
		memcpy(tmp2, reg +1, 4);
		tmp2[4] = '\0';
		keyword = tmp2;

		len = strlen(reg) - 5;
		memcpy(tmp, reg + 5, len);
		tmp[len] = '\0';
		expression = tmp;
		evalReg();
	}

	if (WHILE) {
		keyword = ":while";
		len = strlen(WHILE) - 6;
		memcpy(tmp, WHILE + 7, len);
		tmp[len] = '\0';
		expression = tmp;
		evalWhile();
		return;
	}
	 
	if (IF) {
		keyword = ":if";
		len = strlen(IF) - 2;
		memcpy(tmp, IF + 3, len);
		tmp[len] = '\0';
		expression = tmp;
		evalIf();

		if (cmpResult) {
			ignore = 0;
		} else {
			//ignore :else.
			ignore = 1;
		}
	}

	if (ELSE && cmpResult) {
		ignore = 1;
	}

	if (stk) {
		keyword = ":stk";
		len = strlen(stk) - 3;
		memcpy(tmp, stk +4, len);
		tmp[len] = '\0';
		expression = tmp;
		evalStk();
	}

	if (call) {
		keyword = ":call";
		len = strlen(call) - 5;
		memcpy(tmp, call + 5, len);
		tmp[len] = '\0';
		expression = tmp;
		evalCall();
	}

	if (subroutine) {
		ignore = 1;
	}

	if (print) {
		keyword = ":print";
		len = strlen(print) - 8;
		memcpy(tmp, print + 7, len);
		tmp[len] = '\0';
		expression = tmp;
		evalPrint();
	}
}

void Lexical::getInstructions() {
	int comment = 0;
	char instruction[INSTRUCTIONSIZE];

	while (code[index] != '\0') {	
		//Comments
		if (code[index] == '/' && code[index +1] == '*') {
				comment = 1;
		} else if (code[index] == '*' && code[index +1] == '/') {
			comment = 0;
			startIndex = index + 1;
		}


		if (code[index] == '}') { 
			if (loopLen > 0) {
				startIndex = loop[loopLen];
				index = loop[loopLen - 1];
			}
			ignore = 0; 
		}

		if (index == currentSubroutine.endPos) {
			currentSubroutine.endPos = -1;
			currentSubroutine.startPos = -1;
			index = oldIndex;
		}

		if (code[index] == ';' || code[index] == '{') {
			if (comment == 0 && ignore == 0) {
				endIndex = index;//end 99.
				instructionLen = endIndex - startIndex; //start 69
				instructionLen = abs(instructionLen);
				memcpy(instruction, code + startIndex, instructionLen);
				instruction[instructionLen] = '\0';
				startIndex = endIndex + 1;
				splitInstruction(instruction);
				//printf("%s", instruction);
			}
			startIndex = index;
		}
		++index;
	}
}
