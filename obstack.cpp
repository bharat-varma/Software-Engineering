
/*

package iProlog;
import java.util.ArrayList;

class ObStack<T> extends ArrayList<T> {

  private static final long serialVersionUID = 1L;

  final T pop() {
    final int last = this.size() - 1;
    return this.remove(last);
  }

  final void push(final T O) {
    add(O);
  }

  final T peek() {
    return get(this.size() - 1);
  }
}
*/

#include <vector>
#include <iostream>

using namespace std;

namespace  iProlog
{
	template  <class T>
	class ObStack: public vector<T>
	{

	  private:
	  static const long serialVersionUID = 1L;

	  public:
	  T pop()
	  {
		int last = vector<T>::size() - 1;
		T value = *vector<T>::rbegin();
		vector<T>::pop_back();
		return value;
	  }

	  void push(T O)
	  {
		vector<T>::push_back(O);
	  }

	  T peek()
	  {
		return *vector<T>::rbegin();
	  }
	};
};

int main()
{
	iProlog::ObStack<int> a;
	a.push(1);
	a.push(2);
	a.push(3);
	cout << a.pop() << endl;
	cout << a.pop() << endl;
	cout << a.pop() << endl;

	return 0;
}
