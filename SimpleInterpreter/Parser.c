#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct operator_s {
	int pos;
	char op;
}operator_t;

operator_t findOperator(const char *cStr, const int startPos) {
	char *opPlus = strstr(cStr + startPos, "+");
	char *opMinus = strstr(cStr + startPos, "-");
	char *opMul = strstr(cStr + startPos, "*");
	char *opDiv = strstr(cStr + startPos, "/");
	char *opEqual = strstr(cStr + startPos, "=");

	int len = strlen(cStr);
	int pl = INT_MAX;
	int mi = INT_MAX;
	int di = INT_MAX;
	int mu = INT_MAX;
	int eq = INT_MAX;

	if (opPlus != NULL) {
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

	operator_t newOp;
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
		if (cStr[i] != ' ' && cStr[i] != '(') {
			cStr[j++] = cStr[i];
		} 
		i++;
	}
	cStr[j] = '\0';
}

void trimHeap(char *cStr) {
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

char* parseReg(char *keyword, char *expression) {
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
	return "";
}

char* parseHeap(char *cStr, int isReference) {
	trimHeap(cStr);
	if (isReference) {
		return cStr;
	}
	return getValue(atoi(cStr));
}

char* parseManager(char *cStr, int isReference) {
	trim(cStr);
	operator_t firstOp = findOperator(cStr, 0);

	//Check if it is a hard coded memory address
	char *hashtag = strstr(cStr, "#");
	if (hashtag) {
		return parseHeap(cStr, isReference);
	}

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

			resultLhs = getIndexAsString(lhs);
			if (resultLhs == "") {
				//alias name does not exist on heap, do CRASH!!!
			}
			lhsIsAddress = 1;
		} else {
			int res = getIndexAsInt(lhs);
			if (res == NULL) {
				//alias name does not exist on heap, do CRASH!!!
			}
			resultLhs = getValue(res);
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

			resultRhs = getIndexAsString(rhs);
			rhsIsAddress = 1;
		} else {
			int res = getIndexAsInt(rhs);
			resultRhs = getValue(res);
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
			//ths value is undefined.
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