#include "Heap.h"
#include "memoryLeak.h"
#include "Trim.h"
#include "ErrorCodes.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

Heap::Heap() {}

Heap::~Heap() {
	if (heapIndex) {
		delete[] heapIndex;
		heapIndex = NULL;
	}
	
	if (heapContainer) {
		delete [] heapContainer;
		heapContainer = NULL;
	}
}

void Heap::setHeapSize(size_t size) {
	if (heapIndex) {
		errorManager.ErrorCode(CODE_15);
	}

	if (size <= 0) {
		errorManager.ErrorCode(CODE_14);
	}

	heapIndex = DBG_NEW Index_s[size];
	heapContainer = DBG_NEW char[size];
	heapSize = size;

	expandHeapIndexStruct();
}

void Heap::insertAliasAt(int index, Alias_s alias) {
	if (alias.len > 0 && index >= 0 && index < GetStackSize()) {
		errorManager.ErrorCode(CODE_30);
	} else {
		insertAt(index, alias);
	}
}

void Heap::insertAt(int index, Alias_s alias) {
	// Heap overflow
	if ((alias.len + index) <= heapSize) {
		int len = 0;
		//data type
		len = strlen(alias.type);
		memcpy(heapIndex[index].type, alias.type, len);
		heapIndex[index].type[len] = '\0';

		len = strlen(alias.value);
		(heapIndex + index)->len = len;

		(heapIndex + index)->startPos = index;

		//Insert value.
		if (len > 0) {
			if (global.strCmp((char *)alias.type, "string")) {
				memcpy(heapContainer + index, alias.value, len);

			} else if (global.strCmp(alias.type, "int")) {
				if (strlen(alias.value) > 9) {
					errorManager.ErrorCode(CODE_3511);
				}
				int digits = atoi(alias.value);
				memcpy(heapContainer + index, (char *)&digits, sizeof(int));
			}
		}

		if (strlen(alias.name) > 0) {
			int len = strlen(alias.name);
			memcpy((heapIndex + index)->name, alias.name, len);
			heapIndex[index].name[len] = '\0';
		}
	} else {
		//Trying to write outside allocated memory.
		errorManager.ErrorCode(CODE_10);
	}
}

//Specialcases for content inside struct.
void Heap::insertStructIndex(Index_s index) {
	if (indexStructLen >= indexStructMax) {
		expandHeapIndexStruct();
	}

	heapIndexStructs[indexStructLen] = index;
	++indexStructLen;
}

void Heap::expandHeapIndexStruct() {
	if (heapIndexStructs == NULL) {
		indexStructMax = 100;
		heapIndexStructs = DBG_NEW Index_s[indexStructMax * sizeof(Index_s)];
	} else {
		Index_s *tmpArr = heapIndexStructs;

		indexStructMax *= 2;
		heapIndexStructs = DBG_NEW Index_s[indexStructMax * sizeof(Index_s)];
		memcpy(heapIndexStructs, tmpArr, indexStructLen * sizeof(Index_s));

		delete[] tmpArr;
	}
}

int Heap::getAddress(char *name) {
	int ret = -1;
	int i = 0;
	while (i != heapSize) {
		if (global.strCmp(heapIndex[i].name, name)) {
			ret = heapIndex[i].startPos;
			break;
		} else if (global.strCmp(heapIndexStructs[i].name, name)) {
			ret = heapIndexStructs[i].startPos;
			break;
		}
		++i;
	}
	return ret;
}

Alias_s Heap::getAlias(const char *name) {
	Alias_s alias = { "\0", "\0" , "\0", 0 };
	int i = 0;
	int len = 0;
	while (i != heapSize) {
		if (global.strCmp(heapIndex[i].name, name)) {
			len = strlen(heapIndex[i].name);
			memcpy(alias.name, heapIndex[i].name, len);
			alias.name[len] = '\0';

			alias.len = heapIndex[i].len;

			len = strlen(heapIndex[i].type);
			memcpy(alias.type, heapIndex[i].type, len);
			alias.type[len] = '\0';

			if (global.strCmp(alias.type, "string")) {
				memcpy(alias.value, &heapContainer[heapIndex[i].startPos], alias.len);
				alias.value[alias.len] = '\0';
			} else if (global.strCmp(alias.type, "int")) {
				//convert char to int and int to char
				int dest;
				memcpy(&dest, &heapContainer[i], sizeof(int));
				sprintf(alias.value, "%i", dest);
				
				len = global.intLength(dest);

				//is it negative
				if (global.isNegativeNumber(alias.value)) {
					len += 1;
				}

				alias.value[len] = '\0';
				alias.len = sizeof(int);
			}
			break;
		} else if (global.strCmp(heapIndexStructs[i].name, name)) {
			//structs
			len = strlen(heapIndexStructs[i].name);
			memcpy(alias.name, heapIndexStructs[i].name, len);
			alias.name[len] = '\0';

			alias.len = heapIndexStructs[i].len;

			len = strlen(heapIndexStructs[i].type);
			memcpy(alias.type, heapIndexStructs[i].type, len);
			alias.type[len] = '\0';

			if (global.strCmp(alias.type, "string")) {
				memcpy(alias.value, &heapContainer[heapIndexStructs[i].startPos], alias.len);
				alias.value[alias.len] = '\0';
			} else if (global.strCmp(alias.type, "int")) {
				//convert char to int and int to char
				int dest;
				memcpy(&dest, &heapContainer[heapIndexStructs[i].startPos], sizeof(int));
				sprintf(alias.value, "%i", dest);
				len = global.intLength(dest);
				alias.value[len] = '\0';
			} else if (!global.strCmp(alias.type, "offset")) {
				int k = 0;
				while (k != heapSize) {
					Index_s s = heapIndexStructs[k];
					if (heapIndexStructs[k].startPos == heapIndexStructs[i].startPos) {
							len = strlen(heapIndexStructs[k].type);
							memcpy(alias.type, heapIndexStructs[k].type, len);
							alias.type[len] = '\0';

							if (global.strCmp(alias.type, "string")) {
								memcpy(alias.value, &heapContainer[heapIndexStructs[i].startPos], alias.len);
								alias.value[alias.len] = '\0';
							} else if (global.strCmp(alias.type, "int")) {
								//convert char to int and int to char
								int dest;
								memcpy(&dest, &heapContainer[heapIndexStructs[i].startPos], sizeof(int));
								sprintf(alias.value, "%i", dest);
								len = global.intLength(dest);
								alias.value[len] = '\0';
								alias.len = sizeof(int);
							}
							break;
					}
					++k;
				}
			}
			break;
		}
		++i;
	}
	return alias;
}

Alias_s Heap::getAlias(const int index) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	int len = strlen(heapIndex[index].name);

	memcpy(alias.name, heapIndex[index].name, len);
	alias.name[len] = '\0';

 	alias.len = heapIndex[index].len;

	len = strlen(heapIndex[index].type);
	memcpy(alias.type, heapIndex[index].type, len);
	alias.type[len] = '\0';

	if (global.strCmp(alias.type, "string")) {
		memcpy(alias.value, &heapContainer[heapIndex[index].startPos], alias.len);
		alias.value[alias.len] = '\0';
	} else if (global.strCmp(alias.type, "int")) {
		//convert char to int and int to char
		int dest;
		memcpy(&dest, &heapContainer[index], sizeof(int));
		sprintf(alias.value, "%i", dest);
		len = global.intLength(dest);
		alias.value[len] = '\0';
		alias.len = sizeof(int);
	}
	return alias;
}

Index_s Heap::getStructIndex(const char *name) {
	Index_s index = { NULL, NULL, 0, 0 };
	for (int i = 0; i != indexStructLen; ++i) {
		if (global.strCmp(heapIndexStructs[i].name, name)) {
			index = heapIndexStructs[i];
			break;
		}
	}
	return index;
}

void Heap::getStructIndex(const char *type, Index_s *indexes, int &len) {
	int lenRetIndexes = 0;
	for (int i = 0; i != indexStructLen; ++i) {
		const char *name = heapIndexStructs[i].name;
		const char *slash = strstr(name, ".");
		if (slash) {
			int len = strlen(name) - strlen(slash);
			memcpy(tmpStr, name, len);
			tmpStr[len] = '\0';
			if (global.strCmp(tmpStr, type)) {
				indexes[lenRetIndexes] = heapIndexStructs[i];
				++lenRetIndexes;
			}
		}
	}
	len = lenRetIndexes;
}

void Heap::updateHeapIndex(Index_s index) {
	for (int i = 0; i != heapSize; ++i) {
		if (global.strCmp(heapIndex[i].name, index.name)) {
			heapIndex[i].len = index.len;
			heapIndex[i].startPos = index.startPos;
			int lenType = strlen(index.type);
			memcpy(heapIndex[i].type, index.type, lenType);
			heapIndex[i].type[lenType] = '\0';
			break;
		}
	}
}

bool Heap::updateStructIndex(Index_s index, char *searchName) {
	for (int i = 0; i != indexStructLen; ++i) {
		if (global.strCmp(heapIndexStructs[i].name, searchName)) {
			heapIndexStructs[i].len = index.len;
			heapIndexStructs[i].startPos = index.startPos;
			int lenType = strlen(index.type);
			memcpy(&heapIndexStructs[i].type, index.type, lenType);
			heapIndexStructs[i].type[lenType] = '\0';
			
			int lenName = strlen(index.name);
			memcpy(&heapIndexStructs[i].name, index.name, lenName);
			heapIndexStructs[i].name[lenName] = '\0';
			return true;
		}
	}
	return false;
}

bool Heap::findStructIndex(Index_s index) {
	for (int i = 0; i != indexStructLen; ++i) {
		if (global.strCmp(heapIndexStructs[i].name, index.name)) {
			return true;
		}
	}
	return false;
}

Index_s Heap::findStructIndex(char *name) {
	Index_s index = { NULL, NULL, NULL, 0 };
	for (int i = 0; i != indexStructLen; ++i) {
		if (global.strCmp(heapIndexStructs[i].name, name)) {
			return heapIndexStructs[i];
		}
	}
	return index;
}

void Heap::typedefStructMembers(char *searchName, char *extendName) {
	Index_s buffer[1024];
	int bufferLen = 0;
	int len1 = 0;
	int len2 = 0;
	for (int i = 0; i != indexStructLen; ++i) {
		Index_s *s = &heapIndexStructs[i];
		char *res = strstr(s->name, searchName);
		if (res) {
			char *findDot = strstr(s->name, ".");
			if (findDot) {
				len1 = strlen(searchName);
				memcpy(tmpStr, searchName, len1);
				memcpy(tmpStr + len1, ".", 1);
				len1 += 1;
				len2 = strlen(extendName);
				memcpy(tmpStr + len1, extendName, len2);
				memcpy(tmpStr + len1 + len2, ".", 1);
				len2 += len1;
				len1 = strlen(findDot);
				memcpy(tmpStr + len2, findDot, len1);
				tmpStr[len2 + len1] = '\0';
				len1 += len2;

				//Create a new struct member.
				Index_s newIndex;
				memcpy(newIndex.name, tmpStr, len1);
				newIndex.name[len1] = '\0';

				newIndex.len = s->len;
				newIndex.startPos = s->startPos;

				len1 = strlen(s->type);
				memcpy(newIndex.type, s->type, len1);
				newIndex.type[len1] = '\0';
				buffer[bufferLen] = newIndex;
				++bufferLen;
			} else {
				//Syntax struct is wrong Do CRASH!!!
			}
		}
	}
	//Insert struct members into struct index array.
	for (int i = 0; i != bufferLen; ++i) {
		if (indexStructLen < indexStructMax) {
			heapIndexStructs[indexStructLen] = buffer[i];
			++indexStructLen;
		}
	}
}

void Heap::updateStructHeaderPointer(Index_s index) {
	char buffer[INSTRUCTIONSIZE];
	bool res = global.findSubStrRev(buffer, index.name, ".");
	if (res) {
		int len = strlen(buffer) - 1;
		buffer[len] = '\0';
		for (int i = 0; i != indexStructLen; ++i) {
			Index_s *currentIndex = &heapIndexStructs[i];
			if (global.strCmp(currentIndex->name, buffer) && currentIndex->type[0] == '\0') {
				currentIndex->startPos = index.startPos;

				len = strlen(index.type);
				memcpy(currentIndex->type, index.type, len);
				currentIndex->type[len] = '\0';

				currentIndex->len = index.len;
				break;
			}
		}
	}
}

char* Heap::getFullNameStructMember(char *lastname) {
	bool andFound = global.findAnd(lastname);

	char searchName[NAMESIZE];
	int lenSearchName = strlen(lastname);
	memcpy(searchName, lastname, lenSearchName);
	searchName[lenSearchName] = '\0';

	trimAnd(searchName);

	char buffer[NAMESIZE];
	int len = 0;
	int firstDot = 0;
	int lenFirstWord = 0;
	for (int i = 0; i != indexStructLen; ++i) {	
		char *name = heapIndexStructs[i].name;
		int k = strlen(name);
		while (k != 0) {
			if (name[k] == '.') {
				if (firstDot == 0) {
					firstDot = strlen(searchName) - k;
					lenFirstWord = k;
				}
				len = strlen(name) - k -1;
				memcpy(buffer, name + k +1, len);
				buffer[len] = '\0';

				if (global.strCmp(buffer, searchName)) {
					if (andFound) {
						len = strlen(name);
						memcpy(buffer, "&", 1);
						memcpy(buffer +1, name, len);
						buffer[len +1] = '\0';
						return buffer;
					}
					return name;
				}
			}
			--k;
		}
	}
	return NULL;
}

void Heap::initializeHeap(size_t size) {
	setHeapSize(size);
	heapStartPos = 0;
}

void Heap::createStack(size_t size) {
	if (GetStackSize() > 0) {
		errorManager.ErrorCode(CODE_16);
	}

	if (size <= 0) {
		errorManager.ErrorCode(CODE_13);
	}

	SetStackSize(size);
	SetStackLen(0);

	if (GetStackSize() > heapSize) {
		//Stack cannot be bigger then heap.
		errorManager.ErrorCode(CODE_12);
	}
	heapStartPos = heapSize - GetStackSize();
}

void Heap::pushTop(Alias_s alias) {
	if (!isStackOverflow(GetStackLen(), alias.len)) {
		if (GetStackLen() > 0) {
			insertAt(GetStackLen() + 1, alias);
		} else {
			insertAt(GetStackLen(), alias);
		}
		SetStackLen(GetStackLen() + alias.len);
	}
}

void Heap::pushAt(int index, Alias_s alias) {
	if (!isStackOverflow(index, alias.len)) {
		insertAt(index, alias);
	}
}

Alias_s Heap::getTop() {
	int len = GetStackLen();
	if (len > 0) { len -= 1; }
	Alias_s alias = { NULL, NULL, NULL, 0 };
	if (!isStackOverflow(len, 0)) {
		for (int i = len; i >= 0; --i) {
			if (heapIndex[i].startPos != len) {
				--len;
			} else {
				alias = getAlias(i);
				break;
			}
		}
	}
	return alias;
}

Alias_s Heap::getAt(int index) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	if (!isStackOverflow(index, 0)) {
		alias = getAlias(index);
	}
	return alias;
}

void Heap::pop() {
	if (GetStackLen() > 0) {
		SetStackLen(GetStackLen() - 1);
		Alias_s alias = { NULL, NULL, NULL, 0 };
		int len = GetStackLen();
		for (int i = len; i >= 0; --i) {
			if (heapIndex[i].startPos != len) {
				--len;
			} else {
				alias = getAlias(i);
				alias.len -= 1;
				alias.value[alias.len] = '\0';
				insertAt(i, alias);
				break;
			}
		}
	}
}

void Heap::popTop() {
	for (int i = GetStackLen(); i >= 0; --i) {
		if (heapIndex[i].startPos != GetStackLen()) {
			SetStackLen(GetStackLen() - 1);
		} else {
			break;
		}
	}
}

bool Heap::isStackOverflow(int index, int len) {
	if (index + len >= (GetStackSize() +2) || index < 0) {
		errorManager.ErrorCode(CODE_11);
	}
	return false;
}
