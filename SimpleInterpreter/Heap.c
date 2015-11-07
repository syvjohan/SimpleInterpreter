#include <stdio.h>
#include <stdlib.h>

typedef struct object_s {
	char *name;
}object_t;

object_t obj[10];

typedef struct heap_s {
	char *container;
	size_t size;
}heap_t;

heap_t heap;

void setHeapSize(size_t size) {
	heap.size = size;
	heap.container = (char *)malloc(size * sizeof(char));
}

size_t getHeapSize(void) {
	return heap.size;
}

void insertAt(int index, char *cStr) {
	heap.container[index] = cStr;
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

void Init(size_t heapSize) {
	setHeapSize(heapSize);
}

void freeHeap(void) {
	heap.container;
}

