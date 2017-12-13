#ifndef __INTSTACK_H__
#define __INTSTACK_H__

#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

class IntStack {

private:

	vector<int> stack;
	int top;
	int SIZE = 16;
	int MINSIZE = 1 << 15;

public:

	IntStack();
	IntStack(int size);
	void init(int size);
	int getTop();
	int setTop(int top);
	void clear();
	bool isEmpty();
	void push(int i);
	int pop();
	int get(int i);
	void set(int i, int val);
	int size();

//private:
  /**
   * dynamic array operation: doubles when full
   */
	void expand();
	void shrink();
	vector<int> toArray();
	void reverse();
	string toString();

};

#endif
