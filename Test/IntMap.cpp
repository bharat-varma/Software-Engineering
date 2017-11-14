#include "IntMap.h"


IntMap::IntMap():IntMap(1<<2) {
    //this(1 << 2);
  }

  IntMap::IntMap(int size):IntMap(size, 0.75f) {
    //this(size, 0.75f);
  }

  IntMap::IntMap(int size, float fillFactor) {
    //if (fillFactor <= 0 || fillFactor >= 1)
    //  throw new IllegalArgumentException("FillFactor must be in (0, 1)");
    //if (size <= 0)
    //  throw new IllegalArgumentException("Size must be positive!");
    int capacity = arraySize(size, fillFactor);
    m_mask = capacity - 1;
    m_mask2 = capacity * 2 - 1;
    m_fillFactor = fillFactor;

    m_data.resize(capacity * 2);
    m_threshold = (int) (capacity * fillFactor);
  }

  int IntMap::get(int key) {
    int ptr = (phiMix(key) & m_mask) << 1;

    if (key == FREE_KEY)
      return m_hasFreeKey ? m_freeValue : NO_VALUE;

    int k = m_data[ptr];

    if (k == FREE_KEY)
      return NO_VALUE; //end of chain already
    if (k == key) //we check FREE prior to this call
      return m_data[ptr + 1];

    while (true) {
      ptr = ptr + 2 & m_mask2; //that's next index
      k = m_data[ptr];
      if (k == FREE_KEY)
        return NO_VALUE;
      if (k == key)
        return m_data[ptr + 1];
    }
  }

  // for use as IntSet - Paul Tarau

  bool IntMap::contains(int key) {
    return NO_VALUE != get(key);
  }

  bool IntMap::add(int key) {
    return NO_VALUE != put(key, 666);
  }

  bool IntMap::del(int key) {
    return NO_VALUE != remove(key);
  }

  bool IntMap::isEmpty() {
    return 0 == m_size;
  }

  void IntMap::intersect0(IntMap &m, vector<IntMap> & maps, vector<IntMap> &  vmaps, stack<int> r) {
    vector<int> & data = m.m_data;
    for (int k = 0; k < data.size(); k += 2) {
      bool found = true;
      int key = data[k];
      if (FREE_KEY == key) {
        continue;
      }
      for (int i = 1; i < maps.size(); i++) {
        IntMap map = maps[i];
        int val = map.get(key);

        if (NO_VALUE == val) {
          IntMap vmap = vmaps[i];
          int vval = vmap.get(key);
          if (NO_VALUE == vval) {
            found = false;
            break;
          }
        }
      }
      if (found) {
        r.push(key);
      }
    }
  }

  stack<int> * IntMap::intersect(vector<IntMap> & maps, vector<IntMap> & vmaps) {
    stack<int> *r = new stack<int>;
    intersect0(maps[0], maps, vmaps, *r);
    intersect0(vmaps[0], maps, vmaps, *r);
    return r;
  }

  // end changes

  int IntMap::put(int key, int value) {
    if (key == FREE_KEY) {
      int ret = m_freeValue;
      if (!m_hasFreeKey) {
        ++m_size;
      }
      m_hasFreeKey = true;
      m_freeValue = value;
      return ret;
    }

    int ptr = (phiMix(key) & m_mask) << 1;
    int k = m_data[ptr];
    if (k == FREE_KEY) //end of chain already
    {
      m_data[ptr] = key;
      m_data[ptr + 1] = value;
      if (m_size >= m_threshold) {
        rehash(m_data.size() * 2); //size is set inside
      } else {
        ++m_size;
      }
      return NO_VALUE;
    } else if (k == key) //we check FREE prior to this call
    {
      int ret = m_data[ptr + 1];
      m_data[ptr + 1] = value;
      return ret;
    }

    while (true) {
      ptr = ptr + 2 & m_mask2; //that's next index calculation
      k = m_data[ptr];
      if (k == FREE_KEY) {
        m_data[ptr] = key;
        m_data[ptr + 1] = value;
        if (m_size >= m_threshold) {
          rehash(m_data.size() * 2); //size is set inside
        } else {
          ++m_size;
        }
        return NO_VALUE;
      } else if (k == key) {
        int ret = m_data[ptr + 1];
        m_data[ptr + 1] = value;
        return ret;
      }
    }
  }

  int IntMap::remove(int key) {
    if (key == FREE_KEY) {
      if (!m_hasFreeKey)
        return NO_VALUE;
      m_hasFreeKey = false;
      --m_size;
      return m_freeValue; //value is not cleaned
    }

    int ptr = (phiMix(key) & m_mask) << 1;
    int k = m_data[ptr];
    if (k == key) //we check FREE prior to this call
    {
      int res = m_data[ptr + 1];
      shiftKeys(ptr);
      --m_size;
      return res;
    } else if (k == FREE_KEY)
      return NO_VALUE; //end of chain already
    while (true) {
      ptr = ptr + 2 & m_mask2; //that's next index calculation
      k = m_data[ptr];
      if (k == key) {
        int res = m_data[ptr + 1];
        shiftKeys(ptr);
        --m_size;
        return res;
      } else if (k == FREE_KEY)
        return NO_VALUE;
    }
  }

  int IntMap::shiftKeys(int pos) {
    // Shift entries with the same hash.
    int last, slot;
    int k;
    vector<int> & data = m_data;
    while (true) {
      pos = (last = pos) + 2 & m_mask2;
      while (true) {
        if ((k = data[pos]) == FREE_KEY) {
          data[last] = FREE_KEY;
          return last;
        }
        slot = (phiMix(k) & m_mask) << 1; //calculate the starting slot for the current key
        if (last <= pos ? last >= slot || slot > pos : last >= slot && slot > pos) {
          break;
        }
        pos = pos + 2 & m_mask2; //go to the next entry
      }
      data[last] = k;
      data[last + 1] = data[pos + 1];
    }
  }

  int IntMap::size() {
    return m_size;
  }

  void IntMap::rehash(int newCapacity) {
    m_threshold = (int) (newCapacity / 2 * m_fillFactor);
    m_mask = newCapacity / 2 - 1;
    m_mask2 = newCapacity - 1;

    int oldCapacity = m_data.size();
    vector<int> & oldData = m_data;

    m_data.resize(newCapacity);
    m_size = m_hasFreeKey ? 1 : 0;

    for (int i = 0; i < oldCapacity; i += 2) {
      int oldKey = oldData[i];
      if (oldKey != FREE_KEY) {
        put(oldKey, oldData[i + 1]);
      }
    }
  }

  /** Taken from FastUtil implementation */

  /** Return the least power of two greater than or equal to the specified value.
   *
   * <p>Note that this function will return 1 when the argument is 0.
   *
   * @param x a long integer smaller than or equal to 2<sup>62</sup>.
   * @return the least power of two greater than or equal to the specified value.
   */
    long IntMap::nextPowerOfTwo(long x) {
    if (x == 0)
      return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return (x | x >> 32) + 1;
  }

  /** Returns the least power of two smaller than or equal to 2<sup>30</sup>
   * and larger than or equal to <code>Math.ceil( expected / f )</code>.
   *
   * @param expected the expected number of elements in a hash table.
   * @param f the load factor.
   * @return the minimum possible size for a backing array.
   * @throws IllegalArgumentException if the necessary size is larger than 2<sup>30</sup>.
   */
   int IntMap::arraySize(int expected, float f) {
    long s = max(2l, nextPowerOfTwo((long) ceil(expected / f)));
    if (s > 1 << 30) return -1;
      //throw new IllegalArgumentException("Too large (" + expected + " expected elements with load factor " + f + ")");
    return (int) s;
  }


  int IntMap::phiMix(int x) {
    int h = x * INT_PHI;
    return h ^ h >> 16;
  }

  string IntMap::show()
  {
    string b = "{";
    int l = m_data.size();
    bool first = true;
    for (int i = 0; i < l; i += 2) {

      int v = m_data[i];
      if (v != FREE_KEY) {
        if (!first) {
          b += ",";
        }
        first = false;
        b+=v-1;;
      }
    }
    b+="}";
    return b;
  }

/*
  @Override
  public String toString() {
    //return java.util.Arrays.toString(m_data);
    final StringBuffer b = new StringBuffer("{");
    final int l = m_data.length;
    boolean first = true;
    for (int i = 0; i < l; i += 2) {

      final int v = m_data[i];
      if (v != FREE_KEY) {
        if (!first) {
          b.append(',');
        }
        first = false;
        b.append(v - 1);
      }
    }
    b.append("}");
    return b.toString();
  }*/


#ifdef EXE

  int main()
  {
	  IntMap m(20);

	  

	  for (int i = 0;i < 10; i++)
		  m.put(i*2, i * 10);

	  for (int i = 0; i < 10; i++)
	  {
		  printf("i=%d: %d\n", i, (int)m.contains(i));
	  }

	  for (int i = 0; i < 10; i++)
	  {
		  printf("i=%d: %d\n", i*2, (int)m.get(i*2));
	  }

	  return 0;
  }

#endif
