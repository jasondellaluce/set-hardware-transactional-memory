#ifndef SET_H
#define SET_H

#define ulong unsigned long

class Set {
	public:
		virtual bool add(int item) = 0;
		virtual bool remove(int item) = 0;
		virtual bool contains(int item) = 0;
		virtual void print() = 0;
};

#endif