#ifndef HEAP_H
#define HEAP_H

void initializeHeap(size_t heapSize);
void insertAt(int index, char *value, char *name);
char* getValueAt(int position);

#endif //!HEAP_H