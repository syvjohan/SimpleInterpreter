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
	char *cStr = NULL;
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

void freeHeap(void) {
	free(heap.container);
}

