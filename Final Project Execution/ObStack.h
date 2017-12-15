#ifndef __OBSTACK_H__
#define __OBSTACK_H__

#include <vector>

using namespace std;

template <class T>
class ObStack
{
	int last;
	vector<T> v;
public:

	T pop();


	ObStack();

	void push(T O);

	bool isEmpty();

	T get(int index);


	T peek();
};

#endif
