#ifndef __INTMAP_H__
#define __INTMAP_H__

#include <stack>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

class IntMap  {

public:

  static const long serialVersionUID = 1L;

  static const int FREE_KEY = 0;

  static const int NO_VALUE = 0;

  /** Keys and values */
  vector<int> m_data;

  /** Do we have 'free' key in the map? */
  bool m_hasFreeKey;
  /** Value of 'free' key */
  int m_freeValue;

  /** Fill factor, must be between (0 and 1) */
  float m_fillFactor;
  /** We will resize a map once it reaches this size */
  int m_threshold;
  /** Current map size */
  int m_size;

  /** Mask to calculate the original position */
  int m_mask;
  int m_mask2;

  IntMap();

  IntMap(int size);

  IntMap(int size, float fillFactor);

  int get(int key);

  // for use as IntSet - Paul Tarau

  bool contains(int key);

  bool add(int key);

  bool del(int key);

  bool isEmpty();

  static void intersect0(IntMap &m, vector<IntMap> & maps, vector<IntMap> &  vmaps, stack<int> r);

  static stack<int> * intersect(vector<IntMap> & maps, vector<IntMap> & vmaps);

  // end changes

  int put(int key, int value);

  int remove(int key);

  int shiftKeys(int pos);

  int size();

  string show();

  void rehash(int newCapacity);

  /** Taken from FastUtil implementation */

  /** Return the least power of two greater than or equal to the specified value.
   *
   * <p>Note that this function will return 1 when the argument is 0.
   *
   * @param x a long integer smaller than or equal to 2<sup>62</sup>.
   * @return the least power of two greater than or equal to the specified value.
   */
   static long nextPowerOfTwo(long x);

  /** Returns the least power of two smaller than or equal to 2<sup>30</sup>
   * and larger than or equal to <code>Math.ceil( expected / f )</code>.
   *
   * @param expected the expected number of elements in a hash table.
   * @param f the load factor.
   * @return the minimum possible size for a backing array.
   * @throws IllegalArgumentException if the necessary size is larger than 2<sup>30</sup>.
   */
  static int arraySize(int expected, float f);

  //taken from FastUtil
  static const int INT_PHI = 0x9E3779B9;

  int static phiMix(int x); 

};

#endif
