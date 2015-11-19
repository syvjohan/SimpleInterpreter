#ifndef HEAP_H
#define HEAP_H

void initializeHeap(size_t heapSize);
void insertAt(int index, char *value, char *name);
char* getValueAt(int position);

int getIndexAsInt(char *name);
char* getIndexAsString(char *name);
char* getValue(int index);
char* getName(int index);

size_t getHeapSize(void);

void freeHeap();
void freeTmpValue();

#endif //!HEAP_H