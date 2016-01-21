#include "Parser.h"

Parser::Parser() {}

Parser::~Parser() {}

Operator_s Parser::findOperator(const char *cStr, const int startPos) {
	const char *opCmpEqual = strstr(cStr + startPos, "==");
	const char *opCmpNotEqual = strstr(cStr + startPos, "!=");
	const char *opEqual = strstr(cStr + startPos, "=");
	const char *opCmpBigger = strstr(cStr + startPos, ">");
	const char *opCmpSmaller = strstr(cStr + startPos, "<");
	const char *opPlus = strstr(cStr + startPos, "+");
	const char *opMinus = strstr(cStr + startPos, "-");
	const char *opMul = strstr(cStr + startPos, "*");
	const char *opDiv = strstr(cStr + startPos, "/");

	int len = strlen(cStr);
	int pl = INT_MAX;
	int mi = INT_MAX;
	int di = INT_MAX;
	int mu = INT_MAX;
	int eq = INT_MAX;
	int cmpEq = INT_MAX;
	int cmpNEq = INT_MAX;
	int cmpBi = INT_MAX;
	int cmpSm = INT_MAX;

	if (opCmpEqual) {
		cmpEq = len - strlen(opCmpEqual);
	} else if (opCmpNotEqual) {
		cmpNEq = len - strlen(opCmpNotEqual);
	} else if (opEqual) {
		eq = len - strlen(opEqual);
	} else if (opCmpBigger) {
		cmpBi = len - strlen(opCmpBigger);
	} else if (opCmpSmaller) {
		cmpSm = len - strlen(opCmpSmaller);
	} else if (opPlus) {
		pl = len - strlen(opPlus);
	} else if (opMinus) {
		mi = len - strlen(opMinus);
	} else if (opMul) {
		mu = len - strlen(opMul);
	} else if (opDiv) {
		di = len - strlen(opDiv);
	} else {
		//Operator not supported, DO CRASH!!!
	}

	Operator_s newOp;
	memset(newOp.op, '\0', 2);
	newOp.pos = -1;
	newOp.len = 0;

	if (pl < mi && pl < di && pl < mu && pl < eq && pl < cmpEq && pl < cmpNEq && pl < cmpBi && pl < cmpSm) {
		newOp.pos = pl;
		memcpy(newOp.op, opPlus, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (mi < pl && mi < di && mi < mu && mi < eq && mi < cmpEq && mi < cmpNEq && mi < cmpBi && mi < cmpSm) {
		newOp.pos = mi;
		memcpy(newOp.op, opMinus, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (di < pl && di < mi && di < mu && di < eq && di < cmpEq && di < cmpNEq && di < cmpBi && di < cmpSm) {
		newOp.pos = di;
		memcpy(newOp.op, opDiv, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (mu < pl && mu < di && mu < mi && mu < eq && mu < cmpEq && mu < cmpNEq && mu < cmpBi && mu < cmpSm) {
		newOp.pos = mu;
		memcpy(newOp.op, opMul, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (eq < pl && eq < mu && eq < di && eq < mi && eq < cmpEq && eq < cmpNEq && eq < cmpBi && eq < cmpSm) {
		newOp.pos = eq;
		memcpy(newOp.op, opEqual, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (cmpEq < pl && cmpEq < mu && cmpEq < di && cmpEq < mi && cmpEq < cmpNEq && cmpEq < cmpBi && cmpEq < cmpSm) {
		newOp.pos = cmpEq;
		memcpy(newOp.op, opCmpEqual, 2);
		newOp.op[2] = '\0';
		newOp.len = 2;
	} else if (cmpNEq < pl && cmpNEq < mu && cmpNEq < di && cmpNEq < mi && cmpNEq < cmpEq && cmpNEq < cmpBi && cmpNEq < cmpSm) {
		newOp.pos = cmpNEq;
		memcpy(newOp.op, opCmpNotEqual, 2);
		newOp.op[2] = '\0';
		newOp.len = 2;
	} else if (cmpBi < pl && cmpBi < mu && cmpBi < di && cmpBi < mi && cmpBi < cmpEq && cmpBi < cmpNEq && cmpBi < cmpSm) {
		newOp.pos = cmpBi;
		memcpy(newOp.op, opCmpBigger, 2);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (cmpSm < pl && cmpSm < mu && cmpSm < di && cmpSm < mi && cmpSm < cmpEq && cmpSm < cmpNEq && cmpSm < cmpBi) {
		newOp.pos = cmpSm;
		memcpy(newOp.op, opCmpSmaller, 2);
		newOp.op[1] = '\0';
		newOp.len = 1;
	}
	return newOp;
}
bool Parser::isNegativeNumber(const char *cStr) {
	Operator_s op0 = findOperator(cStr, 0);
	if (op0.op[0] == '-' && op0.pos == 0) {
		Operator_s op1 = findOperator(cStr, op0.len);
		if (op1.len == 0) {
			return true;
		}
	} 
	return false;
}

void Parser::stackPop() {
	heap.pop();
}

int Parser::stackPushAt(char *cStr) {
	int len = strlen(cStr) - 6;
	memcpy(tmpStr, cStr + 6, len);
	tmpStr[len] = '\0';

	char *identifyType = strstr(cStr, "\"");
	char *comma = strstr(cStr, ",");

	int success = 0;
	if (comma) {
		Alias_s alias = { NULL, NULL, NULL, 0 };
		int len = strlen(tmpStr) - strlen(comma);
		memcpy(tmpLhs, tmpStr, len);
		tmpLhs[len] = '\0';

		char *resLhs = regularExpression(tmpLhs);
		int index;
		if (checkForDigits(resLhs) == 1 && identifyType == NULL) {
			index = atoi(resLhs);
		}

		len = strlen(comma) -1;
		memcpy(tmpRhs, comma +1, len);
		tmpRhs[len] = '\0';

		char *resRhs = regularExpression(tmpRhs);
		len = strlen(resRhs);

		if (identifyType) {
			memcpy(alias.type, "string", 6);
			alias.type[len] = '\0';
			alias.len = len;

		} else if (checkForDigits(resRhs) == 1 && identifyType == NULL) {
			memcpy(alias.type, "int", 3);
			alias.type[3] = '\0';
			alias.len = len;
		} else {
			//Syntax is wrong, DO CRASH!!
		}
		memcpy(alias.value, resRhs, alias.len);
		alias.value[alias.len] = '\0';

		success = heap.pushAt(index, alias);

	} else {
		//Only one parameter, DO CRASH!!
	}
	return success;
}

int Parser::stackPushTop(char *cStr) {
	int len = strlen(cStr) - 7;
	memcpy(tmpStr, cStr + 7, len);
	tmpStr[len] = '\0';

	char *identifyType = strstr(cStr, "\"");
	char *res = regularExpression(tmpStr);
	len = strlen(res);

	Alias_s alias;
	if (checkForAlpha(res) == 1 && identifyType) {
		memcpy(alias.type, "string", 6);
		alias.type[6] = '\0';

		alias.len = len;
	} else if (checkForDigits(res) == 1 && identifyType == NULL) {
		memcpy(alias.type, "int", 3);
		alias.type[3] = '\0';

		alias.len = len;
	} else {
		//Syntax is wrong, DO CRASH!!
	}
	memcpy(alias.name, "\0", 1);
	memcpy(alias.value, res, alias.len);
	alias.value[alias.len] = '\0';

	return heap.pushTop(alias);
}

Alias_s Parser::stackGetAt(char *cStr) {
	int len = strlen(cStr) - 5;
	memcpy(tmpStr, cStr + 5, len);
	tmpStr[len] = '\0';

	char *identifyType = strstr(tmpStr, "\"");
	
	Alias_s alias = { NULL, NULL, NULL, 0 };
	if (checkForDigits(tmpStr) == 1 && identifyType == NULL) {
		int val = atoi(tmpStr);
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
	trimBothParanthesis(expression);

	expression = "this is a string without meaning";
	char *sdaefrgjm = strstrReverse(expression, "string");

	



	//If there is no operators return expression(end condition).
	Operator_s op0 = findOperator(expression, 0);
	if (op0.pos == -1) {
		Alias_s a;
		//Is it a stack call?
		a = parseKeywords(expression);

		if (strlen(a.value) == 0) {
			memcpy(a.value, expression, a.len);
		}
		return a.value;
	}

	int len = strlen(expression);
	Operator_s op1 = findOperator(expression, op0.pos + op0.len);
	if (op1.pos != -1) {

		//Negative number.
		if (op0.pos != -1 && op1.op[0] == '-' && op1.pos == op0.pos +1) {
			int len = strlen(expression);
		} else {
			//If there is a second operator set len = operator.
			len = op1.pos;
		}
	}

	memcpy(tmpLhs, expression, len);
	tmpLhs[len] = '\0';

	int rhsLen = strlen(expression) - len;
	memcpy(tmpRhs, expression + len, rhsLen);
	tmpRhs[rhsLen] = '\0';
	
	char *res = calculateResult(tmpLhs);
	
	len = strlen(res);
	memcpy(tmpStr, res, len);
	tmpStr[len] = '\0';

	strcat(tmpStr, tmpRhs);
	tmpStr[len + strlen(tmpRhs)] = '\0';

	//Negative number.
	if (isNegativeNumber(tmpStr)) {
		return tmpStr;
	}

	//recursion.
	regularExpression(tmpStr);
	
	return tmpStr;
}

char* Parser::calculateResult(char *exp) {
	tmpStr[0] = '\0'; //reseting...

	Operator_s op0 = findOperator(exp, 0);

	char lhs[INSTRUCTIONSIZE];
	char rhs[INSTRUCTIONSIZE];

	memcpy(lhs, exp, op0.pos);
	lhs[op0.pos] = '\0';
	
	int rhsLen = strlen(exp) - op0.pos;
	memcpy(rhs, exp + op0.pos + op0.len, rhsLen);
	rhs[rhsLen] = '\0';

	Alias_s aliasLhs = parseKeywords(lhs);
	Alias_s aliasRhs = parseKeywords(rhs);
	
	setDatatype(&aliasLhs, aliasRhs); //Change type in parameter 1.

	if (strCmp(aliasLhs.value, "")) {
		//alias need to be set to an default value, DO CRASH!!!
	} else if (strCmp(aliasRhs.value, "")) {
		//alias need to be set to an default value, DO CRASH!!!
	}

	//do calculation
	//Both are digits
		if (strCmp(aliasLhs.type, "int") && strCmp(aliasRhs.type, "int")) {
			if (strCmp(op0.op, "+")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) + atoi(aliasRhs.value));
			} else if (strCmp(op0.op, "-")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) - atoi(aliasRhs.value));
			} else if (strCmp(op0.op, "*")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) * atoi(aliasRhs.value));
			} else if (strCmp(op0.op, "/")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) / atoi(aliasRhs.value));
				int v = atoi(tmpStr);
				if (v < 0) {
					tmpStr[0] = '0';
					tmpStr[1] = '\0';
				}
			} else if (strCmp(op0.op, "==")) {
				if (strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (strCmp(op0.op, "!=")) {
				if (strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
			} else if (strCmp(op0.op, "<")) {
				int l = atoi(aliasLhs.value);
				int r = atoi(aliasRhs.value);
				if (l < r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (strCmp(op0.op, ">")) {
				int l = atoi(aliasLhs.value);
				int r = atoi(aliasRhs.value);
				if (l < r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (strCmp(op0.op, "=")) {
				memcpy(&aliasLhs.value, &aliasRhs.value, aliasRhs.len);
				aliasLhs.value[aliasRhs.len] = '\0';
				aliasLhs.len = aliasRhs.len;
				int a = heap.getAddress(aliasLhs.name);
				heap.insertAliasAt(a, aliasLhs);

				int len = strlen(aliasLhs.value);
				memcpy(tmpStr, aliasLhs.value, len);
				tmpStr[len] = '\0';

			} else {
				//Wrong syntax do CRASH!!!
			}
		}
		//Both are text strings.
		else if (strCmp(aliasLhs.type, "string") && strCmp(aliasRhs.type, "string")) {
			if (strCmp(op0.op, "+")) {
				return strcat(aliasLhs.value, aliasRhs.value);
			} else if (strCmp(op0.op, "-")) {
				//Wrong syntax do CRASH!!!
			} else if (strCmp(op0.op, "*")) {
				//Wrong syntax do CRASH!!!
			} else if (strCmp(op0.op, "/")) {
				//Wrong syntax do CRASH!!!
			} else if (strCmp(op0.op, "==")) {
				if (strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (strCmp(op0.op, "!=")) {
				if (strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
			} else if (strCmp(op0.op, "<")) {
				if (aliasLhs.len < aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (strCmp(op0.op, ">")) {
				if (aliasLhs.len > aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (strCmp(op0.op, "=")) {
				memcpy(&aliasLhs.value, &aliasRhs.value, strlen(aliasRhs.value));
				aliasLhs.len == aliasRhs.len;
				int a = heap.getAddress(aliasLhs.name);
				heap.insertAliasAt(a, aliasLhs);

				int len = strlen(aliasLhs.value);
				memcpy(tmpStr, aliasLhs.value, len);
				tmpStr[len] = '\0';
			} else {
				//Wrong syntax do CRASH!
			}
		} else {
			//Cannot compare digits and strings.
		}

		return tmpStr;
}

Alias_s Parser::parseKeywords(char *exp) {
	Alias_s alias = { NULL, NULL, NULL, 0 };

	Alias_s a = heap.getAlias(exp);
	char *stack = strstr(exp, ":stk");
	char *address = strstr(exp, "#");
	char *and = strstr(exp, "&");
	char *dot = strstr(exp, ".");

	if (a.len > 0) {
		memcpy(&alias, &a, sizeof(a));

	} else if (stack) {
		char *pop = strstr(stack, "pop");
		char *pushAt = strstr(stack, "pushAt");
		char *pushTop = strstr(stack, "pushTop");
		char *getAt = strstr(stack, "getAt");
		char *getTop = strstr(stack, "getTop");

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
			alias = heap.getTop();
		} 
		else {
			//unsupported stack command.
		}

	} else if (address && !isAdress) {
		trimBothParanthesis(address);
		int len = strlen(address) - 1;
		memcpy(tmpStr, address +1, len);
		tmpStr[len] = '\0';
		int a = atoi(tmpStr);
		alias = heap.getAlias(a);

	} else if (address && isAdress) {
		trimBothParanthesis(address);
		int len = strlen(address) - 1;
		memcpy(alias.type, "address", 7);
		memcpy(alias.name, tmpStr, len);

		int digits = heap.getAddress(tmpStr);
		sprintf(alias.value, "%d", digits);

		alias.len = strlen(alias.value);

	} else if (and && !dot && !isAdress) {
		trimBothParanthesis(and);
		int len = strlen(and) - 1;
		memcpy(tmpStr, and + 1, len);
		tmpStr[len] = '\0';
		alias = heap.getAlias(tmpStr);

	} else if (and && !dot && isAdress) {
		trimBothParanthesis(and);
		int len = strlen(and) - 1;
		memcpy(tmpStr, and + 1, len);
		tmpStr[len] = '\0';
		alias.len = len;
		memcpy(alias.type, "address", 6);
		memcpy(alias.name, tmpStr, alias.len);

		int digits = heap.getAddress(tmpStr);
		sprintf(alias.value, "%d", digits);
	
	} else if (and && dot && !isAdress) {
		trimBothParanthesis(and);
		//struct
		int lenDot = strlen(dot) -1;
		int lenStruct = strlen(and) - 2 - lenDot;
		memcpy(tmpStr, and +1, lenStruct);
		tmpStr[lenStruct] = '\0';
		Index_s indexStruct = heap.getStructIndex(tmpStr);

		//alias
		int lenStructType = strlen(indexStruct.type);
		memcpy(tmpStr, indexStruct.type, lenStructType);
		memcpy(tmpStr + lenStructType, "/", 1);
		memcpy(tmpStr + lenStructType +1, dot + 1, lenDot);
		int lenType = lenDot + lenStructType + 1;
		tmpStr[lenType] = '\0';
		Index_s indexAlias = heap.getStructIndex(tmpStr);

		int a = indexStruct.startPos + indexAlias.startPos;
		Index_s newIndex;
		newIndex.len = indexAlias.len;
		newIndex.startPos = a;
		int lenAliasName = strlen(indexAlias.name);
		memcpy(newIndex.name, indexAlias.name, lenAliasName);
		newIndex.name[lenAliasName] = '\0';

		lenType = strlen(indexAlias.type);
		memcpy(newIndex.type, indexAlias.type, lenType);
		newIndex.type[lenType] = '\0';

		heap.updateStructIndex(newIndex); //Update startposition.

		alias = heap.getAlias(tmpStr);

	} else if (strstr(exp, "\"") == NULL) {
		trimBothParanthesis(exp);
		//Digits
		alias.len = strlen(exp);
		memcpy(alias.value, exp, alias.len);
		alias.value[alias.len] = '\0';

		memcpy(alias.type, "int", 3);
		alias.type[3] = '\0';

	} else if (strstr(exp, "\"")) {
		trimBothParanthesis(exp);
		//Letters
		trimText(exp);
		alias.len = strlen(exp);
		memcpy(alias.value, exp, alias.len);
		alias.value[alias.len] = '\0';

		memcpy(alias.type, "string", 6);
		alias.type[6] = '\0';

	} else {
		//Syntax for expression is wrong DO CRASH!!
	}

	return alias;
}

void Parser::setDatatype(Alias_s *aliasLhs, Alias_s aliasRhs) {
	if (!strCmp(aliasRhs.type, "")) {
		if (strCmp(aliasRhs.type, "int")) {
			memcpy(aliasLhs->type, "int", 3);
			aliasLhs->type[3] = '\0';

		} else if (strCmp(aliasRhs.type, "string")) {
			memcpy(aliasLhs->type, "string", 6);
			aliasLhs->type[6] = '\0';
		}
	}
}