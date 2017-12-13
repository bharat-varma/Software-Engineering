#include "ObStack.h"
#include "Spine.h"

template <class T>
T ObStack<T>::pop()
	{
		T x = v[last];
		last--;
		v.pop_back();
		return x;
	}


template <class T>
ObStack<T>::ObStack()
	{
		last = -1;
	}

	template <class T>
	void ObStack<T>::push(T O)
	{
		v.push_back(O);
		last++;
	}

	template <class T>
	T ObStack<T>::get(int index)
	{
		return v[index];
	}

	template <class T>
	T ObStack<T>::peek()
	{
		return v[last];
	}

	template <class T>
	bool ObStack<T>::isEmpty()
	{
		return v.size() == 0;
	}

template class ObStack<Spine*>;
