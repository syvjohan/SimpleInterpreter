#include "Parser.h"

#include "Trim.h"

#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser::Parser() {}

Parser::~Parser() {}

Operator_s Parser::findOperator(const char *cStr, const int startPos) {
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

	char buffer[INSTRUCTIONSIZE];

	if (global.findSubStrRev(buffer, cStr + startPos, "=")) {
		if (buffer) {
			eq = strlen(buffer) -1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, "+")) {
		if (buffer) {
			pl = strlen(buffer) - 1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, "-")) {
		if (buffer) {
			mi = strlen(buffer) - 1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, "*")) {
		if (buffer) {
			mu = strlen(buffer) - 1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, "/")) {
		if (buffer) {
			di = strlen(buffer) - 1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, ">")) {
		if (buffer) {
			cmpBi = strlen(buffer) - 1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, "<")) {
		if (buffer) {
			cmpSm = strlen(buffer) - 1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, "==")) {
		if (buffer) {
			cmpEq = strlen(buffer) - 1;
		}
	}

	if (global.findSubStrRev(buffer, cStr + startPos, "!=")) {
		if (buffer) {
			cmpNEq = strlen(buffer) - 1;
		}
	}

	Operator_s newOp;
	memset(newOp.op, '\0', 3);
	newOp.pos = -1;
	newOp.len = 0;

	if (pl < mi && pl < di && pl < mu && pl < eq && pl < cmpEq && pl < cmpNEq && pl < cmpBi && pl < cmpSm) {
		newOp.pos = pl;
		memcpy(newOp.op, buffer + pl, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (mi < pl && mi < di && mi < mu && mi < eq && mi < cmpEq && mi < cmpNEq && mi < cmpBi && mi < cmpSm) {
		newOp.pos = mi;
		memcpy(newOp.op, buffer + mi, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (di < pl && di < mi && di < mu && di < eq && di < cmpEq && di < cmpNEq && di < cmpBi && di < cmpSm) {
		newOp.pos = di;
		memcpy(newOp.op, buffer + di, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (mu < pl && mu < di && mu < mi && mu < eq && mu < cmpEq && mu < cmpNEq && mu < cmpBi && mu < cmpSm) {
		newOp.pos = mu;
		memcpy(newOp.op, buffer + mu, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (eq < pl && eq < mu && eq < di && eq < mi && eq < cmpEq && eq < cmpNEq && eq < cmpBi && eq < cmpSm) {
		newOp.pos = eq;
		memcpy(newOp.op, buffer + eq, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (cmpEq < pl && cmpEq < mu && cmpEq < di && cmpEq < mi && cmpEq < cmpNEq && cmpEq < cmpBi && cmpEq < cmpSm) {
		newOp.pos = cmpEq;
		memcpy(newOp.op, buffer + cmpEq -1, 2);
		newOp.op[2] = '\0';
		newOp.len = 2;
	} else if (cmpNEq < pl && cmpNEq < mu && cmpNEq < di && cmpNEq < mi && cmpNEq < cmpEq && cmpNEq < cmpBi && cmpNEq < cmpSm) {
		newOp.pos = cmpNEq;
		memcpy(newOp.op, buffer + cmpNEq - 1, 2);
		newOp.op[2] = '\0';
		newOp.len = 2;
	} else if (cmpBi < pl && cmpBi < mu && cmpBi < di && cmpBi < mi && cmpBi < cmpEq && cmpBi < cmpNEq && cmpBi < cmpSm) {
		newOp.pos = cmpBi;
		memcpy(newOp.op, buffer + cmpBi, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (cmpSm < pl && cmpSm < mu && cmpSm < di && cmpSm < mi && cmpSm < cmpEq && cmpSm < cmpNEq && cmpSm < cmpBi) {
		newOp.pos = cmpSm;
		memcpy(newOp.op, buffer + cmpSm, 1);
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
		if (global.checkForDigits(resLhs) == 1 && identifyType == NULL) {
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

		} else if (global.checkForDigits(resRhs) == 1 && identifyType == NULL) {
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
	if (global.checkForAlpha(res) == 1 && identifyType) {
		memcpy(alias.type, "string", 6);
		alias.type[6] = '\0';

		alias.len = len;
	} else if (global.checkForDigits(res) == 1 && identifyType == NULL) {
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
	if (global.checkForDigits(tmpStr) == 1 && identifyType == NULL) {
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
	int len = strlen(expression);
	char str[INSTRUCTIONSIZE];
	char lhs[INSTRUCTIONSIZE];
	char rhs[INSTRUCTIONSIZE];

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

	Operator_s op1 = findOperator(expression, op0.pos + op0.len);
	if (op1.pos != -1) {

		//Negative number.
		if (op0.pos != -1 && op1.op[0] == '-' && op1.pos == op0.pos +1) {
			len = strlen(expression);
		} else {
			//If there is a second operator set len = operator.
			len = op1.pos;
		}
	}

	memcpy(lhs, expression, len);
	lhs[len] = '\0';

	int rhsLen = strlen(expression) - len;
	memcpy(rhs, expression + len, rhsLen);
	rhs[rhsLen] = '\0';
	
	if (len > 0) {
		char *res = calculateResult(lhs);

		len = strlen(res);
		memcpy(str, res, len);
		str[len] = '\0';

		strcat(str, rhs);
		str[len + strlen(rhs)] = '\0';

		//Negative number.
		if (isNegativeNumber(str)) {
			return str;
		}

		//recursion.
		len = strlen(str);
		memcpy(tmpStr, str, len);
		tmpStr[len] = '\0';

		regularExpression(tmpStr);

		return tmpStr;
	}
	return "";
}

char* Parser::calculateResult(char *exp) {
	tmpStr[0] = '\0'; //reseting...

	Operator_s op0 = findOperator(exp, 0);

	char lhs[INSTRUCTIONSIZE];
	char rhs[INSTRUCTIONSIZE];

	assert(op0.pos > -1, "RegularExpression has failed with the parsing!");

	if (op0.len == 1) {
		memcpy(lhs, exp, op0.pos);
		lhs[op0.pos] = '\0';
	} else if (op0.len == 2) {
		memcpy(lhs, exp, op0.pos -1);
		lhs[op0.pos -1] = '\0';
	}

	int rhsLen = strlen(exp) - op0.pos;
	memcpy(rhs, exp + op0.pos + op0.len, rhsLen);
	rhs[rhsLen] = '\0';

	Alias_s aliasLhs = parseKeywords(lhs);
	Alias_s aliasRhs = parseKeywords(rhs);
	
	setDatatype(&aliasLhs, aliasRhs); //Change type in parameter 1.
	setLength(&aliasLhs, aliasRhs); //Change type in parameter 1.

	if (global.strCmp(aliasLhs.value, "")) {
		//alias need to be set to an default value, DO CRASH!!!
	} else if (global.strCmp(aliasRhs.value, "")) {
		//alias need to be set to an default value, DO CRASH!!!
	}

	//do calculation
	//Both are digits
	if (global.strCmp(aliasLhs.type, "int") && global.strCmp(aliasRhs.type, "int")) {
		if (global.strCmp(op0.op, "+")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) + atoi(aliasRhs.value));
		} else if (global.strCmp(op0.op, "-")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) - atoi(aliasRhs.value));
		} else if (global.strCmp(op0.op, "*")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) * atoi(aliasRhs.value));
		} else if (global.strCmp(op0.op, "/")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) / atoi(aliasRhs.value));
				int v = atoi(tmpStr);
				if (v < 0) {
					tmpStr[0] = '0';
					tmpStr[1] = '\0';
				}
		} else if (global.strCmp(op0.op, "==")) {
			if (global.strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
		} else if (global.strCmp(op0.op, "!=")) {
			if (global.strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
		} else if (global.strCmp(op0.op, "<")) {
				int l = atoi(aliasLhs.value);
				int r = atoi(aliasRhs.value);
				if (l < r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
		} else if (global.strCmp(op0.op, ">")) {
				int l = atoi(aliasLhs.value);
				int r = atoi(aliasRhs.value);
				if (l < r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
		} else if (global.strCmp(op0.op, "=")) {
			memcpy(&aliasLhs.value, &aliasRhs.value, strlen(aliasRhs.value));
			aliasLhs.len = sizeof(int);
			int a = heap.getAddress(aliasLhs.name);
			heap.insertAliasAt(a, aliasLhs);

			//Update index.
			Index_s index;
			index.len = aliasLhs.len;
			int lenName = strlen(aliasLhs.name);
			memcpy(index.name, aliasLhs.name, lenName);
			index.name[lenName] = '\0';

			index.startPos = a;
			int lenType = strlen(aliasLhs.type);
			memcpy(index.type, aliasLhs.type, lenType);
			index.type[lenType] = '\0';

			memcpy(index.name, aliasLhs.name, lenName);
			index.name[lenName] = '\0';

			index.len = aliasLhs.len;

			//Is it heapIndex or structIndex.
			char *type = strstr(aliasLhs.name, ".");
			if (type) {
				heap.updateStructIndex(index);
			} else {
				heap.updateHeapIndex(index);
			}

			int len = strlen(aliasLhs.value);
			memcpy(tmpStr, aliasLhs.value, len);
			tmpStr[len] = '\0';

			} else {
				//Wrong syntax do CRASH!!!
			}
		}
		//Both are text strings.
	else if (global.strCmp(aliasLhs.type, "string") && global.strCmp(aliasRhs.type, "string")) {
		if (global.strCmp(op0.op, "+")) {
				return strcat(aliasLhs.value, aliasRhs.value);
		} else if (global.strCmp(op0.op, "-")) {
				//Wrong syntax do CRASH!!!
		} else if (global.strCmp(op0.op, "*")) {
				//Wrong syntax do CRASH!!!
		} else if (global.strCmp(op0.op, "/")) {
				//Wrong syntax do CRASH!!!
			} else if (global.strCmp(op0.op, "==")) {
				if (global.strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (global.strCmp(op0.op, "!=")) {
				if (global.strCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
			} else if (global.strCmp(op0.op, "<")) {
				if (aliasLhs.len < aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (global.strCmp(op0.op, ">")) {
				if (aliasLhs.len > aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (global.strCmp(op0.op, "=")) {
				memcpy(&aliasLhs.value, &aliasRhs.value, strlen(aliasRhs.value));
				aliasLhs.len = aliasRhs.len;
				int a = heap.getAddress(aliasLhs.name);
				heap.insertAliasAt(a, aliasLhs);
				
				//Update index.
				Index_s index;
				index.len = aliasLhs.len;
				int lenName = strlen(aliasLhs.name);
				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.startPos = a;
				int lenType = strlen(aliasLhs.type);
				memcpy(index.type, aliasLhs.type, lenType);
				index.type[lenType] = '\0';

				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.len = aliasLhs.len;

				//Is it heapIndex or structIndex.
				char *type = strstr(aliasLhs.name, ".");
				if (type) {
					heap.updateStructIndex(index);
				} else {
					heap.updateHeapIndex(index);
				}

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

	int len = strlen(exp);

	if (a.len > 0) {
		memcpy(&alias, &a, sizeof(a));

	} else if (a.name[0] != '\0') {
		Index_s index = heap.getStructIndex(a.name);
		alias = heap.getAlias(index.startPos);
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
		len = strlen(address) - 1;
		memcpy(tmpStr, address +1, len);
		tmpStr[len] = '\0';
		int a = atoi(tmpStr);
		alias = heap.getAlias(a);

	} else if (address && isAdress) {
		trimBothParanthesis(address);
		len = strlen(address) - 1;
		memcpy(alias.type, "address", 7);
		memcpy(alias.name, tmpStr, len);

		int digits = heap.getAddress(tmpStr);
		sprintf(alias.value, "%d", digits);

		alias.len = strlen(alias.value);

	} else if (!and && dot && !isAdress) {
		int lenRhs = strlen(dot) - 1;
		memcpy(tmpRhs, dot + 1, lenRhs);
		tmpRhs[lenRhs] = '\0';

		int lenLhs = len - lenRhs - 1;
		memcpy(tmpLhs, exp, lenLhs);
		tmpLhs[lenLhs] = '\0';
		Index_s indexType = heap.getStructIndex(tmpLhs);

		int lenType = strlen(indexType.type);
		memcpy(tmpStr, tmpLhs, lenLhs);
		memcpy(tmpStr + lenLhs, ".", 1);
		memcpy(tmpStr + 1 + lenLhs, indexType.type, lenType);
		memcpy(tmpStr + 1 + lenLhs + lenType, ".", 1);
		memcpy(tmpStr + lenLhs + lenType + 2, tmpRhs, lenRhs);
		tmpStr[lenLhs + lenType + lenRhs + 2] = '\0';

		alias = heap.getAlias(tmpStr);
	} else if (and && !dot && !isAdress) {
		trimBothParanthesis(and);
		len = strlen(and) - 1;
		memcpy(tmpStr, and + 1, len);
		tmpStr[len] = '\0';
		alias = heap.getAlias(tmpStr);

	} else if (and && !dot && isAdress) {
		trimBothParanthesis(and);
		len = strlen(and) - 1;
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
		memcpy(tmpLhs, and +1, lenStruct);
		tmpLhs[lenStruct] = '\0';
		Index_s indexStruct = heap.getStructIndex(tmpLhs); //string

		//alias
		int oldLen = 0;
		int newLen = 0;
		int lenStructType = strlen(indexStruct.type);
		memcpy(tmpStr, indexStruct.type, lenStructType);
		tmpStr[lenStructType] = '\0';
		for (int i = 0; dot[i] != '\0'; ++i) {
			if (dot[i] == '.') {
				int k = i +1;
				int it = 0;
				while (dot[k] != '.' && dot[k] != '\0') {
					tmpLhs[it] = dot[k];
					++it;
					++k;
				}
				tmpLhs[it] = '\0';
				newLen = strlen(tmpLhs) +1;
				memcpy(tmpStr + lenStructType + oldLen +1, tmpLhs, newLen);
				memcpy(tmpStr + lenStructType + oldLen, ".", 1);
				oldLen += newLen;
				tmpStr[oldLen + lenStructType] = '\0';
				i = k -1;
			}
		}

		Index_s indexAlias = heap.getStructIndex(tmpStr);

		int a = indexStruct.startPos + indexAlias.startPos;
		Index_s newIndex;
		newIndex.len = indexAlias.len;
		newIndex.startPos = a;

		int lenStructName = strlen(indexStruct.name);
		memcpy(newIndex.name, indexStruct.name, lenStructName);
		memcpy(newIndex.name + lenStructName, ".", 1);
		int lenAliasName = strlen(indexAlias.name);
		memcpy(newIndex.name + lenStructName +1, indexAlias.name, lenAliasName);
		newIndex.name[lenStructName + lenAliasName +1] = '\0';

		int lenType = strlen(indexAlias.type);
		memcpy(newIndex.type, indexAlias.type, lenType);
		newIndex.type[lenType] = '\0';

		bool isUpdated = heap.updateStructIndex(newIndex); //Update startposition and name.
		
		if (!isUpdated) {
			heap.insertStructIndex(newIndex);
		}

		alias = heap.getAlias(newIndex.name);

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
	if (!global.strCmp(aliasRhs.type, "")) {
		if (global.strCmp(aliasRhs.type, "int")) {
			memcpy(aliasLhs->type, "int", 3);
			aliasLhs->type[3] = '\0';

			updateAlias(aliasLhs);
		} else if (global.strCmp(aliasRhs.type, "string")) {
			memcpy(aliasLhs->type, "string", 6);
			aliasLhs->type[6] = '\0';

			updateAlias(aliasLhs);
		}
	}
}

void Parser::setLength(Alias_s *aliasLhs, Alias_s aliasRhs) {
	aliasLhs->len = aliasRhs.len;
	updateAlias(aliasLhs);
}

void Parser::updateAlias(Alias_s *alias) {
	int address = heap.getAddress(alias->name);
	heap.insertAliasAt(address, *alias);
}