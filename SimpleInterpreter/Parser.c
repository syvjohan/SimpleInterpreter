#include "Parser.h"

Parser::Parser() {}

Parser::~Parser() {}

operator_s Parser::findOperator(const char *cStr, const int startPos) {
	const char *opPlus = strstr(cStr + startPos, "+");
	const char *opMinus = strstr(cStr + startPos, "-");
	const char *opMul = strstr(cStr + startPos, "*");
	const char *opDiv = strstr(cStr + startPos, "/");
	const char *opEqual = strstr(cStr + startPos, "=");

	int len = strlen(cStr);
	int pl = INT_MAX;
	int mi = INT_MAX;
	int di = INT_MAX;
	int mu = INT_MAX;
	int eq = INT_MAX;

	if (opPlus) {
		pl = len - strlen(opPlus);
	} else if (opMinus) {
		mi = len - strlen(opMinus);
	} else if (opMul) {
		di = len - strlen(opMul);
	} else if (opDiv) {
		mu = len - strlen(opDiv);
	} else if (opEqual) {
		eq = len - strlen(opEqual);
	}

	operator_s newOp;
	newOp.op = ' ';
	newOp.pos = -1;

	if (pl < mi && pl < di && pl < mu && pl < eq) {
		newOp.pos = pl;
		newOp.op = opPlus[0];
	} else if (mi < pl && mi < di && mi < mu && mi < eq) {
		newOp.pos = mi;
		newOp.op = opMinus[0];
	} else if (di < pl && di < mi && di < mu && di < eq) {
		newOp.pos = di;
		newOp.op = opDiv[0];
	} else if (mu < pl && mu < di && mu < mi && mu < eq) {
		newOp.pos = mu;
		newOp.op = opMul[0];
	} else if (eq < pl && eq < mu && eq < di && eq < mi) {
		newOp.pos = eq;
		newOp.op = opEqual[0];
	}
	return newOp;
}

int Parser::checkForAlpha(const char *cStr) {
	int i = 0;
	while (cStr[i] != '\0') {
		if (!((cStr[i] >= 'a' && cStr[i] <= 'z') || (cStr[i] >= 'A' && cStr[i] <= 'Z'))) {
			return -1;
		}
		++i;
	}
	return 1;
}

int Parser::checkForDigits(const char *cStr) {
	int i = 0;
	if (cStr[0] == '\0') { return -1; }
	while (cStr[i] != '\0') {
		if (!(cStr[i] >= '0' && cStr[i] <= '9')) {
			return -1;
		}
		++i;
	}
	return 1;
}

void Parser::trimString(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != '\"') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

void Parser::trim(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ' ' && cStr[i] != '(') {
			cStr[j++] = cStr[i];
		} 
		i++;
	}
	cStr[j] = '\0';
}

void Parser::trimHeap(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ' ' && cStr[i] != '&' && cStr[i] != '#' && cStr[i] != ')' && cStr[i] != '(' && cStr[i] != ';') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

void Parser::trimCloseParanthes(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ')') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

char* Parser::parseRegArg(char *keyword, char *arg) {
	operator_s op = findOperator(arg, 0);
	if (op.pos != -1) {

	} else {
		//only one parameter.
		char *reg = strstr(arg, ":reg");
		if (reg) {
			parseReg(keyword, reg);
		}
		//numbers or alias name or text string.
	}

	return NULL;
}

char* Parser::parseReg(char *keyword, char *expression) {
	trim(keyword);
	trim(expression);

	char mem[2];
	memcpy(mem, keyword +3, 2);
	mem[1] = '\0';

	char operation[4];
	memcpy(operation, expression +1, 3);
	operation[3] = '\0';

	char arg[50];
	int argLen = strlen(expression) - 5;
	memcpy(arg, expression + 4, argLen);
	arg[argLen] = '\0';

	//TODO! Parse arg.
	//arg = parseRegArg(keyword, arg));

	if (strCmp(operation, "set")) {
		reg.set(mem, arg);
	} else if (strCmp(operation, "get")) {
		return reg.get(mem);
	} else if (strCmp(operation, "add")) {
		reg.add(mem, arg);
	} else if (strCmp(operation, "sub")) {
		reg.sub(mem, arg);
	} else if (strCmp(operation, "div")) {
		reg.division(mem, arg);
	} else if (strCmp(operation, "mul")) {
		reg.mul(mem, arg);
	}
	return "";
}

char* Parser::parseHeap(char *cStr, int isReference) {
	trimHeap(cStr);
	if (isReference) {
		return cStr;
	}
	Alias_s alias = heap.getAlias(cStr);
	return alias.value;
}

char* Parser::parseManager(char *cStr, int isReference) {
	trim(cStr);
	operator_s firstOp = findOperator(cStr, 0);

	//Check if it is a hard coded memory address
	char *hashtag = strstr(cStr, "#");
	if (hashtag) {
		return parseHeap(cStr, isReference);
	}

	//only one value
	if (firstOp.pos == -1) {
		return cStr;
	}

	operator_s secondOp = findOperator(cStr, firstOp.pos + 1);
	if (secondOp.pos == -1) {
		secondOp.pos = strlen(cStr);
	}

	char lhs[50];
	char rhs[50];
	memcpy(lhs, cStr, firstOp.pos);
	lhs[firstOp.pos] = '\0';

	memcpy(rhs, cStr + firstOp.pos +1, secondOp.pos);
	rhs[secondOp.pos] = '\0';

	char *lhsIsStr = strstr(lhs, ":");
	char *rhsIsStr = strstr(rhs, ":");

	char *resultLhs = NULL;
	char *resultRhs = NULL;

	int lhsIsAddress = 0;
	int rhsIsAddress = 0;

	static char result[50];

	//It is an variable.
	if (lhsIsStr == NULL) {

		//Check if is a reference to a memory location.
		char *bitwiseAnd = strstr(lhs, "&");
		if (bitwiseAnd) {
			int len = strlen(bitwiseAnd);
			memcpy(lhs, bitwiseAnd + 1, len);
			lhs[len] = '\0';

			Alias_s aliasLhs = heap.getAlias(lhs);
			if (aliasLhs.name == NULL) {
				//alias name does not exist on heap, do CRASH!!!
			}
			resultLhs = aliasLhs.value;
			lhsIsAddress = 1;
		} else {
			Alias_s aliasLhs = heap.getAlias(lhs);
			if (aliasLhs.name == NULL) {
				//alias name does not exist on heap, do CRASH!!!
			}
			resultLhs = aliasLhs.value;
		}

	} else {
		char keyword[5];
		memcpy(keyword, lhsIsStr + 1, 3);
		keyword[3] = '\0';

		char *dot = strstr(lhsIsStr, ".");
		char ex[50];
		memcpy(ex, dot, strlen(dot));
		ex[strlen(dot)] = '\0';

		if (strCmp(keyword, "reg")) {
			char mem[2];
			memcpy(mem, lhsIsStr + 4, 1);
			mem[1] = '\0';
			strcat(keyword, mem);
			resultRhs = parseReg(keyword, ex);

		} else if (strCmp(keyword, "stk")) {

		} else {
			//Unknown keyword do CRASH!
		}

		if (resultRhs == NULL) {
			//rhs value is undefined.
		} else if (resultLhs == NULL) {
			//lhs value is undefined.
		}

		int isDigitsLhs = checkForDigits(resultLhs);
		int isDigitsRhs = checkForDigits(resultRhs);

		//lhs is text string and rhs is digit.
		if (isDigitsLhs == -1 && isDigitsRhs == 1 && !lhsIsAddress) {
			sprintf(result, "%s", *(resultLhs + atoi(resultRhs)));
		}
		else if (isDigitsLhs == 1 && isDigitsRhs == -1) {
			//Wrong syntax do CRASH!
		}
		//Both are digits
		else if (isDigitsLhs == 1 && isDigitsRhs == 1) {
			if (firstOp.op == '+') {
				sprintf(result, "%d", atoi(resultLhs) + atoi(resultRhs));
			} else if (firstOp.op == '-') {
				sprintf(result, "%d", atoi(resultLhs) - atoi(resultRhs));
			} else if (firstOp.op == '*') {
				sprintf(result, "%d", atoi(resultLhs) * atoi(resultRhs));
			} else if (firstOp.op == '/') {
				sprintf(result, "%d", atoi(resultLhs) / atoi(resultRhs));
			} else {
				//Wrong syntax do CRASH!
			}
		}
		//Both are text strings.
		else if (isDigitsLhs - 1 && isDigitsRhs == -1) {
			if (firstOp.op == '+') {
				return strcat(resultLhs, resultRhs);
			} else if (firstOp.op == '-') {
				//Wrong syntax do CRASH!
			} else if (firstOp.op == '*') {
				//Wrong syntax do CRASH!
			} else if (firstOp.op == '/') {
				//Wrong syntax do CRASH!
			}
		}
	}

	if (rhsIsStr == NULL) {
		char *bitwiseAnd = strstr(rhs, "&");
		if (bitwiseAnd) {
			int len = strlen(bitwiseAnd);
			memcpy(rhs, bitwiseAnd + 1, len);
			rhs[len] = '\0';
			
			Alias_s alias = heap.getAlias(rhs);
			resultRhs = alias.value;
			rhsIsAddress = 1;
		} else {
			Alias_s alias = heap.getAlias(rhs);
			resultRhs = alias.value;
		}
	} else {
		char keyword[5];
		memcpy(keyword, rhsIsStr +1, 3);
		keyword[3] = '\0';

		char *dot = strstr(rhsIsStr, ".");
		char ex[50];
		memcpy(ex, dot, strlen(dot));
		ex[strlen(dot)] = '\0';

		if (strCmp(keyword, "reg")) {
			char mem[2];
			memcpy(mem, rhsIsStr + 4, 1);
			mem[1] = '\0';
			strcat(keyword, mem);
			resultRhs = parseReg(keyword, ex);

		} else if (strCmp(keyword, "stk")) {

		} else {
			//Unknown keyword do CRASH!
		}

		if (resultRhs == NULL) {
			//rhs value is undefined.
		} else if (resultLhs == NULL) {
			//lhs value is undefined.
		}

		int isDigitsLhs = checkForDigits(resultLhs);
		int isDigitsRhs = checkForDigits(resultRhs);

		//lhs is text string and rhs is digit.
		if (isDigitsLhs == -1 && isDigitsRhs == 1 && !lhsIsAddress) {
			sprintf(result, "%s", *(resultLhs + atoi(resultRhs)));
		}
		//lhs is address and rhs is digit and lhs is a reference.
		else if (isDigitsLhs == 1 && isDigitsRhs == 1 && lhsIsAddress) {
			sprintf(result, "%d", atoi(resultLhs) + atoi(resultRhs));
		}
		else if (resultLhs == "" && resultRhs != "" && lhsIsAddress) {
			return resultRhs;
		}
		else if (isDigitsLhs == 1 && isDigitsRhs == -1) {
			//Wrong syntax do CRASH!
		} 
		//Both are digits
		else if (isDigitsLhs == 1 && isDigitsRhs == 1) {
			if (firstOp.op == '+') {
				sprintf(result, "%d", atoi(resultLhs) + atoi(resultRhs));
			} else if (firstOp.op == '-') {
				sprintf(result, "%d", atoi(resultLhs) - atoi(resultRhs));
			} else if (firstOp.op == '*') {
				sprintf(result, "%d", atoi(resultLhs) * atoi(resultRhs));
			} else if (firstOp.op == '/') {
				sprintf(result, "%d", atoi(resultLhs) / atoi(resultRhs));
			} else {
				//Wrong syntax do CRASH!
			}
		} 
		//Both are text strings.
		else if (isDigitsLhs - 1 && isDigitsRhs == -1) {
			if (firstOp.op == '+') {
				return strcat(resultLhs, resultRhs);
			} else if (firstOp.op == '-') {
				//Wrong syntax do CRASH!
			} else if (firstOp.op == '*') {
				//Wrong syntax do CRASH!
			} else if (firstOp.op == '/') {
				//Wrong syntax do CRASH!
			}
		}
	}
	return result;
}

void Parser::stackPop() {
	heap.pop();
}

int Parser::stackPushAt(char *cStr) {
	char *identifyType = strstr(cStr, "\"");
	char *comma = strstr(cStr, ",");

	int success = 0;
	if (comma) {
		Alias_s alias;
		char lhs[EXPRESSIONSIZE];
		memcpy(lhs, cStr, strlen(comma) - strlen(cStr));
		char *resLhs = regularExpression(lhs);
		int index;
		if (checkForDigits(resLhs) == 1 && identifyType == NULL) {
			index = atoi(resLhs);
		}

		char rhs[EXPRESSIONSIZE];
		memcpy(rhs, cStr, strlen(comma));
		char *resRhs = regularExpression(rhs);
		if (identifyType) {
			alias.type = "string";
			alias.len = strlen(resRhs);
		} else if (checkForDigits(resRhs) == 1 && identifyType == NULL) {
			alias.type = "int";
			alias.len = NULL;
		} else {
			//Syntax is wrong, DO CRASH!!
		}
		alias.name = NULL;
		alias.value = resRhs;
		success = heap.pushAt(index, alias);

	} else {
		//Only one parameter, DO CRASH!!
	}
	return success;
}

int Parser::stackPushTop(char *cStr) {
	char *identifyType = strstr(cStr, "\"");
	char *res = regularExpression(cStr);

	Alias_s alias;
	if (checkForAlpha(res) == 1 && identifyType) {
		alias.type = "string";
		alias.len = strlen(res);
	} else if (checkForDigits(res) == 1 && identifyType == NULL) {
		alias.type = "int";
		alias.len = NULL;
	} else {
		//Syntax is wrong, DO CRASH!!
	}
	alias.name = NULL;
	alias.value = res;

	return heap.pushTop(alias);
}

Alias_s Parser::stackGetAt(char *cStr) {
	char *identifyType = strstr(cStr, "\"");

	Alias_s alias;
	if (checkForDigits(cStr) == 1 && identifyType == NULL) {
		int val = atoi(cStr);
		alias = heap.getAt(val);
	} else {
		//Syntax wrong expression need to be a valid integer, DO CRASH!!
	}
	return alias;
}

Alias_s Parser::stackGetTop() {
	return heap.getTop();
}

char* Parser::regularExpression(char *expression) {
	//If there is no operators return expression(end condition).
	operator_s op0 = findOperator(expression, 0);
	if (op0.pos == -1) {
		Alias_s a;
		//Is it a stack call?
		a = findOutSecret(expression);

		if (a.value == NULL) {
			a.value = expression;
		}
		trimString(expression);
		return expression;
	}

	//If there is a second operator set len = operator.
	int len = strlen(expression);
	operator_s op1 = findOperator(expression, op0.pos +1);
	if (op1.pos != -1) {
		len = op1.pos;
	}

	char exp[EXPRESSIONSIZE];
	memcpy(exp, expression, len);
	exp[len] = '\0';

	char dest[EXPRESSIONSIZE];
	int destLen = strlen(expression) - len;
	memcpy(dest, expression + len, destLen);
	dest[destLen] = '\0';
	
	char *res = recursiveParser(exp);
	strcat(res, dest);

	len = strlen(res);
	memcpy(str, res, len);
	str[len] = '\0';
	//recursion.
	regularExpression(str);
	
	return str;
}

char* Parser::recursiveParser(char *exp) {
	operator_s op0 = findOperator(exp, 0);
	char lhs[EXPRESSIONSIZE];
	char rhs[EXPRESSIONSIZE];

	memcpy(lhs, exp, op0.pos);
	lhs[op0.pos] = '\0';
	
	int rhsLen = strlen(exp) - op0.pos;
	memcpy(rhs, exp + op0.pos + 1, rhsLen);
	rhs[rhsLen] = '\0';

	Alias_s aliasLhs = findOutSecret(lhs);
	Alias_s aliasRhs = findOutSecret(rhs);

	//do calculation
	//Both are digits
		if (aliasLhs.type == "int" && aliasRhs.type == "int") {
			char result[EXPRESSIONSIZE];
			if (op0.op == '+') {
				sprintf(result, "%d", atoi(aliasLhs.value) + atoi(aliasRhs.value));
			} else if (op0.op == '-') {
				sprintf(result, "%d", atoi(aliasLhs.value) - atoi(aliasRhs.value));
			} else if (op0.op == '*') {
				sprintf(result, "%d", atoi(aliasLhs.value) * atoi(aliasRhs.value));
			} else if (op0.op == '/') {
				sprintf(result, "%d", atoi(aliasLhs.value) / atoi(aliasRhs.value));
			} else {
				//Wrong syntax do CRASH!
			}
			return result;
		}
		//Both are text strings.
		else if (aliasLhs.type == "string" && aliasRhs.type == "string") {
			if (op0.op == '+') {
				return strcat(aliasLhs.value, aliasRhs.value);
			} else if (op0.op == '-') {
				//Wrong syntax do CRASH!
			} else if (op0.op == '*') {
				//Wrong syntax do CRASH!
			} else if (op0.op == '/') {
				//Wrong syntax do CRASH!
			}
		} else {
			//Cannot compare digits and strings.
		}

	return NULL;
}

Alias_s Parser::findOutSecret(char *exp) {
	Alias_s alias = { NULL, NULL, NULL, 0 };

	Alias_s a = heap.getAlias(exp);
	char *stack = strstr(exp, ":stk.");
	char *address = strstr(exp, "#");

	if (a.name) {
		alias = a;

	} else if (stack) {
		trimCloseParanthes(stack);
		char *pop = strstr(stack, "pop(");
		char *pushAt = strstr(stack, "pushAt(");
		char *pushTop = strstr(stack, "pushTop(");
		char *getAt = strstr(stack, "getAt(");
		char *getTop = strstr(stack, "getTop(");

		char *identifyType = strstr(exp, "\"");

		if (pop) {
			stackPop();
		} 
		else if (pushAt) {
			stackPushAt(stack);
		} 
		else if (pushTop) {
			stackPushTop(pushTop);
		} 
		else if (getAt) {
			alias = stackGetAt(getAt);
		} 
		else if (getTop) {
			return heap.getTop();
		} 
		else {
			//unsupported stack command.
		}

	} else if (address) {
		char a[EXPRESSIONSIZE];
		int len = strlen(address) - 1;
		memcpy(a, address, len);
		a[len] = '\0';
		alias.len = len;
		alias.type = "address";
		alias.value = a;
	} else if (strstr(exp, "\"") == NULL) {
		//Digits
		alias.value = exp;
		alias.type = "int";
	} else if (strstr(exp, "\"")) {
		//Letters
		trimString(exp);
		alias.value = exp;
		alias.type = "string";
		alias.len = strlen(exp);
	} else {
		//Syntax for expression is wrong DO CRASH!!
	}

	return alias;
}