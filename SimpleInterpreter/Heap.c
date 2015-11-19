#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define NAMESIZE 30

typedef struct index_s {
	char name[NAMESIZE];
	int startPos;
	int endPos;
}index_t;

char *heapContainer;
index_t *heapIndex;
size_t heapSize;

char *tmp;

void setHeapSize(size_t size) {
	heapIndex = (index_t *)malloc(size * sizeof(index_t));
	heapContainer = malloc(size * sizeof(char));
	heapSize = size;
}

size_t getHeapSize(void) {
	return heapSize;
}

void insertAt(int index, char *value, char *name) {
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

char* getName(int index) {
	return heapIndex[index].name;
}

char* getValueAt(int position) {
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

void initializeHeap(size_t size) {
	setHeapSize(size);

	int i;
	for (i = 0; i != heapSize; ++i) {
		heapIndex[i].endPos = 0;
		heapIndex[i].startPos = 0;
	}
}

//Returns name index.
int getIndexAsInt(char *name) {
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
char* getIndexAsString(char *name) {
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

char* getValue(int index) {
	int len = heapIndex[index].endPos - heapIndex[index].startPos;
	tmp = (char *)malloc(len +1 * sizeof(char));

	memcpy(tmp, heapContainer + index, len);
	tmp[len] = '\0';
	return tmp;
}

void freeTmpValue() {
	if (tmp) {
		free(tmp);
		tmp = NULL;
	}
}

void freeHeap(void) {
	if (heapContainer) {
		free(heapContainer);
		heapContainer = NULL;
	}

	if (heapIndex) {
		free(heapIndex);
		heapIndex = NULL;
	}
}