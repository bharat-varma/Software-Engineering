#ifndef __INTLIST_H__
#define __INTLIST_H__

#include <string>
#include <vector>
#include "IntStack.h"

using namespace std;

class IntList {

private:

	int _head;
	IntList * _tail;
	
	IntList(int head);
	IntList(int X, IntList * Xs);

public:

	static bool isEmpty(IntList *Xs);
	static int head(IntList *Xs);
	static IntList * tail(IntList * Xs);
	static IntList * cons(int X, IntList *Xs);
	static IntList * app(vector<int> xs, IntList * Ys);
	static IntStack * toInts(IntList * Xs);
	static int len(IntList *Xs);
	string toString();
};

#endif
