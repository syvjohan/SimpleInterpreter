#include "Lexical.h"
#include "HelpHeaders.h"
#include "ErrorHeaders.h"

#include <errno.h>

namespace Partitioning {
	Lexical::Lexical() {
		ExpandCallsSize();
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

		if (structs) {
			delete[] structs;
			structs = NULL;
		}

		for (int i = lenFiles - 1; i >= 0; --i) {
			if (files[i].index) {
				delete[] files[i].index;
				files[i].index = NULL;
			}
		}
	}

	size_t Lexical::CalculateFileSize(const char *path) {
		FILE *file;
		file = fopen(path, "r");

		if (file == NULL) {
			printf("Error CODE_90: Wrong file path,\n");
			perror(path);
		}

		//get size of file.
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		fseek(file, 0, SEEK_SET);
		
		return size +1;
	}

	void Lexical::ReadFile(char *buffer, const char *path) {
		FILE *file;
		file = fopen(path, "r");

		if (file == NULL) {
			printf("Error CODE_90: Wrong file path,\n");
			perror(path);
		}

		//read file into string
		int c;
		int i = 0;
		while ((c = fgetc(file)) != EOF) {
			buffer[i] = (char)c;
			++i;
		}
		buffer[i] = '\0';

		fclose(file);
	}

	void Lexical::SetCode(const char *cStr) {
		code = (char *)cStr;
	}

	void Lexical::AllocateMem() {
		Global::trimText(expression);

		//Allocate heap memory.
		const int s = atoi(expression);
		parser.heap.InitializeHeap(s);
	}

	void Lexical::CreateStack() {
		Global::trimText(expression);

		//Create stack on heap
		const int s = atoi(expression);
		parser.heap.CreateStack(s);
	}

	void Lexical::ExpandCallsSize(void) {
		if (calls == NULL) {
			callsMax = 100;
			calls = DBG_NEW Global::Call_s[callsMax * sizeof(Global::Call_s)];
		} else {
			Global::Call_s *tmpArr = calls;

			callsMax *= 2;
			calls = DBG_NEW Global::Call_s[callsMax * sizeof(Global::Call_s)];
			memcpy(calls, tmpArr, callsLen * sizeof(Global::Call_s));

			delete[] tmpArr;
		}
	}

	void Lexical::ExpandStructsSize() {
		if (structs == NULL) {
			structsMax = 100;
			structs = DBG_NEW Global::CallableUnit_s[structsMax * sizeof(Global::CallableUnit_s)];
		} else {
			Global::CallableUnit_s *tmpArr = subroutines;

			structsMax *= 2;
			structs = DBG_NEW Global::CallableUnit_s[structsMax * sizeof(Global::CallableUnit_s)];
			memcpy(structs, tmpArr, structsLen * sizeof(Global::CallableUnit_s));

			delete[] tmpArr;
		}
	}

	void Lexical::ExpandSubroutineSize(void) {
		if (subroutines == NULL) {
			subroutinesMax = 100;
			subroutines = DBG_NEW Global::CallableUnit_s[subroutinesMax * sizeof(Global::CallableUnit_s)];
		} else {
			Global::CallableUnit_s *tmpArr = subroutines;

			subroutinesMax *= 2;
			subroutines = DBG_NEW Global::CallableUnit_s[subroutinesMax * sizeof(Global::CallableUnit_s)];
			memcpy(subroutines, tmpArr, subroutinesLen * sizeof(Global::CallableUnit_s));

			delete[] tmpArr;
		}
	}

	//Makes it possible to call a subroutine that has been declared after it is called.
	void Lexical::RegisterAllSubroutines(void) {
		bool insideComment = false;
		int i = 0;
		while (code[i] != '\0') {
			//Comments
			if (code[i] == '/' && code[i + 1] == '*') {
				insideComment = true;
			} else if (code[i] == '*' && code[i + 1] == '/') {
				insideComment = false;
			}

			if (!insideComment) {
				if (code[i] == ':' && code[i + 1] == 's' && code[i + 2] == 'u' && code[i + 3] == 'b' && code[i + 4] == 'r' &&
					code[i + 5] == 'o' && code[i + 6] == 'u' && code[i + 7] == 't' && code[i + 8] == 'i' &&
					code[i + 9] == 'n' && code[i + 10] == 'e') {
					char *findSub = code + i + 11;

					const char *findOpenBracket = strstr(findSub, "{");
					const char *findCloseBracket = strstr(findSub, "}");

					//Close bracket missing.
					const char *findWrongOpenBracket = strstr(findOpenBracket + 1, "{");
					if (findWrongOpenBracket) {
						if (findWrongOpenBracket < findCloseBracket) {
							Error::ErrorManager::ErrorCode(Error::CODE_52);
						}
					}

					//Open bracket missing.
					if (findCloseBracket) {
						if (findOpenBracket) {
							if (findOpenBracket > findCloseBracket) {
								Error::ErrorManager::ErrorCode(Error::CODE_51);
							}
						} else {
							Error::ErrorManager::ErrorCode(Error::CODE_51);
						}
					} else {
						Error::ErrorManager::ErrorCode(Error::CODE_54);
					}

					if (findOpenBracket) {
						int len = strlen(findSub) - strlen(findOpenBracket);
						char name[NAMESIZE];
						if (len > NAMESIZE) {
							Error::ErrorManager::ErrorCode(Error::CODE_54);
						}
						memcpy(name, findSub, len);
						name[len] = '\0';
						Global::trimText(name);
						Global::trimWhitespaces(name);
						if (name[0] == '\0') {
							Error::ErrorManager::ErrorCode(Error::CODE_50);
						}

						const char *ret = strstr(findSub, "};");
						if (ret) {
							Global::CallableUnit_s subroutine;
							subroutine.endPos = ret - code;
							subroutine.startPos = findOpenBracket - code;
							memcpy(subroutine.name, name, len);
							subroutine.name[len] = '\0';

							if (subroutinesLen == subroutinesMax) {
								ExpandSubroutineSize();
							}

							//Check if subroutine is inside struct.
							for (int i = 0; i != structsLen; ++i) {
								Global::CallableUnit_s *s = &structs[i];
								if (subroutine.startPos > s->startPos && subroutine.endPos < s->endPos) {
									char tmpName[NAMESIZE];
									len = strlen(subroutine.name);
									memcpy(tmpName, subroutine.name, len);
									int lenStructName = strlen(s->name);
									memcpy(subroutine.name, s->name, lenStructName);
									memcpy(subroutine.name + lenStructName, ".", 1);
									memcpy(subroutine.name + lenStructName + 1, tmpName, len);
									subroutine.name[len + lenStructName + 1] = '\0';
									break;
								}
							}

							subroutines[subroutinesLen] = subroutine;
							++subroutinesLen;

							i = subroutine.endPos; //sets the program counter to a new value.
						} else {
							//Missing end of subroutine };.
							Error::ErrorManager::ErrorCode(Error::CODE_52);
						}
					} else {
						//Missing subroutine open bracket.
						Error::ErrorManager::ErrorCode(Error::CODE_51);
					}
				}
			}
			++i;
		}
	}

	void Lexical::RegisterAllStructs() {
		bool insideComment = false;
		int i = 0;
		while (code[i] != '\0') {
			//Comments
			if (code[i] == '/' && code[i + 1] == '*') {
				insideComment = true;
			} else if (code[i] == '*' && code[i + 1] == '/') {
				insideComment = false;
			}

			if (!insideComment) {
				if (code[i] == ':' && code[i + 1] == 's' && code[i + 2] == 't' && code[i + 3] == 'r' && code[i + 4] == 'u' &&
					code[i + 5] == 'c' && code[i + 6] == 't') {
					char *findStruct = code + i + 7;

					const char *findOpenBracket = strstr(findStruct, "{");
					const char *findCloseBracket = strstr(findStruct, "};");

					//Open bracket missing.
					if (findCloseBracket) {
						if (findOpenBracket) {
							if (findOpenBracket > findCloseBracket) {
								Error::ErrorManager::ErrorCode(Error::CODE_41);
							}
						} else {
							Error::ErrorManager::ErrorCode(Error::CODE_41);
						}
					} else {
						Error::ErrorManager::ErrorCode(Error::CODE_43);
					}

					if (findOpenBracket) {
						int lenOpenBracket = strlen(findOpenBracket);
						int lenName = strlen(findStruct) - lenOpenBracket;
						char name[NAMESIZE];
						if (lenName > NAMESIZE) {
							Error::ErrorManager::ErrorCode(Error::CODE_44);
						}
						memcpy(name, findStruct, lenName);
						name[lenName] = '\0';
						Global::trimText(name);
						Global::trimWhitespaces(name);
						if (name[0] == '\0') {
							Error::ErrorManager::ErrorCode(Error::CODE_40);
						}

						//count open brackets for ignore.
						int indexOpen = 0;
						int indexClose = 0;
						int moveIndexTo = 0;
						for (int i = 0; i != fileSize; ++i) {
							if (findStruct[i] == '{') {
								++indexOpen;
							} else if (findStruct[i] == '}') {
								++indexClose;
								if (indexClose == indexOpen) {
									moveIndexTo = i - 1;
									break;
								}
							}
						}

						char *ret = strstr(findStruct + moveIndexTo, "};");
						if (ret) {
							//check if there has been a new open bracket before close bracket was found.
							char *wrongOpenBracket = strstr(findStruct + moveIndexTo, "{");
							if (wrongOpenBracket) {
								if (ret > wrongOpenBracket) {
									Error::ErrorManager::ErrorCode(Error::CODE_42);
								}
							}

							Global::CallableUnit_s newStruct;
							newStruct.endPos = ret - code;
							newStruct.startPos = findOpenBracket - code;
							memcpy(newStruct.name, name, lenName);
							newStruct.name[lenName] = '\0';

							if (structsLen == structsMax) {
								ExpandStructsSize();
							}

							structs[structsLen] = newStruct;
							++structsLen;

							i = newStruct.endPos; //sets the program counter to a new value.

							//Get code inside struct.
							int lenInsideStruct = lenOpenBracket - strlen(ret);
							char *codeInsideStruct = DBG_NEW char[lenInsideStruct + 1];
							memcpy(codeInsideStruct, findOpenBracket, lenInsideStruct);
							codeInsideStruct[lenInsideStruct] = '\0';

							memcpy(currentStructName, name, lenName);
							isInitializingStructs = true;
							EvalCodeInsideStruct(codeInsideStruct);

							//Free memory
							delete[] codeInsideStruct;
							codeInsideStruct = NULL;

							isInitializingStructs = false;
						} else {
							//Missing end of struct };.
							Error::ErrorManager::ErrorCode(Error::CODE_42);
						}
					} else {
						//Missing struct open bracket.
						Error::ErrorManager::ErrorCode(Error::CODE_41);
					}
				}
			}
			++i;
		}
	}

	void Lexical::UpdateStructsIndexes() {
		structsLen = 0;
		RegisterAllStructs();
	}

	void Lexical::UpdateSubroutinesIndexes() {
		subroutinesLen = 0;
		RegisterAllSubroutines();
	}

	void Lexical::TypedefSubroutines(const char *searchName, const char *extendName) {
		char str[NAMESIZE];
		int len1 = 0;
		int len2 = 0;
		for (int i = 0; i != subroutinesLen; ++i) {
			Global::CallableUnit_s *s = &subroutines[i];
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

	void Lexical::TypedefSubroutinesMembers(const char *searchName, const char *extendName) {
		char buffer[2048];
		char instruction[INSTRUCTIONSIZE];
		char newSearchName[STRUCTNAMESIZE];
		Global::Operator_s op;
		int len1 = 0;
		int len2 = 0;
		int len = 0;
		for (int i = 0; i != subroutinesLen; ++i) {
			Global::CallableUnit_s *s = &subroutines[i];
			char *res = strstr(s->name, searchName);
			if (res) {
				int position = 0;
				int lenInstructions = s->endPos - s->startPos;
				if (lenInstructions > 0) {
					memcpy(buffer, code + s->startPos, lenInstructions);
					buffer[lenInstructions] = '\0';
					Global::trimHeap(buffer);
					Global::trimNewline(buffer);
					Global::trimTabb(buffer);
					Global::trimBracket(buffer);
					Global::trimSemicolon(buffer);
					lenInstructions = strlen(buffer);
					while (position <= lenInstructions) {

						op = Global::HelpClass::FindOperator(buffer, position);
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

						Global::Index_s foundIndex = parser.heap.FindStructIndex(newSearchName);


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

						parser.heap.UpdateStructIndex(foundIndex, newSearchName);

					}
				}
			}
		}
	}

	void Lexical::ResetLoopArray() {
		for (int i = 0; i != LOOPMAX; ++i) {
			Global::Loop_s *l = &loop[i];
			l->start = -1;
			l->end = -1;
			l->stop = -1;
			l->type = -1;
		}
	}

	void Lexical::EvalAlias() {
		int len = strlen(expression);
		char *sep1 = strstr(expression, ":");
		const char *sep2 = strstr(expression, "=");
		const char *sep3 = strstr(expression, "#");
		const char *offset = strstr(expression, "offset(");
		const char *identifyType = strstr(expression, "\"");

		char address[ADDRESSSIZE];
		int lenAddress;

		char val[VALUESIZE];
		Global::Alias_s alias;

		if (!sep1) {
			//Syntax error.
			Error::ErrorManager::ErrorCode(Error::CODE_34);
		}

		int lenName = len - strlen(sep1);
		memcpy(alias.name, expression, lenName);
		alias.name[lenName] = '\0';
		if (Global::HelpClass::CheckAliasNameConversion(alias.name) == -1) {
			//Wrong name conversion.
			Error::ErrorManager::ErrorCode(Error::CODE_31);
		}

		if (sep2) {
			lenAddress = strlen(sep1) - strlen(sep2) - 2;
		} else {
			lenAddress = len - strlen(sep1) - 1;//-2
		}

		memcpy(address, sep1 + 2, lenAddress);
		address[lenAddress] = '\0';

		if (offset && isInitializingStructs) {
			//special case, alias with offset in struct.
			lenAddress = strlen(offset) - 8;
			memcpy(address, offset + 7, lenAddress);
			address[lenAddress] = '\0';

			Global::Index_s index = { NULL, NULL, 0, 0 };
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
			char *nestedStruct = strstr(sep1 + 1, ":");
			if (nestedStruct) {
				len = strlen(sep1) - strlen(nestedStruct) - 1;
				memcpy(tmp, sep1 + 1, len);
				tmp[len] = '\0';
				int i = 0;
				Global::Index_s indexes[STRUCTMEMBERS];
				int lenIndexes = 0;
				parser.heap.GetStructIndex(tmp, indexes, lenIndexes);
				for (int i = 0; i != lenIndexes; ++i) {
					const char *name = indexes[i].name;
					const char *slash = strstr(name, ".");
					int lenDot = strlen(slash) - 1;

					memcpy(index.name + lenStructName, ".", 1);
					memcpy(index.name + lenStructName + 1, slash + 1, lenDot);
					index.name[lenStructName + 1 + lenDot] = '\0';

					index.startPos += indexes[i].startPos;

					parser.heap.InsertStructIndex(index);

					//create a index for struct head pointer.
					if (indexes[i].startPos == 0) {
						Global::HelpClass::FindSubStrRev(tmp, index.name, ".");
						lenDot = strlen(tmp) - 1;
						memcpy(index.name, tmp, lenDot);
						index.name[lenDot] = '\0';

						memset(index.type, '\0', strlen(index.type));

						parser.heap.InsertStructIndex(index);
					}

				}
				return;
			}

			parser.heap.InsertStructIndex(index);
			return;

		} else if (sep2) {
			if (!sep3) {
				Error::ErrorManager::ErrorCode(Error::CODE_35);
			}
			//With definition.
			//check datatype and get val.
			if (identifyType) {
				memcpy(alias.type, "string", 6);
				alias.type[6] = '\0';

				alias.len = strlen(sep2) - 3;
				memcpy(val, sep2 + 2, alias.len);
				val[alias.len] = '\0';

			} else {

				len = strlen(sep2 + 1);
				alias.len = len;

				memcpy(val, sep2 + 1, len);
				val[len] = '\0';

				memcpy(alias.type, val, len);
				alias.type[len] = '\0';

				if (Global::HelpClass::CheckForDigits(val) == -1) {
					if (Global::HelpClass::CheckForAlpha(val) == 1) {
						Global::Index_s index = { NULL, NULL, 0, 0 };
						//Not checking if struct exist. Checking and mapping is carried out when user assign a value.
						index.len = 0;
						index.startPos = atoi(address);
						memcpy(index.type, val, len);
						memcpy(index.name, alias.name, lenName);

						parser.heap.InsertStructIndex(index);

						//structs only! insert pointer name into path. Find all structs with typename.
						bool isHit = false;
						for (int i = 0; i != structsLen; ++i) {
							Global::CallableUnit_s *s = &structs[i];
							if (Global::HelpClass::StrCmp(s->name, index.type)) {
								//typedef struct members
								parser.heap.TypedefStructMembers(index.type, index.name);
								//typedef subroutines.
								TypedefSubroutines(index.type, index.name);
								TypedefSubroutinesMembers(index.name, index.type);
								isHit = true;
							}
						}
						if (!isHit) {
							Error::ErrorManager::ErrorCode(Error::CODE_3510);
						}
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

			if (!sep3 && !offset && !isInitializingStructs) {
				Error::ErrorManager::ErrorCode(Error::CODE_35);
			}
		}

		//alias name need to contain at least one letter.
		if (alias.name[0] == '\0') {
			Error::ErrorManager::ErrorCode(Error::CODE_31);
		}

		char *parserVal = val;
		if (!identifyType) {
			//regular expression.
			if (val[0] != '\0') {
				parserVal = parser.RegularExpression(val);
				len = strlen(parserVal);
				memcpy(alias.value, parserVal, len);
				alias.value[len] = '\0';
				alias.len = len;

				//identify type.
				if (Global::HelpClass::CheckForDigits(alias.value) == 1) {
					memcpy(alias.type, "int", 3);
					alias.type[3] = '\0';
				} else if (Global::HelpClass::CheckForAlpha(alias.value) == 1) {
					memcpy(alias.type, "string", 6);
					alias.type[6] = '\0';
				} else if (Global::HelpClass::IsNegativeNumber(alias.value)) {
					//Negative number.
					memcpy(alias.type, "int", 3);
					alias.type[3] = '\0';
				} else {
					Error::ErrorManager::ErrorCode(Error::CODE_3510);
				}
			}
		} else {
			alias.len = strlen(parserVal);
			memcpy(alias.value, parserVal, alias.len);
			alias.value[alias.len] = '\0';
		}

		//Insert.
		if (!offset && !isInitializingStructs && Global::HelpClass::CheckForDigits(address) == -1) {
			Error::ErrorManager::ErrorCode(Error::CODE_35);
		}

		const int i = atoi(address);

		parser.heap.InsertAliasAt(i, alias);
	}

	void Lexical::EvalDo() {
		Global::Loop_s newLoop;

		//find open bracket for loop start
		int i = index;
		while (i != fileSize) {
			if (code[i] == '{') {
				newLoop.start = i + 1;
				break;
			}
		}

		if (newLoop.start == -1) {
			//Wrong syntax missing open bracket.
			Error::ErrorManager::ErrorCode(Error::CODE_56);
		}

		newLoop.stop = -1;
		newLoop.type = 1;
		++loopLen;
		loop[loopLen] = newLoop;
	}

	void Lexical::EvalWhile() {
		//type = 1 do while, type = 0 while.
		const char *res = parser.RegularExpression(expression);
		if (Global::HelpClass::StrCmp(res, "true")) {
			//do while loop.
			if (loop[loopLen].end == -1 && loop[loopLen].type == 1) {
				Global::Loop_s *l = &loop[loopLen];
				l->end = startIndex - 20;
				l->stop = -1;

				index = l->start;
				return;
			} else if (loop[loopLen].type == 1) {
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
			} else if (loop[loopLen].type == 2 && loop[loopLen + 1].end != -1) {
				++loopLen;
			}
			//TODO stop är 1 behöver nollställas!
		} else if (Global::HelpClass::StrCmp(res, "false")) {
			if (loopLen == 1) {
				loop[loopLen].stop = 1; //stopping outer loop and continue to read.
				//--loopLen;
			} else {
				--loopLen;
			}
		}
	}

	void Lexical::EvalCall(const int len) {
		int found = 0;
		int i;
		for (i = 0; i != subroutinesLen; ++i) {
			Global::CallableUnit_s *s = &subroutines[i];
			if (Global::HelpClass::StrCmp(s->name, expression)) {
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

	void Lexical::EvalStk() {
		Global::trimBothParanthesis(expression);
		const char *popTop = strstr(expression, "popTop");
		const char *pop = strstr(expression, "pop");
		const char *pushAt = strstr(expression, "pushAt");
		const char *pushTop = strstr(expression, "pushTop");
		const char *getAt = strstr(expression, "getAt");
		const char *getTop = strstr(expression, "getTop");

		if (popTop) {
			parser.StackPopTop();
		} else if (pop) {
			parser.StackPop();

		} else if (pushAt) {
			parser.StackPushAt(pushAt);

		} else if (pushTop) {
			parser.StackPushTop(pushTop);

		} else if (getAt) {
			parser.StackGetAt(getAt);

		} else if (getTop) {
			parser.StackGetTop();
		} else {
			//unsupported stack command.
			Error::ErrorManager::ErrorCode(Error::CODE_60);
		}
	}

	void Lexical::EvalPrint() {
		Global::Parts_s parts[NUMBEROFPRINTARGUMENTS];
		int len = 0;
		parser.ParsePrint(expression, parts, len);

		for (int i = 0; i != len; ++i) {
			if (parts[i].type == 2) {
				const char *and = strstr(parts[i].part, "&");
				const char *adress = strstr(parts[i].part, "#");
				if (and /*|| adress*/) {
					parser.isAdress = true;
				}

				parts[i].part[parts[i].len] = '\0';
				const char *out = parser.RegularExpression(parts[i].part);
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

	void Lexical::EvalIf() {
		Global::trimBothParanthesis(expression);

		const char *res = parser.RegularExpression(expression);

		if (Global::HelpClass::StrCmp(res, "true")) {
			cmpResult = 1;
		} else if (Global::HelpClass::StrCmp(res, "false")) {
			cmpResult = 0;
		} else {
			//Something went completly wrong.
			Error::ErrorManager::ErrorCode(Error::CODE_80);
		}
	}

	char* Lexical::RegisterAllIncludes() {
		bool insideComment = false;
		bool isContinue = true;
		const int lenInclude = 8;
		int lenCode = fileSize;
		int i = 0;

		while (code[i] != '\0' && isContinue) {
			int offset = 0;
			int lenPath = 0;

			//Comments
			if (code[i] == '/' && code[i + 1] == '*') {
				insideComment = true;
			} else if (code[i] == '*' && code[i + 1] == '/') {
				insideComment = false;
			}

			if (!insideComment) {
				if (code[i] == ':' && code[i + 1] == 'i' && code[i + 2] == 'n' && code[i + 3] == 'c' && code[i + 4] == 'l' &&
					code[i + 5] == 'u' && code[i + 6] == 'd' && code[i + 7] == 'e') {
					char *foundInclude = code + i + 8;

					const char *foundSemicolon = strstr(foundInclude, ";");
					if (foundSemicolon) {
						char path[INSTRUCTIONSIZE];
						lenPath = foundSemicolon - foundInclude;
						offset = lenInclude;
						memcpy(path, foundInclude, lenPath);
						path[lenPath] = '\0';
						offset += lenPath;

						Global::trimWhitespaces(path);
						Global::trimNewline(path);
						Global::trimBothParanthesis(path);
						Global::trimText(path);

						const int size = CalculateFileSize(path);
						char *extendedCode = DBG_NEW char[size];
						ReadFile(extendedCode, path);

						const int lenExtended = strlen(extendedCode);

						char *codeBefore = DBG_NEW char[lenCode];
						const int lenCodeBefore = foundInclude - code - lenInclude;
						memcpy(codeBefore, code, lenCodeBefore);
						codeBefore[lenCodeBefore] = '\0';

						char *codeAfter = DBG_NEW char[lenCode];
						offset += lenCodeBefore;
						const int lenCodeAfter = abs(lenCode - offset);
						memcpy(codeAfter, code + offset, lenCodeAfter);
						codeAfter[lenCodeAfter] = '\0';

						delete[] code;
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

						delete[] extendedCode;
						extendedCode = NULL;
						delete[] codeAfter;
						codeAfter = NULL;
						delete[] codeBefore;
						codeBefore = NULL;

						//register file indexes.
						RegisterFile(lenCodeBefore + 1, lenCodeBefore + lenExtended, path);
					}
				}
			}
			++i;
		}
		RegisterFile(0, fileSize, "main.q"); //Register .main.q file
		Error::ErrorManager::SetRegisteredFiles(files, lenFiles);
		return code;
	}

	void Lexical::RegisterFile(const int start, const int end, const char *name) {
		if (lenFiles >= MAXINCLUDEDFILES) {
			Error::ErrorManager::ErrorCode(Error::CODE_91);
		} else {
			Global::File_s file;

			file.index = DBG_NEW int[fileSize];
			file.startPos = start + 1;
			file.endPos = end;
			int lenName = strlen(name);
			memcpy(file.name, name, lenName);
			file.name[lenName] = '\0';

			files[lenFiles] = file;

			++lenFiles;
		}
	}

	bool Lexical::IsCorrectFileType(const char *cStr) {
		const char *fileType = strstr(cStr, ".q");
		if (fileType) {
			return true;
		}
		return false;
	}

	bool Lexical::IsCorrectMainFileType(const char *cStr) {
		const char *fileType = strstr(cStr, ".main.q");
		if (fileType) {
			return true;
		}
		return false;
	}

	void Lexical::EvalCodeInsideStruct(const char *structCode) {
		const char *oldCode = code;
		SetCode(structCode);
		GetInstructions();
		SetCode(oldCode);
	}

	void Lexical::EvalExpressionWithoutKeyword() {
		const char *eq = strstr(expression, "=");
		if (eq) {
			parser.RegularExpression(expression);
		}
	}

	void Lexical::SplitInstruction(char *instruction) {
		char tmp[INSTRUCTIONSIZE];
		int isKeywordMissing = 1;
		int len = 0;

		Global::trimWhitespacesExceptInTextString(instruction);

		const char *print = strstr(instruction, ":print(");
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
			AllocateMem();
			isKeywordMissing = 0;
		}

		if (sysCreateStack) {
			keyword = ":sysCreateStack";
			len = strlen(sysCreateStack) - 15;
			memcpy(tmp, sysCreateStack + 15, len);
			tmp[len] = '\0';
			expression = tmp;
			CreateStack();
			isKeywordMissing = 0;
		}

		if (alias) {
			keyword = ":alias";
			len = strlen(alias) - 6;
			memcpy(tmp, alias + 6, len);
			tmp[len] = '\0';
			expression = tmp;
			EvalAlias();
			isKeywordMissing = 0;
			return;
		}

		if (DO) {
			EvalDo();
			isKeywordMissing = 0;
		}

		if (WHILE) {
			keyword = ":while";
			len = strlen(WHILE) - 6;
			memcpy(tmp, WHILE + 7, len);
			tmp[len] = '\0';
			expression = tmp;
			EvalWhile();
			isKeywordMissing = 0;
			return;
		}

		if (IF) {
			keyword = ":if";
			len = strlen(IF) - 2;
			memcpy(tmp, IF + 3, len);
			tmp[len] = '\0';
			expression = tmp;
			EvalIf();

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

		if (print) {
			keyword = ":print";
			len = strlen(print) - 8;
			memcpy(tmp, print + 7, len);
			tmp[len] = '\0';
			expression = tmp;
			EvalPrint();
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
				EvalStk();
				isKeywordMissing = 0;
			}
		}

		if (call) {
			keyword = ":call";
			len = strlen(call) - 5;
			memcpy(tmp, call + 5, len);
			tmp[len] = '\0';
			expression = tmp;

			EvalCall(len);
			isKeywordMissing = 0;
		}

		if (subroutine) {
			ignore = 1;
			isKeywordMissing = 0;
		}

		if (isKeywordMissing == 1) {
			keyword = "regularExpression";
			Global::trimWhitespaces(instruction);
			Global::trimNewline(instruction);
			Global::trimTabb(instruction);
			Global::trimBracket(instruction);
			Global::trimSemicolon(instruction);
			len = strlen(instruction);
			memcpy(tmp, instruction, len);
			tmp[len] = '\0';
			expression = instruction;

			EvalExpressionWithoutKeyword();
		}
	}

	void Lexical::ResetIndex() {
		index = 0;
		startIndex = 0;
		endIndex = 0;
		instructionLen = 0;
	}

	void Lexical::GetInstructions() {
		int comment = 0;
		char instruction[INSTRUCTIONSIZE];
		ResetIndex();

		while (code[index] != '\0') {
			//newline
			if (code[index] == '\n') {
				Error::ErrorManager::AddLine(index);
			}

			//Comments
			if (code[index] == '/' && code[index + 1] == '*') {
				comment = 1;
			} else if (code[index] == '*' && code[index + 1] == '/') {
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
					ResetLoopArray();
				}
			}

			if (index == currentSubroutine.endPos) {
				for (int i = 0; i != subroutinesLen; ++i) {
					currentSubroutine = subroutines[i];
					if (Global::HelpClass::StrCmp(currentSubroutine.name, calls[callsLen].name)) {
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
					if (endIndex + 1 >= startIndex) {
						instructionLen = endIndex - startIndex;
						instructionLen = abs(instructionLen);
						memcpy(instruction, code + startIndex, instructionLen);
						instruction[instructionLen] = '\0';
						startIndex = endIndex + 1;
						Error::ErrorManager::SetInstruction(instruction, index);
						SplitInstruction(instruction);
					}
				}
				startIndex = index;
			}
			++index;
		}
	}
}