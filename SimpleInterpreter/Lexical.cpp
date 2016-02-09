#include "Lexical.h"
#include "Trim.h"
#include "memoryLeak.h"

#include <iostream>
#include <errno.h>

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
	if (calls == NULL) {
		callsMax = 100;
		calls = DBG_NEW Call_s[callsMax * sizeof(Call_s)];
	} else {
		Call_s *tmpArr = calls;

		callsMax *= 2;
		calls = DBG_NEW Call_s[callsMax * sizeof(Call_s)];
		memcpy(calls, tmpArr, callsLen * sizeof(Call_s));

		delete[] tmpArr;
	}
}

void Lexical::expandStructsSize() {
	if (structs == NULL) {
		structsMax = 100;
		structs = DBG_NEW CallableUnit_s[structsMax * sizeof(CallableUnit_s)];
	} else {
		CallableUnit_s *tmpArr = subroutines;

		structsMax *= 2;
		structs = DBG_NEW CallableUnit_s[structsMax * sizeof(CallableUnit_s)];
		memcpy(structs, tmpArr, structsLen * sizeof(CallableUnit_s));

		delete[] tmpArr;
	}
}

void Lexical::expandSubroutineSize(void) {
	if (subroutines == NULL) {
		subroutinesMax = 100;
		subroutines = DBG_NEW CallableUnit_s[subroutinesMax * sizeof(CallableUnit_s)];
	} else {
		CallableUnit_s *tmpArr = subroutines;

		subroutinesMax *= 2;
		subroutines = DBG_NEW CallableUnit_s[subroutinesMax * sizeof(CallableUnit_s)];
		memcpy(subroutines, tmpArr, subroutinesLen * sizeof(CallableUnit_s));

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
			if (findOpenBracket) {
				int len = strlen(findSub) - strlen(findOpenBracket) - 11;
				char name[NAMESIZE];
				memcpy(name, findSub + 11, len);
				name[len] = '\0';
				trimText(name);
				trimWhitespaces(name);

				char *ret = strstr(findSub, "};");
				if (ret) {
					CallableUnit_s subroutine;
					subroutine.endPos = ret - code;
					subroutine.startPos = findOpenBracket - code;
					memcpy(subroutine.name, name, len);
					subroutine.name[len] = '\0';

					if (subroutinesLen == subroutinesMax) {
						expandSubroutineSize();
					}

					//Check if subroutine is inside struct.
					for (int i = 0; i != structsLen; ++i) {
						CallableUnit_s *s = &structs[i];
						if (subroutine.startPos > s->startPos && subroutine.endPos < s->endPos) {
							char tmpName[NAMESIZE];
							len = strlen(subroutine.name);
							memcpy(tmpName, subroutine.name, len);
							int lenStructName = strlen(s->name);
							memcpy(subroutine.name, s->name, lenStructName);
							memcpy(subroutine.name +lenStructName, ".", 1);
							memcpy(subroutine.name + lenStructName +1, tmpName, len);
							subroutine.name[len + lenStructName +1] = '\0';
							break;
						}
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

void Lexical::registerAllStructs() {
	int i = 0;
	do {
		char *findStruct = strstr(code + i, ":struct");
		if (findStruct) {
			char *findOpenBracket = strstr(findStruct, "{");
			if (findOpenBracket) {
				int lenOpenBracket = strlen(findOpenBracket);
				int lenName = strlen(findStruct) - lenOpenBracket - 7;
				char name[NAMESIZE];
				memcpy(name, findStruct + 7, lenName);
				name[lenName] = '\0';
				trimText(name);
				trimWhitespaces(name);

				//count open brackets for ignore.
				int indexOpen = 0;
				int indexClose = 0;
				int moveIndexTo;
				for (int i = 0; i != fileSize; ++i) {
					if (findStruct[i] == '{') {
						++indexOpen;
					} else if (findStruct[i] == '}') {
						++indexClose;
						if (indexClose == indexOpen) {
							moveIndexTo = i -1;
							break;
						}
					}
				}

				char *ret = strstr(findStruct + moveIndexTo, "};");
				if (ret) {
					CallableUnit_s newStruct;
					newStruct.endPos = ret - code;
					newStruct.startPos = findOpenBracket - code;
					memcpy(newStruct.name, name, lenName);
					newStruct.name[lenName] = '\0';

					if (structsLen == structsMax) {
						expandStructsSize();
					}

					structs[structsLen] = newStruct;
					++structsLen;

					i = newStruct.endPos; //sets the program counter to a new value.

					//Get code inside struct.
					int lenInsideStruct = lenOpenBracket - strlen(ret);
					char *codeInsideStruct = DBG_NEW char[lenInsideStruct +1];
					memcpy(codeInsideStruct, findOpenBracket, lenInsideStruct);
					codeInsideStruct[lenInsideStruct] = '\0';

					memcpy(currentStructName, name, lenName);
					isInitializingStructs = true;
					evalCodeInsideStruct(codeInsideStruct);

					//Free memory
					delete[] codeInsideStruct;
					codeInsideStruct = NULL;

					isInitializingStructs = false;
				} else {
					//Missing end of struct }; do CRASH!!
				}
			} else {
				//Missing struct open bracket, do CRASH!!!
			}
		} else {
			//No more struct to find end.
			return;
		}
	} while (i < fileSize);
}

void Lexical::updateStructsIndexes() {
	structsLen = 0;
	registerAllStructs();
}

void Lexical::updateSubroutinesIndexes() {
	subroutinesLen = 0;
	registerAllSubroutines();
}

void Lexical::typedefSubroutines(char *searchName, char *extendName) {
	char str[NAMESIZE];
	int len1 = 0;
	int len2 = 0;
	for (int i = 0; i != subroutinesLen; ++i) {
		CallableUnit_s *s = &subroutines[i];
		char *res = strstr(s->name, searchName);
		if (res) {
			char *findDot = strstr(s->name, ".");
			if (findDot) {
				len2 = strlen(extendName);
				memcpy(str, extendName, len2);
				memcpy(str + len2, ".", 1);
				len1 = strlen(findDot);
				memcpy(str + len2, findDot, len1);
				str[len2 + len1] = '\0';

				len1 += len2;
				memcpy(s->name, str, len1);
				s->name[len1] = '\0';
			} else {
				//Syntax struct is wrong Do CRASH!!!
			}
		}
	}
}

void Lexical::typedefSubroutinesMembers(char *searchName, char *extendName) {
	char buffer[2048];
	char instruction[INSTRUCTIONSIZE];
	char newSearchName[NAMESIZE];
	Operator_s op;
	int len1 = 0;
	int len2 = 0;
	int len = 0;
	for (int i = 0; i != subroutinesLen; ++i) {
		CallableUnit_s *s = &subroutines[i];
		char *res = strstr(s->name, searchName);
		if (res) {
			int position = 0;
			int lenInstructions = s->endPos - s->startPos;
			if (lenInstructions > 0) {
				memcpy(buffer, code + s->startPos, lenInstructions);
				buffer[lenInstructions] = '\0';
				trimHeap(buffer);
				trimNewline(buffer);
				trimTabb(buffer);
				trimBracket(buffer);
				trimSemicolon(buffer);
				lenInstructions = strlen(buffer);
				while (position <= lenInstructions) {

				op = global.findOperator(buffer, position);
				if (op.pos == -1) {
					len = lenInstructions - position;
				} else {
					op.pos += position;
					len = op.pos - position;
				}

				len2 = strlen(extendName);
				memcpy(instruction, extendName, len2);
				memcpy(instruction + len2, ".", 1);
				len2 += 1;

				memcpy(instruction + len2, buffer + position, len);
				memcpy(newSearchName, instruction, len + len2);
				newSearchName[len + len2] = '\0';

				Index_s foundIndex = parser.heap.findStructIndex(newSearchName);


				len1 = strlen(searchName);
				memcpy(instruction + len2, searchName, len1);
				len2 += len1;
				memcpy(instruction + len2, ".", 1);

				len2 += 1;
				memcpy(instruction + len2, buffer + position, len);
				len += len2;
				instruction[len] = '\0';

				if (op.pos == -1) {
					position = instructionLen;
				} else {
					position = op.pos + 1;
				}

				//Update struct index.					
				memcpy(foundIndex.name, instruction, len);
				foundIndex.name[len] = '\0';

				parser.heap.updateStructIndex(foundIndex, newSearchName);

				}
			}
		}
	}
}

void Lexical::resetLoopArray() {
	for (int i = 0; i != LOOPMAX; ++i) {
		Loop_s *l = &loop[i];
		l->start = -1;
		l->end = -1;
		l->stop = -1;
		l->type = -1;
	}
}

void Lexical::evalAlias() {
	int len = strlen(expression);
	char *sep1 = strstr(expression, ":");
	char *sep2 = strstr(expression, "=");
	char *offset = strstr(expression, "offset(");
	char *identifyType = strstr(expression, "\"");
	
	char address[ADDRESSSIZE];
	int lenAddress;

	char val[VALUESIZE];
	Alias_s alias;

	int lenName = len - strlen(sep1);
	memcpy(alias.name, expression, lenName);
	alias.name[lenName] = '\0';
	if (global.checkForAlpha(alias.name) == -1) {
		//Wrong name conversion, DO CRASH!!!
	}

	if (!sep1) {
		//Syntax error DO CRASH!!!
	}

	if (sep2) {
		lenAddress = strlen(sep1) - strlen(sep2) - 2;
	} else {
		lenAddress = len - strlen(sep1) -2;
	}
	
	memcpy(address, sep1 + 2, lenAddress);
	address[lenAddress] = '\0';

	if (offset && isInitializingStructs) {
		//special case, alias with offset in struct.
		lenAddress = strlen(offset) - 8;
		memcpy(address, offset + 7, lenAddress);
		address[lenAddress] = '\0';

		Index_s index = { NULL, NULL, 0, 0 };
		index.startPos = atoi(address);
		index.len = 4;
		memcpy(index.type, "offset", 6);
		index.type[6] = '\0';

		int lenStructName = strlen(currentStructName);
		memcpy(index.name, currentStructName, lenStructName);
		memcpy(index.name + lenStructName, ".", 1);
		memcpy(index.name + lenStructName + 1, alias.name, lenName);
		lenStructName += 1 + lenName;
		index.name[lenStructName] = '\0';

		char tmp[INSTRUCTIONSIZE];
		//check if it shall point to struct.
		char *nestedStruct = strstr(sep1 +1, ":");
		if (nestedStruct) {
			len = strlen(sep1) - strlen(nestedStruct) - 1;
			memcpy(tmp, sep1 +1, len);
			tmp[len] = '\0';
			int i = 0;
			Index_s indexes[STRUCTMEMBERS];
			int lenIndexes = 0;
			parser.heap.getStructIndex(tmp, indexes, lenIndexes);
			for (int i = 0; i != lenIndexes; ++i) {
				char *name = indexes[i].name;
				const char *slash = strstr(name, ".");
				int lenDot = strlen(slash) -1;

				memcpy(index.name + lenStructName, ".", 1);
				memcpy(index.name + lenStructName + 1, slash + 1, lenDot);
				index.name[lenStructName + 1 + lenDot] = '\0';

				index.startPos += indexes[i].startPos;

				parser.heap.insertStructIndex(index);

				//create a index for struct head pointer.
				if (indexes[i].startPos == 0) {
					global.findSubStrRev(tmp, index.name, ".");
					lenDot = strlen(tmp) -1;
					memcpy(index.name, tmp, lenDot);
					index.name[lenDot] = '\0';

					memset(index.type, '\0', strlen(index.type));

					parser.heap.insertStructIndex(index);
				}

			}
			return;
		}

 		parser.heap.insertStructIndex(index);
		return;

	} else if (sep2) {
		//With definition.
		//check datatype and get val.
		if (identifyType) {
			memcpy(alias.type, "string", 6);
			alias.type[6] = '\0';

			alias.len = strlen(sep2) - 3;
			memcpy(val, sep2 + 2, alias.len);
			val[alias.len] = '\0';

		} else {

			len = strlen(sep2 +1);
			alias.len = len;

			memcpy(val, sep2 + 1, len);
			val[len] = '\0';

			memcpy(alias.type, val, len);
			alias.type[len] = '\0';
	
if (global.checkForDigits(val) == -1) {
	if (global.checkForAlpha(val) == 1) {
		Index_s index = { NULL, NULL, 0, 0 };
		//Not checking if struct exist. Checking and mapping is carried out when user assign a value.
		index.len = 0;
		index.startPos = atoi(address);
		memcpy(index.type, val, len);
		memcpy(index.name, alias.name, lenName);

		parser.heap.insertStructIndex(index);

		//structs only! insert pointer name into path. Find all structs with typename.
		for (int i = 0; i != structsLen; ++i) {
			CallableUnit_s *s = &structs[i];
			if (global.strCmp(s->name, index.type)) {
				//typedef struct members
				parser.heap.typedefStructMembers(index.type, index.name);
				//typedef subroutines.
				typedefSubroutines(index.type, index.name);
				typedefSubroutinesMembers(index.name, index.type);
			}
		}

	} else {
		//Wrong format DO CRASH!!!
	}
} else {
	memcpy(alias.type, "int", 3);
	alias.type[3] = '\0';
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

	//alias name need to contain at least one letter.
	if (strlen(address) < 1) {
		//do CRASH!!!
	}

	//address can only contain digits.
	if (global.checkForDigits(address) == -1) {
		//do CRASH!!!
	}

	char *parserVal = val;
	if (!identifyType) {
		//regular expression.
		if (val[0] != '\0') {
			parserVal = parser.regularExpression(val);
			len = strlen(parserVal);
			memcpy(alias.value, parserVal, len);
			alias.value[len] = '\0';
			alias.len = len;

			//identify type.
			if (global.checkForDigits(parserVal) == 1) {
				memcpy(alias.type, "int", 3);
				alias.type[3] = '\0';
			} else if (global.checkForAlpha(parserVal) == 1) {
				memcpy(alias.type, "string", 6);
				alias.type[6] = '\0';
			} else if (global.isNegativeNumber(parserVal)) {
				//Negative number.
				memcpy(alias.type, "int", 3);
				alias.type[3] = '\0';
			}
		}
	} else {
		alias.len = strlen(parserVal);
		memcpy(alias.value, parserVal, alias.len);
		alias.value[alias.len] = '\0';
	}

	//Insert.
	int i = atoi(address);
	parser.heap.insertAliasAt(i, alias);
}

void Lexical::evalDo() {
	Loop_s newLoop;

	//find open bracket for loop start
	int i = index;
	while (i != fileSize) {
		if (code[i] == '{') {
			newLoop.start = i +1;
			break;
		}
	}

	if (newLoop.start == -1) {
		//Wrong syntax missing open bracket DO CRASH!!!
	}

	newLoop.stop = -1;
	newLoop.type = 1;
	++loopLen;
	loop[loopLen] = newLoop;
}

void Lexical::evalWhile() {
	//type = 1 do while, type = 0 while.
	char *res = parser.regularExpression(expression);
	if (global.strCmp(res, "true")) {
		//do while loop.
		if(loop[loopLen].end == -1 && loop[loopLen].type == 1) {
			Loop_s *l = &loop[loopLen];
			l->end = startIndex - 20;
			l->stop = -1;

			index = l->start;
			return;
		} else if(loop[loopLen].type == 1) {
			index = loop[loopLen].start;
			return;
		}

		if (loop[loopLen].type == -1) {
			++loopLen;

			loop[loopLen].start = startIndex - instructionLen - strlen(keyword);
			loop[loopLen].stop = 0;
			loop[loopLen].type = 2;

			int nested = 0;
			//get end of loop
			for (int i = startIndex; i != fileSize; ++i) {
				if (code[i] == ':' && code[i + 1] == 'w' && code[i + 2] == 'h' && code[i + 3] == 'i' && code[i + 4] == 'l' && code[i + 5] == 'e') {
					++nested;
					loop[loopLen + nested].start = i;
					loop[loopLen + nested].stop = 0;
					loop[loopLen + nested].type = 2;
				}

				if (code[i] == '}') {
					if (nested > 0) {
						loop[loopLen + nested].end = i;
						--nested;
					} else {
						loop[loopLen].end = i;
						for (int k = loopLen; k != 200; ++k) {
							if (loop[k].end == -1) {
								loopLen = k - 1;
								break;
							}
						}
						break;
					}
				}
			}
		} else if (loop[loopLen].type == 2 && loop[loopLen +1].end != -1) {
			++loopLen;
		}
		 //TODO stop är 1 behöver nollställas!
	} else if (global.strCmp(res, "false")) {
		if (loopLen == 1) {
			loop[loopLen].stop = 1; //stopping outer loop and continue to read.
			//--loopLen;
		} else {
			--loopLen;
		}
	}
}

void Lexical::evalCall(int len) {
	int found = 0;
	int i;
	for (i = 0; i != subroutinesLen; ++i) {
		CallableUnit_s *s = &subroutines[i];
		if (global.strCmp(s->name, expression)) {
			++callsLen;
			calls[callsLen].pos = index - len;
			int len = strlen(s->name);
			memcpy(calls[callsLen].name, s->name, len);
			calls[callsLen].name[len] = '\0';

			index = s->startPos;
			currentSubroutine = *s;
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
	Parts_s parts[NUMBEROFPRINTARGUMENTS];
	int len = 0;
	parser.parsePrint(expression, parts, len);
	
	for (int i = 0; i != len; ++i) {
		if (parts[i].type == 2) {
			char *and = strstr(parts[i].part, "&");
			char *adress = strstr(parts[i].part, "#");
			if (and || adress) {
				parser.isAdress = true;
			}

			parts[i].part[parts[i].len] = '\0';
			char *out = parser.regularExpression(parts[i].part);
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

		} else {
			parts[i].part[parts[i].len] = '\0';
			printf("\n%s", parts[i].part);
		}
	}
}

void Lexical::evalIf() {
	trimBothParanthesis(expression);

	char *res = parser.regularExpression(expression);

	if (global.strCmp(res, "true")) {
		cmpResult = 1;
	} else if (global.strCmp(res, "false")) {
		cmpResult = 0;
	} else {
		//Something went completly wrong, DO CRASH!!!
	}
}

char* Lexical::registerAllIncludes() {
	int i = 0;
	bool isContinue = true;
	const int lenInclude = 8;
	int lenCode = fileSize;
	do {
		int offset = 0;
		char *foundInclude = strstr(code, ":include");
		int p = foundInclude - code;
		int lenPath = 0;
		if (foundInclude) {
			const char *foundSemicolon = strstr(code + p, ";");
			if (foundSemicolon) {
				char path[INSTRUCTIONSIZE];
				lenPath = foundSemicolon - foundInclude - lenInclude;
				offset = lenInclude;
				memcpy(path, foundInclude + offset, lenPath);
				path[lenPath] = '\0';
				offset += lenPath;

				trimWhitespaces(path);
				trimNewline(path);
				trimBothParanthesis(path);
				trimText(path);
				char *extendedCode = readFile(path);
				int lenExtended = strlen(extendedCode);

				char *codeBefore = DBG_NEW char[lenCode];
				int lenCodeBefore = foundInclude - code;
				memcpy(codeBefore, code, lenCodeBefore);
				codeBefore[lenCodeBefore] = '\0';

				char *codeAfter = DBG_NEW char[lenCode];
				offset += lenCodeBefore;
				int lenCodeAfter = abs(lenCode - offset);
				memcpy(codeAfter, code + offset, lenCodeAfter);
				codeAfter[lenCodeAfter] = '\0';

				delete code;
				code = NULL;
				code = DBG_NEW char[lenCode + lenExtended];
				memcpy(code, codeBefore, lenCodeBefore);
				offset = lenCodeBefore;
				memcpy(code + offset, extendedCode, lenExtended);
				offset += lenExtended;
				memcpy(code + offset, codeAfter, lenCodeAfter);
				lenCode = lenCodeAfter + lenCodeBefore + lenExtended;
				code[lenCode] = '\0';

				fileSize = lenCode;
			}

		} else {
			return code;
		}
	} while (isContinue);
	return code;
}

bool Lexical::isCorrectFileType(char *cStr) {
	char *fileType = strstr(cStr, ".q");
	if (fileType) {
		return true;
	}
	return false;
}

bool Lexical::isCorrectMainFileType(char *cStr) {
	char *fileType = strstr(cStr, ".main.q");
	if (fileType) {
		return true;
	}
	return false;
}

void Lexical::evalCodeInsideStruct(char *structCode) {
	char *oldCode = code;
	setCode(structCode);
	getInstructions();
	setCode(oldCode);
}

void Lexical::evalExpressionWithoutKeyword() {
	char tmpLhs[INSTRUCTIONSIZE];
	char tmpRhs[INSTRUCTIONSIZE];
	char tmpStr[INSTRUCTIONSIZE];
	
	char *eq = strstr(expression, "=");
	if (eq) {
		char *res = parser.regularExpression(expression);

		/*int len0 = 0; 
		int len1 = 0;
		len0 = strlen(eq) -1;
		memcpy(tmpRhs, eq +1, len0);
		tmpRhs[len0] = '\0';
		char *resRhs = parser.regularExpression(tmpRhs);
		memcpy(buffer, resRhs, strlen(buffer));

		len1 = strlen(expression) - len0 -1;
		memcpy(tmpLhs, expression, len1);
		tmpLhs[len1] = '\0';
		char *resLhs = parser.regularExpression(tmpLhs);

		len0 = strlen(resLhs);
		memcpy(tmpStr, resLhs, len0);
		memcpy(tmpStr + len0, "=", 1);
		len0 += 1;

		len1 = strlen(resRhs);
		memcpy(tmpStr + len0, buffer, len1);
		len0 += len1;
		tmpStr[len0] = '\0';

		parser.calculateResult(tmpStr);*/
	}
}

void Lexical::splitInstruction(char *instruction) {
	char tmp[INSTRUCTIONSIZE];
	char tmp2[INSTRUCTIONSIZE];
	int isKeywordMissing = 1;
	int len = 0;

	const char *print = strstr(instruction, ":print("); //search before remove whitespaces.
	if (print) {
		keyword = ":print";
		len = strlen(print) - 8;
		memcpy(tmp, print + 7, len);
		tmp[len] = '\0';
		expression = tmp;
		evalPrint();
		isKeywordMissing = 0;
	}

	trimWhitespaces(instruction);

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

	if (alias) {
		keyword = ":alias";
		len = strlen(alias) -6;
		memcpy(tmp, alias +6, len);
		tmp[len] = '\0';
		expression = tmp;
		evalAlias();
		isKeywordMissing = 0;
		return;
	} 

	if (DO) {
		evalDo();
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

		evalCall(len);
		isKeywordMissing = 0;
	}

	if (subroutine) {
		ignore = 1;
		isKeywordMissing = 0;
	}

	if (isKeywordMissing == 1) {
		keyword = "regularExpression";
		trimWhitespaces(instruction);
		trimNewline(instruction);
		trimTabb(instruction);
		trimBracket(instruction);
		trimSemicolon(instruction);
		len = strlen(instruction);
		memcpy(tmp, instruction, len);
		tmp[len] = '\0';
		expression = instruction;

		evalExpressionWithoutKeyword();
	}
}

void Lexical::resetIndex() {
	index = 0;
	startIndex = 0;
	endIndex = 0;
	instructionLen = 0;
}

void Lexical::getInstructions() {
	int comment = 0;
	char instruction[INSTRUCTIONSIZE];
	resetIndex();

	while (code[index] != '\0') {	
		//Comments
		if (code[index] == '/' && code[index +1] == '*') {
				comment = 1;
		} else if (code[index] == '*' && code[index +1] == '/') {
			comment = 0;
			startIndex = index + 2;
		}

		if (code[index] == '}') {
			ignore = 0;
		}

		if (index == loop[loopLen].end) {
			ignore = 0;
			if (loop[loopLen].stop == 0) {
				startIndex = loop[loopLen].start;
				index = loop[loopLen].start;
			} else if (loop[loopLen].stop == 1) {
				startIndex = loop[loopLen].end;
				index = loop[loopLen].end;
				resetLoopArray();
			}
		}

		if (index == currentSubroutine.endPos) {
			for (int i = 0; i != subroutinesLen; ++i) { 
				currentSubroutine = subroutines[i];
				if (global.strCmp(currentSubroutine.name, calls[callsLen].name)) {
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
				}
			}
			startIndex = index;
		}
		++index;
	}
}
