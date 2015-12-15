#include "Heap.h"

Heap::Heap() {}

Heap::~Heap() {
	if (heapContainer) {
		delete [] heapContainer;
		heapContainer = NULL;
	}

	if (heapIndex) {
		delete [] heapIndex;
		heapIndex = NULL;
	}
}

void Heap::setHeapSize(size_t size) {
	heapIndex = DBG_NEW Index_s [size];
	heapContainer = DBG_NEW char[size];
	heapSize = size;
}

int Heap::insertAliasAt(int index, Alias_s alias) {
	//data type
	heapIndex[index].type = alias.type;

	//Insert value.
	if (alias.value != NULL) {
		if (strCmp((char *)alias.type, "string")) {
			int k = 0;
			int i;
			int len = strlen(alias.value);

			if (index + stackLen + len > heapStartPos) {
				//StackOverfloaw. DO CRASH!!
			} else if (index + len > heapSize) {
				//HeapOverflow. DO CRASH!!
			}

			for (i = index; i != heapSize && i < len; ++i) {
				heapContainer[i] = alias.value[k];
				++k;
			}
			heapContainer[i] = '\0';

			heapIndex->len = len;
			heapIndex->startPos = index;
			heapIndex->type = "string";

		} else if (strCmp((char *)alias.type, "int")) {
			int digits = atoi(alias.value);
			if (index + stackLen + sizeof(digits) > heapStartPos) {
				//StackOverfloaw. DO CRASH!!
			} else if (index + sizeof(digits) > heapSize) {
				//HeapOverflow. DO CRASH!!
			}

			memcpy(heapContainer + index, (char *)&digits, sizeof(int));
			heapIndex->len = sizeof(int);
			heapIndex->startPos = index;
			heapIndex->type = "int";
		}
	}
	
	heapIndex[index].name = alias.name;
	return 1;
}

Alias_s Heap::getAlias(char *name) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	int i = 0;
	while (i != heapSize) {
		if (heapIndex[i].name == name) {
			alias.name = heapIndex[i].name;
			alias.len = heapIndex[i].len;
			alias.type = heapIndex[i].type;
			if (alias.type == "string") {
				memcpy(&alias.value, &heapContainer[heapIndex[i].startPos], alias.len);
			} else if (alias.type == "int") {
				//convert char to int and int to char
				int dest;
				memcpy(&dest, &heapContainer[i], sizeof(int));
				sprintf(str, "%d", dest);
				alias.value = str;
			}
			break;
		}
		++i;
	}
	return alias;
}

Alias_s Heap::getAlias(int index) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	if (stackLen != -1) {
		alias.name = heapIndex[stackLen].name;
		alias.len = heapIndex[stackLen].len;
		alias.type = heapIndex[stackLen].type;
		if (alias.type == "string") {
			memcpy(&alias.value, &heapContainer[heapIndex->startPos], alias.len);
		} else if (alias.type == "int") {
			//convert char to int and int to char
			int dest;
			memcpy(&dest, &heapContainer[stackLen], sizeof(int));
			sprintf(str, "%d", dest);
			alias.value = str;
		}
	}
	return alias;
}

void Heap::initializeHeap(size_t size) {
	setHeapSize(size);
	heapStartPos = 0;

	int i;
	for (i = 0; i != heapSize; ++i) {
		heapIndex[i].len = 0;
		heapIndex[i].startPos = 0;
	}
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
	--stackLen;
}