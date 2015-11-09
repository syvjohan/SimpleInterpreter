#include <stdio.h>
#include <stdlib.h>

typedef struct operator_s {
	int pos;
	char op;
}operator_t;

operator_t findOperator(const char *cStr, const int startPos) {
	char *opPlus = strstr(cStr + startPos, "+");
	char *opMinus = strstr(cStr + startPos, "-");
	char *opMul = strstr(cStr + startPos, "*");
	char *opDiv = strstr(cStr + startPos, "/");

	int len = strlen(cStr);
	int pl = INT_MAX;
	int mi = INT_MAX;
	int di = INT_MAX;
	int mu = INT_MAX;

	if (opPlus != NULL) {
		pl = len - strlen(opPlus);
	} else if (opMinus != NULL) {
		mi = len - strlen(opMinus);
	} else if (opMul != NULL) {
		di = len - strlen(opDiv);
	} else if (opDiv != NULL) {
		mu = len - strlen(opMul);
	}

	operator_t newOp;
	newOp.op = ' ';
	newOp.pos = -1;

	if (pl < mi && pl < di && pl < mu) {
		newOp.pos = pl;
		newOp.op = opPlus[0];
	} else if (mi < pl && mi < di && mi < mu) {
		newOp.pos = mi;
		newOp.op = opMinus[0];
	} else if (di < pl && di < mi && di < mu) {
		newOp.pos = di;
		newOp.op = opDiv[0];
	} else if (mu < pl && mu < di && mu < mi) {
		newOp.pos = mu;
		newOp.op = opMul[0];
	} 
	return newOp;
}

int checkForAlpha(const char *cStr) {
	int i = 0;
	while (cStr[i] != '\0') {
		if (!((cStr[i] >= 'a' && cStr[i] <= 'z') || (cStr[i] >= 'A' && cStr[i] <= 'Z'))) {
			return -1;
		}
		++i;
	}
	return 1;
}

int checkForDigits(const char *cStr) {
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

void trim(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ' ' && cStr[i] != '&') {
			cStr[j++] = cStr[i];
		} 
		i++;
	}
	cStr[j] = '\0';
	return cStr;
}

char* parseReg(char *cStr) {
	trim(cStr);

	char *brokenBar = strstr(cStr, ":");
	if (brokenBar != NULL) {
		int len = strlen(brokenBar);
		char tmp[50];
		memcpy(tmp, brokenBar +1, len);
		cStr = tmp;
		cStr[len] = '\0';
	}

	char mem[2];
	memcpy(mem, cStr + 3, 1);
	mem[1] = '\0';

	char operation[4];
	memcpy(operation, cStr + 5, 3);
	operation[3] = '\0';

	char *findClosePar = strpbrk(cStr, ")");
	char arg[50];
	int argLen = strlen(findClosePar) -1;
	memcpy(arg, cStr + 9, argLen);
	arg[argLen] = '\0';

	//TODO! Parse arg.

	if (strCmp(operation, "set")) {
		set(mem, arg);
	} else if (strCmp(operation, "get")) {
		return get(mem);
	} else if (strCmp(operation, "add")) {
		add(mem, arg);
	} else if (strCmp(operation, "sub")) {
		sub(mem, arg);
	} else if (strCmp(operation, "div")) {
		division(mem, arg);
	} else if (strCmp(operation, "mul")) {
		mul(mem, arg);
	}
	
}

char* parseManager(char *cStr) {
	trim(cStr);
	operator_t firstOp = findOperator(cStr, 0);

	//only one value
	if (firstOp.pos == -1) {
		return cStr;
	}

	operator_t secondOp = findOperator(cStr, firstOp.pos + 1);
	if (secondOp.pos == -1) {
		secondOp.pos = strlen(cStr);
	}

	char lhs[50];
	char rhs[50];
	memcpy(lhs, cStr, firstOp);
	lhs[firstOp.pos] = '\0';

	memcpy(rhs, cStr + firstOp.pos +1, secondOp);
	rhs[secondOp.pos] = '\0';

	char *lhsIsStr = strstr(lhs, ":");
	char *rhsIsStr = strstr(rhs, ":");

	char *resultLhs = NULL;
	char *resultRhs = NULL;

	//It is an variable.
	if (lhsIsStr == NULL) {
		int res = searchForName(lhs);
		resultLhs = getValue(res);
	} else {
		char keyword[4];
		memcpy(keyword, lhsIsStr + 1, 3);
		keyword[3] = '\0';

		if (strCmp(keyword, "reg")) {
			resultLhs = parseReg(lhsIsStr);

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
		if (isDigitsLhs == -1 && isDigitsRhs == 1) {
			return *(resultLhs + atoi(resultRhs));
		} else if (isDigitsLhs == 1 && isDigitsRhs == -1) {
			//Wrong syntax do CRASH!
		}
		//Both are digits
		else if (isDigitsLhs == 1 && isDigitsRhs == 1) {
			if (firstOp.op == '+') {
				return atoi(resultLhs) + atoi(resultRhs);
			} else if (firstOp.op == '-') {
				return atoi(resultLhs) - atoi(resultRhs);
			} else if (firstOp.op == '*') {
				return atoi(resultLhs) * atoi(resultRhs);
			} else if (firstOp.op == '/') {
				return atoi(resultLhs) / atoi(resultRhs);
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
		int res = searchForName(rhs);
		resultRhs = getValue(res);
	} else {
		char keyword[4];
		memcpy(keyword, rhsIsStr +1, 3);
		keyword[3] = '\0';

		if (strCmp(keyword, "reg")) {
			resultRhs = parseReg(rhsIsStr);

		} else if (strCmp(keyword, "stk")) {

		} else {
			//Unknown keyword do CRASH!
		}

		if (resultRhs == NULL) {
			//rhs value is undefined.
		} else if (resultLhs == NULL) {
			//ths value is undefined.
		}

		int isDigitsLhs = checkForDigits(resultLhs);
		int isDigitsRhs = checkForDigits(resultRhs);

		//lhs is text string and rhs is digit.
		if (isDigitsLhs == -1 && isDigitsRhs == 1) {
			return *(resultLhs + atoi(resultRhs));
		} 
		else if (isDigitsLhs == 1 && isDigitsRhs == -1) {
			//Wrong syntax do CRASH!
		} 
		//Both are digits
		else if (isDigitsLhs == 1 && isDigitsRhs == 1) {
			if (firstOp.op == '+') {
				return atoi(resultLhs) + atoi(resultRhs);
			} else if (firstOp.op == '-') {
				return atoi(resultLhs) - atoi(resultRhs);
			} else if (firstOp.op == '*') {
				return atoi(resultLhs) * atoi(resultRhs);
			} else if (firstOp.op == '/') {
				return atoi(resultLhs) / atoi(resultRhs);
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

	freeTmpValue();
}