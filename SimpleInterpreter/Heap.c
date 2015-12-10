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

size_t Heap::getHeapSize(void) {
	return heapSize;
}

void Heap::insertAt(int index, char *value, char *name) {
	if (heapIndex[index].endPos + strlen(value) > heapSize) {
		printf("Overflowing the heap, do CRASH!!!\n");
		printf("\n endposition: %i ", heapIndex[index].endPos);
		//Overflowing the heap, do CRASH!!!
		Sleep(5000);
	}

	int k = 0;
	int i;
	for (i = index; i != heapSize && value[k] != '\0'; ++i) {
		heapContainer[i] = value[k];
		++k;
	}
	heapContainer[i] = '\0';

	//Index.
	int len = strlen(value);
	if (name == "") {
		heapIndex[index].endPos += strlen(value);
	} else {
		heapIndex[index].startPos = index;
		heapIndex[index].endPos = index + len;
	}
	
	int j;
	for (j = 0; j != NAMESIZE && name[j] != '\0'; ++j) {
		heapIndex[index].name[j] = name[j];
	}
	heapIndex[index].name[j] = '\0';
}

char* Heap::getName(int index) {
	return heapIndex[index].name;
}

char* Heap::getValueAt(int position) {
	int i = 0;
	char cStr[50];
	while (heapContainer[position] != '\0') {
		cStr[i] = heapContainer[position];

		++position;
		++i;
	}
	cStr[i] = '\0';

	return cStr;
}

void Heap::initializeHeap(size_t size) {
	setHeapSize(size);

	int i;
	for (i = 0; i != heapSize; ++i) {
		heapIndex[i].endPos = 0;
		heapIndex[i].startPos = 0;
	}
}

//Returns name index.
int Heap::getIndexAsInt(char *name) {
	int i = 0;
	while (heapSize != i) {
		if (strCmp(name, heapIndex[i].name)) {
			return heapIndex[i].startPos;
		}
		++i;
	}
	return -1;
}

//Returns name index.
char* Heap::getIndexAsString(char *name) {
	int i = 0;
	while (heapSize != i) {
		if (strCmp(name, heapIndex[i].name)) {
			static char buffer[20];
			sprintf(buffer, "%d", heapIndex[i].startPos);
			return buffer;
		}
		++i;
	}
	return "";
}

char* Heap::getValue(int index) {
	int len = heapIndex[index].endPos - heapIndex[index].startPos;
	tmp = (char *)malloc(len +1 * sizeof(char));

	memcpy(tmp, heapContainer + index, len);
	tmp[len] = '\0';
	return tmp;
}

void Heap::freeTmpValue() {
	if (tmp) {
		free(tmp);
		tmp = NULL;
	}
}