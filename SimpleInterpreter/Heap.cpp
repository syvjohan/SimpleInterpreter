#include "Heap.h"

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
	heapIndex = DBG_NEW Index_s [size];
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
			if (strCmp((char *)alias.type, "string")) {

				if (index + stackLen + len > heapStartPos) {
					//StackOverfloaw. DO CRASH!!
				} else if (index + len > heapSize) {
					//HeapOverflow. DO CRASH!!
				}

				memcpy(heapContainer + index, alias.value, len);

			} else if (strCmp(alias.type, "int")) {
				int digits = atoi(alias.value);
				if (stackLen > -1) {
					if (index + stackLen + sizeof(digits) > heapStartPos) {
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
		if (strCmp(heapIndex[i].name, name)) {
			ret = heapIndex[i].startPos;
			break;
		} else if (strCmp(heapIndexStructs[i].name, name)) {
			ret = heapIndexStructs[i].startPos;
			break;
		}
		++i;
	}
	return ret;
}

Alias_s Heap::getAlias(char *name) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	int i = 0;
	int len = 0;
	while (i != heapSize) {
		if (strCmp(heapIndex[i].name, name)) {
			len = strlen(heapIndex[i].name);
			memcpy(alias.name, heapIndex[i].name, len);
			alias.name[len] = '\0';

			alias.len = heapIndex[i].len;

			len = strlen(heapIndex[i].type);
			memcpy(alias.type, heapIndex[i].type, len);
			alias.type[len] = '\0';

			if (strCmp(alias.type, "string")) {
				memcpy(alias.value, &heapContainer[heapIndex[i].startPos], alias.len);
				alias.value[alias.len] = '\0';
			} else if (strCmp(alias.type, "int")) {
				//convert char to int and int to char
				int dest;
				memcpy(&dest, &heapContainer[i], sizeof(int));
				sprintf(alias.value, "%i", dest);
				alias.value[alias.len] = '\0';
			}
			break;
		} else if (strCmp(heapIndexStructs[i].name, name)) {
			//structs
			len = strlen(heapIndexStructs[i].name);
			memcpy(alias.name, heapIndexStructs[i].name, len);
			alias.name[len] = '\0';

			alias.len = heapIndexStructs[i].len;

			len = strlen(heapIndexStructs[i].type);
			memcpy(alias.type, heapIndexStructs[i].type, len);
			alias.type[len] = '\0';

			if (strCmp(alias.type, "string")) {
				memcpy(alias.value, &heapContainer[heapIndexStructs[i].startPos], alias.len);
				alias.value[alias.len] = '\0';
			} else if (strCmp(alias.type, "int")) {
				//convert char to int and int to char
				int dest;
				memcpy(&dest, &heapContainer[i], sizeof(int));
				sprintf(alias.value, "%i", dest);
				alias.value[alias.len] = '\0';
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

	if (strCmp(alias.type, "string")) {
		memcpy(alias.value, &heapContainer[heapIndex[index].startPos], alias.len);
		alias.value[alias.len] = '\0';
	} else if (strCmp(alias.type, "int")) {
		//convert char to int and int to char
		int dest;
		memcpy(&dest, &heapContainer[index], sizeof(int));
		sprintf(alias.value, "%i", dest);
		alias.value[alias.len] = '\0';
	}
	return alias;
}

Index_s Heap::getStructIndex(char *name) {
	Index_s index = { NULL, NULL, 0, 0 };
	for (int i = 0; i != indexStructLen; ++i) {
		if (strCmp(heapIndexStructs[i].name, name)) {
			index = heapIndexStructs[i];
			break;
		}
	}
	return index;
}

void Heap::updateStructIndex(Index_s index) {
	for (int i = 0; i != indexStructLen; ++i) {
		if (strCmp(heapIndexStructs[i].name, index.name)) {
			heapIndexStructs[i].len = index.len;
			heapIndexStructs[i].startPos = index.startPos;
			memcpy(heapIndexStructs[i].type, index.type, strlen(index.type));
			break;
		}
	}
}

void Heap::initializeHeap(size_t size) {
	setHeapSize(size);
	heapStartPos = 0;
}

void Heap::createStack(size_t stackSize) {
	this->stackSize = stackSize;
	this->stackLen = 0;

	if (stackSize < heapSize) {
		//Stack cannot be bigger then heap, DO CRASH!!
	}
	heapStartPos = heapSize - stackSize;
}

int Heap::pushTop(Alias_s alias) {
	return insertAliasAt(stackLen, alias);
	++stackLen;
}

int Heap::pushAt(int index, Alias_s alias) {
	return insertAliasAt(index, alias);
}

Alias_s Heap::getTop() {
	return getAlias(stackLen);
}

Alias_s Heap::getAt(int index) {
	return getAlias(index);
}

void Heap::pop() {
	if (stackLen > 0) {
		--stackLen;
	}
}