#include "IntStack.h"

#pragma warning(disable: 4996)

	IntStack::IntStack() {
    init(SIZE);
  }

	IntStack::IntStack(int size) {
		init(size);
	}

	void IntStack::init(int size)
	{
		stack.resize(size);
		clear();
	}
	
	int IntStack::getTop() {
    return top;
  }

   int IntStack::setTop( int top) {
    return this->top = top;
  }

   void IntStack::clear() {
    top = -1;
  }

   bool IntStack::isEmpty() {
    return top < 0;
  }

  /**
   * Pushes an element - top is incremented first than the
   * element is assigned. This means top point to the last assigned
   * element - which can be returned with peek().
   */
  void IntStack::push(int i) {
    // IO.dump("push:"+i);
    if (++top >= stack.size()) {
      expand();
    }
    stack[top] = i;
  }

  int IntStack::pop() {
    int r = stack[top--];
    shrink();
    return r;
  }

  int IntStack::get(int i) {
    return stack[i];
  }

  void IntStack::set(int i, int val) {
    stack[i] = val;
  }

  int IntStack::size() {
    return top + 1;
  }


  /**
   * dynamic array operation: doubles when full
   */
    void IntStack::expand() {
     int l = stack.size();
	 vector<int> newstack(l << 1);
	 for (int i = 0; i < l; i++)
	 {
		 newstack[i] = stack[i];
	 }
	 stack = newstack;
  }

  /**
  * dynamic array operation: shrinks to 1/2 if more than than 3/4 empty
  */
   void IntStack::shrink() {
    int l = stack.size();
    if (l <= MINSIZE || top << 2 >= l)
      return;
    l = 1 + (top << 1); // still means shrink to at 1/2 or less of the heap
    if (top < MINSIZE) {
      l = MINSIZE;
    }

	vector<int> newstack(l);
	for (int i = 0; i < top + 1; i++)
	{
		newstack[i] = stack[i];
	}
    //System.arraycopy(stack, 0, newstack, 0, top + 1);
    stack = newstack;
  }

  vector<int> IntStack::toArray() {
	  vector <int> array(size());
    if (size() > 0) {
		for (int i = 0; i < size(); i++)
			array[i] = stack[i];
    }
    return array;
  }

  void IntStack::reverse() {
    int l = size();
    int h = l >> 1;
    for (int i = 0; i < h; i++) {
      int temp = stack[i];
      stack[i] = stack[l - i - 1];
      stack[l - i - 1] = temp;
    }
  }

   string IntStack::toString() {
	  vector<int> v = toArray();
	  string s = "{";
	  for (int i = 0; i < v.size(); i++)
	  {
		  if (i > 0)
		  {
			  s += ", ";
		  }
		  char tmp[10];
		  sprintf(tmp, "%d", v[i]);
		  s += tmp;
	  }
	  s += "}";
	  return s;
    //return Arrays.toString(toArray());
  }

