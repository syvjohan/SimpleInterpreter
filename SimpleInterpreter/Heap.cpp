#include "Heap.h"
#include "memoryLeak.h"

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
	heapIndex = DBG_NEW Index_s[size];
	heapContainer = DBG_NEW char[size];
	heapSize = size;

	expandHeapIndexStruct();
}

int Heap::insertAliasAt(int index, Alias_s alias) {
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

				if (index + GetStackLen() + len > heapStartPos) {
					//StackOverfloaw. DO CRASH!!
				} else if (index + len > heapSize) {
					//HeapOverflow. DO CRASH!!
				}

				memcpy(heapContainer + index, alias.value, len);

			} else if (global.strCmp(alias.type, "int")) {
				int digits = atoi(alias.value);
				if (GetStackLen() > -1) {
					if (index + GetStackLen() + sizeof(digits) > heapStartPos) {
						//StackOverfloaw. DO CRASH!!
					} else if (index + sizeof(digits) > heapSize) {
						//HeapOverflow. DO CRASH!!
					}
				}
				memcpy(heapContainer + index, (char *)&digits, sizeof(int));
			}
		}

		if (strlen(alias.name) > 0) {
			int len = strlen(alias.name);
			memcpy((heapIndex + index)->name, alias.name, len);
			heapIndex[index].name[len] = '\0';
		}

		return 1;
	} else {
		//Trying to write outside allocated memory DO CRASH!!!
	}
	return -1;
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

Alias_s Heap::getAlias(char *name) {
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
				alias.value[len] = '\0';
				alias.len = sizeof(int);
			}
			break;
		} else if (global.strCmp(heapIndexStructs[i].name, name)) { //lhs = String/pPair, rhs = string/String/pPair  
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

Alias_s Heap::getAlias(int index) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	int len = 0;
	len = strlen(heapIndex[index].name);
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

bool Heap::updateStructIndex(Index_s index) {
	for (int i = 0; i != indexStructLen; ++i) {
		if (global.strCmp(heapIndexStructs[i].name, index.name)) {
			heapIndexStructs[i].len = index.len;
			heapIndexStructs[i].startPos = index.startPos;
			int lenType = strlen(index.type);
			memcpy(&heapIndexStructs[i].type, index.type, lenType);
			heapIndexStructs[i].type[lenType] = '\0';
			
			int lenName = strlen(index.name);
			memcpy(&heapIndexStructs[i].name, index.name, lenName);
			heapIndexStructs[i].name[lenName] = '\0';

			//Change the len attribut for typedef type of struct.
			/*for (int k = 0; k != indexStructLen; ++k) {
				if (heapIndexStructs[k].startPos == index.startPos) {
				heapIndexStructs[k].len = index.len;

				lenType = strlen(index.type);
				memcpy(heapIndexStructs[k].type, index.type, lenType);
				heapIndexStructs[k].type[lenType] = '\0';
				break;
				}
			}*/
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

void Heap::initializeHeap(size_t size) {
	setHeapSize(size);
	heapStartPos = 0;
}

void Heap::createStack(size_t stackSize) {
	SetStackSize(stackSize);
	SetStackLen(0);

	if (stackSize < heapSize) {
		//Stack cannot be bigger then heap, DO CRASH!!
	}
	heapStartPos = heapSize - stackSize;
}

int Heap::pushTop(Alias_s alias) {
	int len = GetStackLen();
	SetStackLen(++len);
	return insertAliasAt(len, alias);
}

int Heap::pushAt(int index, Alias_s alias) {
	return insertAliasAt(index, alias);
}

Alias_s Heap::getTop() {
	return getAlias(GetStackLen());
}

Alias_s Heap::getAt(int index) {
	return getAlias(index);
}

void Heap::pop() {
	int len = GetStackLen();
	if (len > 0) {
		SetStackLen(--len);
	}
}