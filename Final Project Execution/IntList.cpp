#include "IntList.h"
	
IntList::IntList(int head) {
    this->_head = head;
    _tail = NULL;
  }

IntList::IntList(int X, IntList * Xs) {
    _head = X;
    _tail = Xs;
  }

	 bool IntList::isEmpty(IntList *Xs) {
    return NULL == Xs;
  }

	 int IntList::head(IntList *Xs) {
    return Xs->_head;
  }

   IntList * IntList::tail(IntList * Xs) {
    return Xs->_tail;
  }

   IntList * IntList::cons(int X, IntList *Xs) {
    return new IntList(X, Xs);
  }

   IntList *IntList::app(vector<int> xs, IntList * Ys) {
    IntList * Zs = Ys;
    for (int i = xs.size() - 1; i >= 0; i--) {
      Zs = cons(xs[i], Zs);
    }
    return Zs;
  }

   IntStack * IntList::toInts(IntList * Xs) {
    IntStack *is = new IntStack();
    while (!isEmpty(Xs)) {
      is->push(head(Xs));
      Xs = tail(Xs);
    }
    return is;
  }

   int IntList::len(IntList *Xs) {
	   IntStack * is = toInts(Xs);
	   int r = is->size();
	   delete is;
	   return r;
//    return toInts(Xs).size();
  }

  string IntList::toString() {
	  IntStack * is = toInts(this);
	  string s = is->toString();
	  delete is;
	  return s;
//    return toInts(this).toString();
  }

