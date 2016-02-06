#pragma once

struct Alias_s;

class IStack {
	public:
	IStack() {};
	virtual ~IStack() {};

		virtual void createStack(size_t stackSize) = 0;
		virtual int pushTop(Alias_s alias) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual int pushAt(int index, Alias_s alias) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual Alias_s getTop() = 0;
		virtual Alias_s getAt(int index) = 0;
		virtual void pop() = 0;

		int GetStackSize() { return stackSize; };
		void SetStackSize(int size) { stackSize = size; };
		int GetStackLen() { return stackLen; };
		void SetStackLen(int len) { stackLen = len; };

	private:
		int stackSize = -1;
		int stackLen = -1;
};