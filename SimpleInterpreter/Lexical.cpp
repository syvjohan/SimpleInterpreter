#include "Lexical.h"

Lexical::Lexical() {
	expandCallsSize();
}

Lexical::~Lexical() {
	if (code) {
		delete[] code;
		code = NULL;
	}

	if (subroutines) {
		delete[] subroutines;
		subroutines = NULL;
	}

	if (calls) {
		delete[] calls;
		calls = NULL;
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
	char *readCode = DBG_NEW char[fileSize + 1];

	//read file into string
	int c;
	int i = 0;
	while ((c = fgetc(file)) != EOF) {
		readCode[i] = (char)c;
		++i;
	}
	readCode[i] = '\0';

	fclose(file);

	return readCode;
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

void Lexical::expandCallsSize(void) {
	int multiplier = 2;
	callsMax *= multiplier;

	if (calls == NULL) {
		callsMax = 100;
		calls = DBG_NEW Call_s[callsMax * sizeof(Call_s)];
	} else {
		Call_s *tmpArr = calls;

		calls = DBG_NEW Call_s[multiplier * sizeof(Call_s)];
		memcpy(calls, tmpArr, callsLen * sizeof(Call_s));

		delete[] tmpArr;
	}
}

void Lexical::expandSubroutineSize(void) {
	int multiplier = 2;
	subroutinesMax *= multiplier;

	if (subroutines == NULL) {
		subroutinesMax = 100;
		subroutines = DBG_NEW Subroutine_s[subroutinesMax * sizeof(Subroutine_s)];
	} else {
		Subroutine_s *tmpArr = subroutines;

		subroutines = DBG_NEW Subroutine_s[multiplier * sizeof(Subroutine_s)];
		memcpy(subroutines, tmpArr, subroutinesLen * sizeof(Subroutine_s));

		delete[] tmpArr;
	}
}

//Makes it possible to call a subroutine that has been declared after it is called.
void Lexical::registerAllSubroutines(void) {
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
				trimWhitespaces(name);

				char *ret = strstr(findSub, "};");
				if (ret) {
					Subroutine_s subroutine;
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

void Lexical::updateSubroutinesIndexes() {
	subroutinesLen = 0;
	registerAllSubroutines();
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

//TODO Update code!!
void Lexical::evalDo(int len) {
	//endIndex = index;
	//index -= len;
	//startIndex = index;
	//scope.incrementScope(index, -1);
}

void Lexical::evalWhile() {
	char *res = parser.regularExpression(expression);
	if (strCmp(res, "true")) {
		if (loop[loopLen].start == startIndex - 1 && loop[loopLen].end == (startIndex - instructionLen - 1)) {
			return;
		}

		++loopLen;
		loop[loopLen].start = startIndex - 1;
		loop[loopLen].end = startIndex - instructionLen - 1;
		loop[loopLen].stop = 0;
	} else if (strCmp(res, "false")) {
		--loopLen;
		loop[loopLen].stop = 1;
	}
}

void Lexical::evalCall(int len) {
	int found = 0;
	int i;
	for (i = 0; i != subroutinesLen; ++i) {
		if (strCmp(subroutines[i].name, expression)) {		
			++callsLen;
			calls[callsLen].pos = index - len;
			int len = strlen(subroutines[i].name);
			memcpy(calls[callsLen].name, &subroutines[i].name, len);
			calls[callsLen].name[len] = '\0';

			index = subroutines[i].startPos;
			currentSubroutine = subroutines[i];
			found = 1;
			break;
		}
	}
	
	if (!found) {
		//Subroutine name does not exist, do CRASH!!!
	}
}

void Lexical::evalStk() {
	trimBothParanthesis(expression);
	char *pop = strstr(expression, "pop");
	char *pushAt = strstr(expression, "pushAt");
	char *pushTop = strstr(expression, "pushTop");
	char *getAt = strstr(expression, "getAt");
	char *getTop = strstr(expression, "getTop");

	if (pop) {
		parser.stackPop();

	} else if (pushAt) {
		parser.stackPushAt(pushAt);

	} else if (pushTop) {
		parser.stackPushTop(pushTop);

	} else if (getAt) {
		parser.stackGetAt(getAt);

	} else if (getTop) {
		parser.stackGetTop();
	}
}

void Lexical::evalPrint() {
	char *and = strstr(expression, "&");
	char *adress = strstr(expression, "#");
	if (and || adress) {
		parser.isAdress = true;
	}

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

	parser.isAdress = false;
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

void Lexical::evalInclude() {
	trimBothParanthesis(expression);
	trimText(expression);

	int lenCode = fileSize;

	char *extendedCode = readFile(expression);
	int lenExtended = strlen(extendedCode);

	char *codeBefore = DBG_NEW char[lenCode];
	int lenCodeBefore = startIndex - instructionLen;
	memcpy(codeBefore, code, lenCodeBefore );
	codeBefore[lenCodeBefore] = '\0';

	char *codeAfter = DBG_NEW char[lenCode];
	int lenCodeAfter = lenCode - startIndex;
	memcpy(codeAfter, code + startIndex, lenCodeAfter);
	codeAfter[lenCodeAfter] = '\0';

	delete[] code;
	code = NULL;
	code = DBG_NEW char[lenCode + lenExtended];

	memcpy(code, codeBefore, lenCodeBefore);
	memcpy(code + strlen(codeBefore), extendedCode, lenExtended);
	memcpy(code + strlen(codeBefore) + lenExtended, codeAfter, lenCodeAfter);
	lenCode = lenCodeBefore + lenExtended + lenCodeAfter;
	code[lenCode] = '\0';

	fileSize = (size_t)lenCode; //Setting the size of the new code base.

	//Free memory
	delete[] codeAfter;
	codeAfter = NULL;

	delete[] codeBefore;
	codeBefore = NULL;

	//Reseting index
	index -= instructionLen;
	startIndex -= instructionLen;
	endIndex = index;
	instructionLen = 0;

	//Update indexes for subroutines since index is changed in document.
	updateSubroutinesIndexes();
}

void Lexical::evalExpressionWithoutKeyword() {
	char tmpLhs[INSTRUCTIONSIZE];
	char tmpRhs[INSTRUCTIONSIZE];
	char tmpStr[INSTRUCTIONSIZE];
	int len = strlen(expression);
	
	char *eq = strstr(expression, "=");
	if (eq) {
		int lenEq = strlen(eq);
		int lenLhs = len - lenEq;
		memcpy(tmpLhs, expression, lenLhs);
		tmpLhs[len - lenEq] = '\0';

		int rhsLen = len - lenLhs -1;
		memcpy(tmpRhs, eq +1, rhsLen);
		tmpRhs[rhsLen] = '\0';

		char *rhs = parser.regularExpression(tmpRhs);

		lenLhs = strlen(tmpLhs);
		memcpy(tmpStr, tmpLhs, lenLhs);

		memcpy(tmpStr + lenLhs, "=", 1);

		rhsLen = strlen(rhs);
		memcpy(tmpStr + lenLhs +1, rhs, rhsLen);
		tmpStr[lenLhs + 1 + rhsLen] = '\0';

		parser.regularExpression(tmpStr);
	}
}

void Lexical::splitInstruction(char *instruction) {
	trimWhitespaces(instruction);
	char tmp[INSTRUCTIONSIZE];
	char tmp2[INSTRUCTIONSIZE];
	int isKeywordMissing = 1;
	int len = 0;

	char *WHILE = strstr(instruction, ":while");
	char *sysMemAllocHeap = strstr(instruction, ":sysMemAllocHeap");
	char *sysCreateStack = strstr(instruction, ":sysCreateStack");
	char *alias = strstr(instruction, ":alias");
	char *DO = strstr(instruction, ":do");
	char *IF = strstr(instruction, ":if");
	char *ELSE = strstr(instruction, ":else");
	char *call = strstr(instruction, ":call");
	char *subroutine = strstr(instruction, ":subroutine");
	char *stk = strstr(instruction, ":stk.");
	char *print = strstr(instruction, ":print(");
	char *include = strstr(instruction, ":include(");

	if (sysMemAllocHeap) {
		keyword = ":sysMemAllocHeap";
		len = strlen(sysMemAllocHeap) - 16;
		memcpy(tmp, sysMemAllocHeap + 16, len);
		tmp[len] = '\0';
		expression = tmp;
		allocateMem();
		isKeywordMissing = 0;
	} 

	if (sysCreateStack) {
		keyword = ":sysCreateStack";
		len = strlen(sysCreateStack) - 15;
		memcpy(tmp, sysCreateStack + 15, len);
		tmp[len] = '\0';
		expression = tmp;
		createStack();
		isKeywordMissing = 0;
	}

	if (include) {
		keyword = ":include";
		len = strlen(include) - 8;
		memcpy(tmp, include + 8, len);
		tmp[len] = '\0';
		expression = tmp;
		evalInclude();
		isKeywordMissing = 0;
	}

	if (alias) {
		keyword = ":alias";
		len = strlen(alias) -6;
		memcpy(tmp, alias +6, len);
		tmp[len] = '\0';
		expression = tmp;
		evalAlias();
		isKeywordMissing = 0;
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
		isKeywordMissing = 0;
	}

	if (WHILE) {
		keyword = ":while";
		len = strlen(WHILE) - 6;
		memcpy(tmp, WHILE + 7, len);
		tmp[len] = '\0';
		expression = tmp;
		evalWhile();
		isKeywordMissing = 0;
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
		isKeywordMissing = 0;
	}

	if (ELSE && cmpResult) {
		ignore = 1;
		isKeywordMissing = 0;
	}

	if (stk) {
		char *opEq = strstr(instruction, "=");
		if (opEq) {
			isKeywordMissing = 1;
		} else {
			keyword = ":stk.";
			len = strlen(stk) - 4;
			memcpy(tmp, stk + 5, len);
			tmp[len] = '\0';
			expression = tmp;
			evalStk();
			isKeywordMissing = 0;
		}
	}

	if (call) {
		keyword = ":call";
		len = strlen(call) - 5;
		memcpy(tmp, call + 5, len);
		tmp[len] = '\0';
		expression = tmp;

		//Move index forward to ignore call again.
		//len += 5;
		//index += len;
		//startIndex = index + 1;
		//endIndex = index;

		evalCall(len);
		isKeywordMissing = 0;
	}

	if (subroutine) {
		ignore = 1;
		isKeywordMissing = 0;
	}

	if (print) {
		keyword = ":print";
		len = strlen(print) - 8;
		memcpy(tmp, print + 7, len);
		tmp[len] = '\0';
		expression = tmp;
		evalPrint();
		isKeywordMissing = 0;
	}

	if (isKeywordMissing == 1) {
		keyword = "regularExpression";
		trimWhitespaces(instruction);
		trimNewline(instruction);
		trimTabb(instruction);
		trimBracket(instruction);
		len = strlen(instruction);
		memcpy(tmp, instruction, len);
		tmp[len] = '\0';
		expression = instruction;

		evalExpressionWithoutKeyword();
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
			ignore = 0;
			if (loop[loopLen].stop == 0) {
				startIndex = loop[loopLen].end;
				index = loop[loopLen].start;
			}
		}

		if (index == currentSubroutine.endPos) {
			for (int i = 0; i != subroutinesLen; ++i) { 
				currentSubroutine = subroutines[i];
				if (strCmp(currentSubroutine.name, calls[callsLen].name)) {
					index = calls[callsLen].pos;
					--callsLen;

					startIndex += index + 1;
					break;
				}

			}
		}

		if (code[index] == '}' && code[index + 1] == ';' && callsLen == 0) {
			index = calls[callsLen].pos;
			startIndex += index + 1;
		}

		if (code[index] == ';' || code[index] == '{') {
			if (comment == 0 && ignore == 0) {
				endIndex = index;
				//TODO Rewrite and Remove this hax (if (endIndex +1 > startIndex) )!!!
				if (endIndex +1 >= startIndex) {
					instructionLen = endIndex - startIndex;
					instructionLen = abs(instructionLen);
					memcpy(instruction, code + startIndex, instructionLen);
					instruction[instructionLen] = '\0';
					startIndex = endIndex + 1;
					splitInstruction(instruction);
					//printf("%s", instruction);
				}
			}
			startIndex = index;
		}
		++index;
	}
}
