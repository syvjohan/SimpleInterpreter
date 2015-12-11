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

void Heap::insertAliasAt(int index, Alias_s alias) {
	if (heapIndex[index].len + strlen(alias.value) > heapSize) {
		printf("Overflowing the heap, do CRASH!!!\n");
		printf("\n endposition: %i ", heapIndex[index].len);
		//Overflowing the heap, do CRASH!!!
		Sleep(5000);
	}

	//data type
	heapIndex[index].type = alias.type;

	//Insert value.
	if (alias.value != NULL) {
		if (strCmp((char *)alias.type, "string")) {
			int k = 0;
			int i;
			for (i = index; i != heapSize && alias.value[k] != '\0'; ++i) {
				heapContainer[i] = alias.value[k];
				++k;
			}
			heapContainer[i] = '\0';

		} else if (strCmp((char *)alias.type, "int")) {
			int dest = atoi(alias.value);
			memcpy(heapContainer +index, &dest, sizeof(int));
		}
	}
	
	//Index.
	int len = strlen(alias.value);
	if (alias.name == "") {
		heapIndex[index].len += strlen(alias.value);
	} else {
		heapIndex[index].startPos = index;
		heapIndex[index].len = index + len;
	}
	
	heapIndex[index].name = alias.name;
}

void Heap::initializeHeap(size_t size) {
	setHeapSize(size);

	int i;
	for (i = 0; i != heapSize; ++i) {
		heapIndex[i].len = 0;
		heapIndex[i].startPos = 0;
	}
}

Alias_s Heap::getAlias(char *name) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	int i = 0;
	while (i != heapSize) {
		if (heapIndex[i].name == name) {
			alias.name = heapIndex[i].name;
			alias.type = heapIndex[i].type;
			alias.len = heapIndex[i].len;
			memcpy(&alias.value, &heapContainer[heapIndex[i].startPos], alias.len);
			break;
		}
	}
	return alias;
}

void Heap::initializeStack(size_t stackSize) {
	this->stackSize = stackSize;
	this->stackLen = 0;
}

int Heap::push(Index_s index, char *value) {
	if (index.len + stackLen > stackSize) { return 0; }

	heapIndex[stackLen].name = index.name;
	heapIndex[stackLen].type = index.type;
	
	heapIndex[stackLen].startPos = stackLen;
	stackLen += index.len;

	int i = heapIndex[stackLen].startPos;
	int end = i += strlen(value);
	int j = 0;
	while (i != index.len) {
		heapContainer[i] = value[j];
	}

	return 1;
}

int Heap::pushAt(Index_s index, char *value) {
	if (index.len + stackLen > stackSize) { return 0; }

	heapIndex[index.startPos].name = index.name;
	heapIndex[index.startPos].type = index.type;

	heapIndex[index.startPos].startPos = stackLen;
	stackLen += index.len;

	int i = heapIndex[index.startPos].startPos;
	int end = i += strlen(value);
	int j = 0;
	while (i != index.len) {
		heapContainer[i] = value[j];
	}

	return 1;
}

Alias_s Heap::getTop() {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	if (stackLen != 0) {
		alias.name = heapIndex[stackLen].name;
		alias.len = heapIndex[stackLen].len;
		alias.type = heapIndex[stackLen].type;
		memcpy(&alias.value, &heapContainer[heapIndex->startPos], alias.len);
	}
	return alias;
}

Alias_s Heap::getAt(int index) {
	Alias_s alias = { NULL, NULL, NULL, 0 };
	if (stackLen != 0) {
		if (index >= 0 && index <= stackSize) {
			alias.name = heapIndex[index].name;
			alias.len = heapIndex[index].len;
			alias.type = heapIndex[index].type;
			memcpy(&alias.value, &heapContainer[heapIndex->startPos], alias.len);
		}
	}
	return alias;
}

void Heap::pop() {
	--stackLen;
}