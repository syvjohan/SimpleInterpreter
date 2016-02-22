#pragma once

struct Alias_s;

class IStack {
	public:
		IStack() {};
		virtual ~IStack() {};

		virtual void CreateStack(const size_t stackSize) = 0;
		virtual void PushTop(const Alias_s alias) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual void PushAt(const int index, const Alias_s alias) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual Alias_s GetTop() = 0;
		virtual Alias_s GetAt(const int index) = 0;
		virtual void PopTop() = 0;
		virtual void Pop() = 0;

		int GetStackSize() { return stackSize; };
		void SetStackSize(int size) { stackSize = size; };
		int GetStackLen() { return stackLen; };
		void SetStackLen(int len) { stackLen = len; };

	private:
		int stackSize = -1;
		int stackLen = -1;
};
