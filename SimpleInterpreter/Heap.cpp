#include "Heap.h"
#include "HelpHeaders.h"
#include "ErrorHeaders.h"

namespace Memory {
	Heap::Heap() {}

	Heap::~Heap() {
		if (heapContainer) {
			delete[] heapContainer;
			heapContainer = NULL;
		}

		if (heapIndex) {
			delete[] heapIndex;
			heapIndex = NULL;
		}
	}

	void Heap::SetHeapSize(const size_t size) {
		if (heapIndex) {
			Error::ErrorManager::ErrorCode(Error::CODE_15);
		}

		if (size <= 0) {
			Error::ErrorManager::ErrorCode(Error::CODE_14);
		}

		heapIndex = DBG_NEW Global::Index_s[size];
		heapContainer = DBG_NEW char[size];
		heapSize = size;

		ExpandHeapIndexStruct();
	}

	void Heap::InsertAliasAt(const int index, const Global::Alias_s alias) {
		if (alias.len > 0 && index >= 0 && index < GetStackSize()) {
			Error::ErrorManager::ErrorCode(Error::CODE_30);
		} else {
			InsertAt(index, alias);
		}
	}

	void Heap::InsertAt(const int index, const Global::Alias_s alias) {
		// Heap overflow
		if ((alias.len + index) <= heapSize) {
			int len = 0;
			//data type
			len = strlen(alias.type);
			memcpy(heapIndex[index].type, alias.type, len);
			heapIndex[index].type[len] = '\0';

			len = strlen(alias.value);
			(heapIndex + index)->len = len;

			(heapIndex + index)->startPos = index;

			//Insert value.
			if (len > 0) {
				if (Global::HelpClass::StrCmp((char *)alias.type, "string")) {
					memcpy(heapContainer + index, alias.value, len);

				} else if (Global::HelpClass::StrCmp(alias.type, "int")) {
					if (strlen(alias.value) > 9) {
						Error::ErrorManager::ErrorCode(Error::CODE_3511);
					}
					const int digits = atoi(alias.value);
					memcpy(heapContainer + index, (char *)&digits, sizeof(int));
				}
			}

			if (strlen(alias.name) > 0) {
				const int len = strlen(alias.name);
				memcpy((heapIndex + index)->name, alias.name, len);
				heapIndex[index].name[len] = '\0';
			}
		} else {
			//Trying to write outside allocated memory.
			Error::ErrorManager::ErrorCode(Error::CODE_10);
		}
	}

	//Specialcases for content inside struct.
	void Heap::InsertStructIndex(const Global::Index_s index) {
		if (indexStructLen >= indexStructMax) {
			ExpandHeapIndexStruct();
		}

		heapIndexStructs[indexStructLen] = index;
		++indexStructLen;
	}

	void Heap::ExpandHeapIndexStruct() {
		if (heapIndexStructs == NULL) {
			indexStructMax = 100;
			heapIndexStructs = DBG_NEW Global::Index_s[indexStructMax * sizeof(Global::Index_s)];
		} else {
			Global::Index_s *tmpArr = heapIndexStructs;

			indexStructMax *= 2;
			heapIndexStructs = DBG_NEW Global::Index_s[indexStructMax * sizeof(Global::Index_s)];
			memcpy(heapIndexStructs, tmpArr, indexStructLen * sizeof(Global::Index_s));

			delete[] tmpArr;
		}
	}

	int Heap::GetAddress(const char *name) {
		int ret = -1;
		int i = 0;
		while (i != heapSize) {
			if (Global::HelpClass::StrCmp(heapIndex[i].name, name)) {
				ret = heapIndex[i].startPos;
				break;
			} else if (Global::HelpClass::StrCmp(heapIndexStructs[i].name, name)) {
				ret = heapIndexStructs[i].startPos;
				break;
			}
			++i;
		}
		return ret;
	}

	Global::Alias_s Heap::GetAlias(const char *name) {
		Global::Alias_s alias = { "\0", "\0", "\0", 0 };
		int i = 0;
		int len = 0;
		while (i != heapSize) {
			if (Global::HelpClass::StrCmp(heapIndex[i].name, name)) {
				Global::Index_s *index = &heapIndex[i];
				len = strlen(heapIndex[i].name);
				memcpy(alias.name, heapIndex[i].name, len);
				alias.name[len] = '\0';

				alias.len = heapIndex[i].len;

				len = strlen(heapIndex[i].type);
				memcpy(alias.type, heapIndex[i].type, len);
				alias.type[len] = '\0';

				if (Global::HelpClass::StrCmp(alias.type, "string")) {
					memcpy(alias.value, &heapContainer[heapIndex[i].startPos], alias.len);
					alias.value[alias.len] = '\0';
				} else if (Global::HelpClass::StrCmp(alias.type, "int")) {
					//convert char to int and int to char
					int dest;
					memcpy(&dest, &heapContainer[i], sizeof(int));
					sprintf(alias.value, "%i", dest);

					len = Global::HelpClass::IntLength(dest);

					//is it negative
					if (Global::HelpClass::IsNegativeNumber(alias.value)) {
						len += 1;
					}

					alias.value[len] = '\0';
					alias.len = sizeof(int);
				}
				break;
			} else if (Global::HelpClass::StrCmp(heapIndexStructs[i].name, name)) {
				//structs
				len = strlen(heapIndexStructs[i].name);
				memcpy(alias.name, heapIndexStructs[i].name, len);
				alias.name[len] = '\0';

				alias.len = heapIndexStructs[i].len;

				len = strlen(heapIndexStructs[i].type);
				memcpy(alias.type, heapIndexStructs[i].type, len);
				alias.type[len] = '\0';

				if (Global::HelpClass::StrCmp(alias.type, "string")) {
					memcpy(alias.value, &heapContainer[heapIndexStructs[i].startPos], alias.len);
					alias.value[alias.len] = '\0';
				} else if (Global::HelpClass::StrCmp(alias.type, "int")) {
					//convert char to int and int to char
					int dest;
					memcpy(&dest, &heapContainer[heapIndexStructs[i].startPos], sizeof(int));
					sprintf(alias.value, "%i", dest);
					len = Global::HelpClass::IntLength(dest);
					alias.value[len] = '\0';
				} else if (!Global::HelpClass::StrCmp(alias.type, "offset")) {
					int k = 0;
					while (k != heapSize) {
						Global::Index_s s = heapIndexStructs[k];
						if (heapIndexStructs[k].startPos == heapIndexStructs[i].startPos) {
							len = strlen(heapIndexStructs[k].type);
							memcpy(alias.type, heapIndexStructs[k].type, len);
							alias.type[len] = '\0';

							if (Global::HelpClass::StrCmp(alias.type, "string")) {
								memcpy(alias.value, &heapContainer[heapIndexStructs[i].startPos], alias.len);
								alias.value[alias.len] = '\0';
							} else if (Global::HelpClass::StrCmp(alias.type, "int")) {
								//convert char to int and int to char
								int dest;
								memcpy(&dest, &heapContainer[heapIndexStructs[i].startPos], sizeof(int));
								sprintf(alias.value, "%i", dest);
								len = Global::HelpClass::IntLength(dest);
								alias.value[len] = '\0';
								alias.len = sizeof(int);
							}
							break;
						}
						++k;
					}
				}
				break;
			}
			++i;
		}
		return alias;
	}

	Global::Alias_s Heap::GetAlias(const int index) {
		Global::Alias_s alias = { NULL, NULL, NULL, 0 };
		int len = strlen(heapIndex[index].name);

		//Alias does not exist
		if (len > heapSize) {
			return alias;
		}

		memcpy(alias.name, heapIndex[index].name, len);
		alias.name[len] = '\0';

		alias.len = heapIndex[index].len;

		len = strlen(heapIndex[index].type);
		memcpy(alias.type, heapIndex[index].type, len);
		alias.type[len] = '\0';

		if (Global::HelpClass::StrCmp(alias.type, "string")) {
			memcpy(alias.value, &heapContainer[heapIndex[index].startPos], alias.len);
			alias.value[alias.len] = '\0';
		} else if (Global::HelpClass::StrCmp(alias.type, "int")) {
			//convert char to int and int to char
			int dest;
			memcpy(&dest, &heapContainer[index], sizeof(int));
			sprintf(alias.value, "%i", dest);
			//Is negative.
			if (dest < 0) {
				len = Global::HelpClass::IntLength(dest) +1;
			} else {
				len = Global::HelpClass::IntLength(dest);
			}

			alias.value[len] = '\0';
			alias.len = sizeof(int);
		}
		return alias;
	}

	const char Heap::GetValueAt(const int memoryAddress) {
		return heapContainer[memoryAddress];
	}

	Global::Index_s Heap::GetStructIndex(const char *name) {
		Global::Index_s index = { NULL, NULL, 0, 0 };
		for (int i = 0; i != indexStructLen; ++i) {
			if (Global::HelpClass::StrCmp(heapIndexStructs[i].name, name)) {
				index = heapIndexStructs[i];
				break;
			}
		}
		return index;
	}

	void Heap::GetStructIndex(const char *type, Global::Index_s *indexes, int &len) {
		int lenRetIndexes = 0;
		for (int i = 0; i != indexStructLen; ++i) {
			const char *name = heapIndexStructs[i].name;
			const char *slash = strstr(name, ".");
			if (slash) {
				int len = strlen(name) - strlen(slash);
				memcpy(tmpStr, name, len);
				tmpStr[len] = '\0';
				if (Global::HelpClass::StrCmp(tmpStr, type)) {
					indexes[lenRetIndexes] = heapIndexStructs[i];
					++lenRetIndexes;
				}
			}
		}
		len = lenRetIndexes;
	}

	void Heap::UpdateHeapIndex(const Global::Index_s index) {
		for (int i = 0; i != heapSize; ++i) {
			if (Global::HelpClass::StrCmp(heapIndex[i].name, index.name)) {
				heapIndex[i].len = index.len;
				heapIndex[i].startPos = index.startPos;
				int lenType = strlen(index.type);
				memcpy(heapIndex[i].type, index.type, lenType);
				heapIndex[i].type[lenType] = '\0';
				break;
			}
		}
	}

	bool Heap::UpdateStructIndex(const Global::Index_s index, const char *searchName) {
		for (int i = 0; i != indexStructLen; ++i) {
			if (Global::HelpClass::StrCmp(heapIndexStructs[i].name, searchName)) {
				heapIndexStructs[i].len = index.len;
				heapIndexStructs[i].startPos = index.startPos;
				const int lenType = strlen(index.type);
				memcpy(&heapIndexStructs[i].type, index.type, lenType);
				heapIndexStructs[i].type[lenType] = '\0';

				const int lenName = strlen(index.name);
				memcpy(&heapIndexStructs[i].name, index.name, lenName);
				heapIndexStructs[i].name[lenName] = '\0';
				return true;
			}
		}
		return false;
	}

	bool Heap::FindStructIndex(const Global::Index_s index) {
		for (int i = 0; i != indexStructLen; ++i) {
			if (Global::HelpClass::StrCmp(heapIndexStructs[i].name, index.name)) {
				return true;
			}
		}
		return false;
	}

	Global::Index_s Heap::FindStructIndex(const char *name) {
		Global::Index_s index = { NULL, NULL, NULL, 0 };
		for (int i = 0; i != indexStructLen; ++i) {
			if (Global::HelpClass::StrCmp(heapIndexStructs[i].name, name)) {
				return heapIndexStructs[i];
			}
		}
		return index;
	}

	void Heap::TypedefStructMembers(const char *searchName, const char *extendName) {
		Global::Index_s buffer[1024];
		int bufferLen = 0;
		int len1 = 0;
		int len2 = 0;
		for (int i = 0; i != indexStructLen; ++i) {
			Global::Index_s *s = &heapIndexStructs[i];
			const char *res = strstr(s->name, searchName);
			if (res) {
				char *findDot = strstr(s->name, ".");
				if (findDot) {
					len1 = strlen(searchName);
					memcpy(tmpStr, searchName, len1);
					memcpy(tmpStr + len1, ".", 1);
					len1 += 1;
					len2 = strlen(extendName);
					memcpy(tmpStr + len1, extendName, len2);
					memcpy(tmpStr + len1 + len2, ".", 1);
					len2 += len1;
					len1 = strlen(findDot);
					memcpy(tmpStr + len2, findDot, len1);
					tmpStr[len2 + len1] = '\0';
					len1 += len2;

					//Create a new struct member.
					Global::Index_s newIndex;
					memcpy(newIndex.name, tmpStr, len1);
					newIndex.name[len1] = '\0';

					newIndex.len = s->len;
					newIndex.startPos = s->startPos;

					len1 = strlen(s->type);
					memcpy(newIndex.type, s->type, len1);
					newIndex.type[len1] = '\0';
					buffer[bufferLen] = newIndex;
					++bufferLen;
				} else {
					//Syntax struct is wrong Do CRASH!!!
				}
			}
		}
		//Insert struct members into struct index array.
		for (int i = 0; i != bufferLen; ++i) {
			if (indexStructLen < indexStructMax) {
				heapIndexStructs[indexStructLen] = buffer[i];
				++indexStructLen;
			}
		}
	}

	void Heap::UpdateStructHeaderPointer(const Global::Index_s index) {
		char buffer[INSTRUCTIONSIZE];
		const bool res = Global::HelpClass::FindSubStrRev(buffer, index.name, ".");
		if (res) {
			int len = strlen(buffer) - 1;
			buffer[len] = '\0';
			for (int i = 0; i != indexStructLen; ++i) {
				Global::Index_s *currentIndex = &heapIndexStructs[i];
				if (Global::HelpClass::StrCmp(currentIndex->name, buffer) && currentIndex->type[0] == '\0') {
					currentIndex->startPos = index.startPos;

					len = strlen(index.type);
					memcpy(currentIndex->type, index.type, len);
					currentIndex->type[len] = '\0';

					currentIndex->len = index.len;
					break;
				}
			}
		}
	}

	char* Heap::GetFullNameStructMember(const char *lastname) {
		const bool andFound = Global::HelpClass::FindAnd((char *)lastname);

		char searchName[NAMESIZE];
		const int lenSearchName = strlen(lastname);
		memcpy(searchName, lastname, lenSearchName);
		searchName[lenSearchName] = '\0';

		Global::trimAnd(searchName);

		char buffer[NAMESIZE];
		int len = 0;
		int firstDot = 0;
		int lenFirstWord = 0;
		for (int i = indexStructLen -1; i >= 0; --i) {
			char *name = heapIndexStructs[i].name;
			int k = strlen(name);
			while (k != 0) {
				if (name[k] == '.') {
					if (firstDot == 0) {
						firstDot = strlen(searchName) - k;
						lenFirstWord = k;
					}
					len = strlen(name) - k - 1;
					memcpy(buffer, name + k + 1, len);
					buffer[len] = '\0';

					if (Global::HelpClass::StrCmp(buffer, searchName)) {
						if (andFound) {
							len = strlen(name);
							memcpy(buffer, "&", 1);
							memcpy(buffer + 1, name, len);
							buffer[len + 1] = '\0';
							return buffer;
						}
						return name;
					}
				}
				--k;
			}
		}
		return NULL;
	}

	void Heap::InitializeHeap(const size_t size) {
		SetHeapSize(size);
		heapStartPos = 0;
	}

	void Heap::CreateStack(const size_t size) {
		if (GetStackSize() > 0) {
			Error::ErrorManager::ErrorCode(Error::CODE_16);
		}

		if (size <= 0) {
			Error::ErrorManager::ErrorCode(Error::CODE_13);
		}

		SetStackSize(size);
		SetStackLen(0);

		if (GetStackSize() > heapSize) {
			//Stack cannot be bigger then heap.
			Error::ErrorManager::ErrorCode(Error::CODE_12);
		}
		heapStartPos = heapSize - GetStackSize();
	}

	void Heap::PushTop(const Global::Alias_s alias) {
		if (!IsStackOverflow(GetStackLen() + 1, alias.len)) {
			if (GetStackLen() > 0) {
				InsertAt(GetStackLen() + 1, alias);
			} else {
				InsertAt(GetStackLen(), alias);
			}
			SetStackLen(GetStackLen() + alias.len);
		}
	}

	void Heap::PushAt(const int index, const Global::Alias_s alias) {
		if (!IsStackOverflow(index, alias.len)) {
			InsertAt(index, alias);
		}
	}

	Global::Alias_s Heap::GetTop() {
		int len = GetStackLen();
		if (len > 0) { len -= 1; }
		Global::Alias_s alias = { NULL, NULL, NULL, 0 };
		if (!IsStackOverflow(len, 0)) {
			for (int i = len; i >= 0; --i) {
				if (heapIndex[i].startPos != len) {
					--len;
				} else {
					alias = GetAlias(i);
					break;
				}
			}
		}
		return alias;
	}

	Global::Alias_s Heap::GetAt(const int index) {
		Global::Alias_s alias = { NULL, NULL, NULL, 0 };
		if (!IsStackOverflow(index, 0)) {
			alias = GetAlias(index);
		}
		return alias;
	}

	void Heap::Pop() {
		if (GetStackLen() > 0) {
			SetStackLen(GetStackLen() - 1);
			Global::Alias_s alias = { NULL, NULL, NULL, 0 };
			int len = GetStackLen();
			for (int i = len; i >= 0; --i) {
				if (heapIndex[i].startPos != len) {
					--len;
				} else {
					alias = GetAlias(i);
					alias.len -= 1;
					alias.value[alias.len] = '\0';
					InsertAt(i, alias);
					break;
				}
			}
		}
	}

	void Heap::PopTop() {
		for (int i = GetStackLen(); i >= 0; --i) {
			if (heapIndex[i].startPos != GetStackLen()) {
				SetStackLen(GetStackLen() - 1);
			} else {
				break;
			}
		}
	}

	bool Heap::IsStackOverflow(const int index, const int len) {
		if (index + len >= (GetStackSize() + 2) || index < 0) {
			Error::ErrorManager::ErrorCode(Error::CODE_11);
		}
		return false;
	}
}