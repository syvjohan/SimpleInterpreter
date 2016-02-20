#include "Parser.h"
#include "Trim.h"
#include "ErrorCodes.h"

#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser::Parser() {}

Parser::~Parser() {}

void Parser::stackPopTop() {
	heap.popTop();
}

void Parser::stackPop() {
	heap.pop();
}

void Parser::stackPushAt(char *cStr) {
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
		if (global.checkForDigits(resLhs) == 1) {
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

		heap.pushAt(index, alias);

	} else {
		//Only one parameter, DO CRASH!!
	}
}

void Parser::stackPushTop(char *cStr) {
	int len = strlen(cStr) - 7;
	memcpy(tmpStr, cStr + 7, len);
	tmpStr[len] = '\0';

	char *identifyType = strstr(cStr, "\"");
	char *res = regularExpression(tmpStr);
	len = strlen(res);

	Alias_s alias;
	if (/*global.checkForAlpha(res) == 1 && */identifyType) {
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

	heap.pushTop(alias);
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
	Operator_s op0 = global.findOperator(expression, 0);
	if (op0.pos == -1) {
		Alias_s a;
		//Is it a stack call?
		a = parseKeywords(expression);

		if (strlen(a.value) == 0) {
			memcpy(a.value, expression, a.len);
		}
		return a.value;
	}

	//Negative number.
	if (global.isNegativeNumber(expression)) {
		return expression;
	}

	Operator_s op1 = global.findOperator(expression, op0.pos + 1);
	if (op1.pos != -1) {

		//Negative number.
		if (op0.pos != -1 && op1.op[0] == '-' && op1.pos == op0.pos +1) {
			len = strlen(expression);
		} else {
			//If there is a second operator set len = operator.
			len = op0.pos +1;
		}
	}

	memcpy(lhs, expression, len);
	lhs[len] = '\0';

	int rhsLen = strlen(expression) - len;
	memcpy(rhs, expression + len, rhsLen);
	rhs[rhsLen] = '\0';
	
	if (len > 0) {
		char *res;
		if (op0.pos != -1 && op1.pos != -1) {
			res = calculateResult(rhs);

			strcat(lhs, res);
			len = strlen(lhs);
			memcpy(str, lhs, len);
			str[len] = '\0';

		} else {
			res = calculateResult(lhs);

			len = strlen(res);
			memcpy(str, res, len);
			str[len] = '\0';

			strcat(str, rhs);
			str[len + strlen(rhs)] = '\0';
		}

		//Negative number.
		if (global.isNegativeNumber(str)) {
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

	Operator_s op0 = global.findOperator(exp, 0);

	char lhs[INSTRUCTIONSIZE];
	char rhs[INSTRUCTIONSIZE];

	assert(op0.pos > -1, "RegularExpression has failed with the parsing!");
	
	int rhsLen = strlen(exp) - op0.pos;
	if (op0.len == 1) {
		memcpy(lhs, exp, op0.pos);
		lhs[op0.pos] = '\0';

		memcpy(rhs, exp + op0.pos + op0.len, rhsLen);
		rhs[rhsLen] = '\0';

	} else if (op0.len == 2) {
		memcpy(lhs, exp, op0.pos -1);
		lhs[op0.pos -1] = '\0';

		memcpy(rhs, exp + op0.pos + op0.len -1, rhsLen);
		rhs[rhsLen] = '\0';
	}

	Alias_s aliasLhs = parseKeywords(lhs);
	Alias_s aliasRhs = parseKeywords(rhs);
	
	if (op0.op[0] == '=' && op0.op[1] != '=') {
		setDatatype(&aliasLhs, aliasRhs); //Change type in parameter 1.
		setLength(&aliasLhs, aliasRhs); //Change type in parameter 1.
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
			if (global.checkForAlpha(aliasRhs.value) == -1 && global.checkForDigits(aliasRhs.value) == -1) {
				errorManager.ErrorCode(CODE_3510);
			}

			int len = strlen(aliasRhs.value);
			memcpy(&aliasLhs.value, &aliasRhs.value, len);
			aliasLhs.value[len] = '\0';
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
				heap.updateStructIndex(index, index.name);
				heap.updateStructHeaderPointer(index);
			} else {
				heap.updateHeapIndex(index);
			}

			len = strlen(aliasLhs.value);
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
					heap.updateStructIndex(index, index.name);
					heap.updateStructHeaderPointer(index);
				} else {
					heap.updateHeapIndex(index);
				}

				int len = strlen(aliasLhs.value);
				memcpy(tmpStr, aliasLhs.value, len);
				tmpStr[len] = '\0';
			}
	} else {
		//errorManager.ErrorCode(CODE_100);
	}
	return tmpStr;
}

Alias_s Parser::parseKeywords(char *exp) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	
	if (exp[0] == '\0') {
		return alias;
	}

	char *structName = heap.getFullNameStructMember(exp);
	if (structName) {
		int len = strlen(structName);
		memcpy(exp, structName, len);
		exp[len] = '\0';
	}

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
		char *popTop = strstr(stack, "popTop");
		char *pop = strstr(stack, "pop");
		char *pushAt = strstr(stack, "pushAt");
		char *pushTop = strstr(stack, "pushTop");
		char *getAt = strstr(stack, "getAt");
		char *getTop = strstr(stack, "getTop");

		char *identifyType = strstr(exp, "\"");

		if (popTop) {
			stackPopTop();
		}
		else if (pop) {
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

		int len1 = strlen(tmpLhs);
		int len2 = strlen(indexType.type);
		int lendot = strlen(dot);
		memcpy(tmpStr, indexType.type, len2);
		memcpy(tmpStr + len2, ".", 1);
		len2 += 1;
		memcpy(tmpStr + len2, tmpLhs, len1);
		memcpy(tmpStr + len2 + len1, dot, lendot);
		tmpStr[len1 + len2 + lendot] = '\0';

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
		char *d = strstr(dot + 1, ".");
		if (d) {
			memcpy(tmpLhs, dot +1, strlen(dot) - strlen(d) -1);
		} else {
			memcpy(tmpLhs, dot, strlen(dot));
		}

		Index_s indexStruct = heap.getStructIndex(tmpLhs);

		//alias
		memcpy(tmpRhs, and + 1, strlen(and));

		Index_s indexAlias = heap.getStructIndex(tmpRhs);

		int a = indexStruct.startPos + indexAlias.startPos;
		Index_s newIndex;
		newIndex.len = indexAlias.len;
		newIndex.startPos = a;

		int lenStructName = strlen(tmpRhs);
		memcpy(newIndex.name, tmpRhs, lenStructName);
		newIndex.name[lenStructName] = '\0';

		int lenType = strlen(indexAlias.type);
		memcpy(newIndex.type, indexAlias.type, lenType);
		newIndex.type[lenType] = '\0';

		bool isUpdated = heap.updateStructIndex(newIndex, newIndex.name);
		
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
			//aliasLhs->len = 0;

			//updateAlias(aliasLhs);
		} else if (global.strCmp(aliasRhs.type, "string")) {
			memcpy(aliasLhs->type, "string", 6);
			aliasLhs->type[6] = '\0';
			//aliasLhs->len = 0;

			//updateAlias(aliasLhs);
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

void Parser::parsePrint(char *expression, Parts_s *parts, int &len) {
	int lenParts = 0;
	char part[OUTPUTSIZE];
	int oldEndPoint = 0;
	int endPoint = 0;
	int lenPart = 0;
	bool isHit = false;
	int lenExp = strlen(expression);

	trimWhitespacesExceptInTextString(expression);

	do {
		for (int i = endPoint; i <= lenExp; ++i) {
			if (expression[i] == ',') {
				oldEndPoint = endPoint;
				endPoint = i;
				isHit = true;
				break;
			}
		}

		if (!isHit) {
			oldEndPoint = endPoint;
			endPoint = lenExp;
		}
		isHit = false;

		lenPart = endPoint - oldEndPoint;
		memcpy(part, expression + oldEndPoint, lenPart);
		part[lenPart] = '\0';

		if (global.isTextString(part)) {
			parts[lenParts].type = 1;
			trimText(part);
		} else {
			trimWhitespaces(part);
			parts[lenParts].type = 2;
		}
		memcpy(&parts[lenParts].part, part, lenPart);
		parts[lenParts].len = lenPart;
		++lenParts;
		endPoint += 1; //Jump over comma.
	} while (endPoint < lenExp);

	len = lenParts;
}