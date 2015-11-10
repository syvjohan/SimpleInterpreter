#include <stdio.h>
#include <stdlib.h>

#define NAMESIZE 30

typedef struct index_s {
	char name[NAMESIZE];
	int startPos;
	int endPos;
}index_t;

typedef struct heap_s {
	char *container;
	index_t *index;
	size_t size;
}heap_t;

heap_t heap;

char *tmp;

void setHeapSize(size_t size) {
	heap.container = malloc(size * sizeof(char));
	heap.index = (index_t *)malloc(size * sizeof(index_t));
	heap.size = size;
}

size_t getHeapSize(void) {
	return heap.size;
}

void insertAt(int index, char *value, char *name) {
	int k = 0;
	int i;
	for (i = index; i != heap.size && value[k] != '\0'; ++i) {
		heap.container[i] = value[k];
		++k;
	}
	heap.container[i] = '\0';

	//Index.
	heap.index->startPos = index;
	int len = strlen(value);
	heap.index->endPos = index + len;

	int j;
	for (j = 0; j != NAMESIZE && name[j] != '\0'; ++j) {
		heap.index->name[j] = name[j];
	}
	heap.index->name[j] = '\0';
}

char* getValueAt(int position) {
	int i = 0;
	static char *cStr = NULL;
	while (heap.container[position] != '\0') {
		cStr[i] = heap.container[position];

		++position;
		++i;
	}

	return cStr;
}

void initializeHeap(size_t heapSize) {
	setHeapSize(heapSize);
}

//Returns name index.
int getIndexAsInt(char *name) {
	int i = 0;
	while (heap.size != i) {
		if (strCmp(name, heap.index->name)) {
			return heap.index->startPos;
		}
		++i;
	}
	return -1;
}

//Returns name index.
char* getIndexAsString(char *name) {
	int i = 0;
	while (heap.size != i) {
		if (strCmp(name, heap.index->name)) {
			static char buffer[20];
			sprintf(buffer, "%d", heap.index->startPos);
			return buffer;
		}
		++i;
	}
	return -1;
}

char* getValue(int index) {
	int len = heap.index->endPos - heap.index->startPos;
	tmp = (char *)malloc(len +1 * sizeof(char));

	memcpy(tmp, heap.container + index, len);
	tmp[len] = '\0';
	return tmp;
}

void freeTmpValue() {
	free(tmp);
	tmp = NULL;
}

void freeHeap(void) {
	free(heap.container);
	heap.container = NULL;
}

