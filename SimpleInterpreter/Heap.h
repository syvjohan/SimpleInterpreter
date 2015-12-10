#ifndef HEAP_H
#define HEAP_H

#include "Defs.h"
#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define NAMESIZE 30

class Heap {
	public:
		Heap();
		~Heap();

		void initializeHeap(size_t heapSize);
		void insertAt(int index, char *value, char *name);
		char* getValueAt(int position);

		int getIndexAsInt(char *name);
		char* getIndexAsString(char *name);
		char* getValue(int index);
		char* getName(int index);

		size_t getHeapSize(void);

		void freeTmpValue();

	private:
		void setHeapSize(size_t size);

		struct Index_s {
			char name[NAMESIZE];
			int startPos;
			int endPos;
		};

		char *heapContainer;
		Index_s *heapIndex;
		size_t heapSize;

		char *tmp;
};

#endif //!HEAP_H