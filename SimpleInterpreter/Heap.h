#ifndef HEAP_H
#define HEAP_H

#include "IStack.h"
#include "Defines.h"
#include "Global.h"
#include "ErrorManager.h"

struct Alias_s;
struct Index_s;

class Heap : public IStack {
	public:
		Heap();
		~Heap();

		//Heap
		void InitializeHeap(size_t heapSize);
		void InsertAliasAt(int index, Alias_s alias);
		Alias_s GetAlias(const char *name);
		Alias_s GetAlias(const int index);
		int GetAddress(char *name);
		void UpdateHeapIndex(Index_s index);

		//Stack
		void CreateStack(size_t stackSize); 
		void PushTop(Alias_s alias);
		void PushAt(int index, Alias_s alias);
		Alias_s GetTop();
		Alias_s GetAt(const int index);
		void PopTop();
		void Pop();
		bool IsStackOverflow(int index, int len);

		//Index
		//Below functions is specialcases for content inside struct
		void InsertStructIndex(Index_s index);
		bool UpdateStructIndex(Index_s index, char *searchName);
		bool FindStructIndex(Index_s index);
		Index_s FindStructIndex(char *name);
		Index_s GetStructIndex(const char *name);
		void GetStructIndex(const char *type, Index_s *index, int &len);
		void TypedefStructMembers(char *searchName, char *extendName);
		void UpdateStructHeaderPointer(Index_s index);
		char* GetFullNameStructMember(char *lastname);

	private:
		void SetHeapSize(size_t size);
		void InsertAt(int index, Alias_s alias);

		char *heapContainer;
		Index_s *heapIndex;
		size_t heapStartPos;
		size_t heapSize;
		
		char tmpStr[INSTRUCTIONSIZE];

		//Index
		//Specialcases for content inside struct
		void ExpandHeapIndexStruct();
		Index_s *heapIndexStructs;
		size_t indexStructLen = 0;
		size_t indexStructMax = 0;

		Global global;
		ErrorManager errorManager;
};

#endif //!HEAP_H