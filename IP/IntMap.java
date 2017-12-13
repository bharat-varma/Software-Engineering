/**
 * derived from code at https://github.com/mikvor/hashmapTest
 */

import java.util.*;

class Ran1
{
  final int IA = 16807;
  final int IM = 2147483647;
  final int  IQ = 127773;
  final int IR  = 2836;
  final int NTAB = 32;
  final double EPS =  (1.2E-07);

  int idum;
  int [] iv;
  int iy=0;

  public Ran1(int seed)
  {
    idum = seed;
    iv = new int[NTAB];
    iy = 0;
  }

public double ran1()
{
    int j,k;
         double NDIV = 1.0/(1.0+(IM-1.0)/NTAB);
         double RNMX = (1.0-EPS);
         double AM = (1.0/IM);

  if ((idum <= 0) || (iy == 0)) {
    idum = Math.max(-idum,idum);
                for(j=NTAB+7;j>=0;j--) {
      k = idum/IQ;
      idum = IA*(idum-k*IQ)-IR*k;
      if(idum < 0) idum += IM;
      if(j < NTAB) iv[j] = idum;
    }
    iy = iv[0];
  }
  k = idum/IQ;
  idum = IA*(idum-k*IQ)-IR*k;
  if(idum<0) idum += IM;
  j = (int)((double)iy*NDIV);
  iy = iv[j];
  iv[j] = idum;
  return Math.min(AM*iy,RNMX);
}

int random_int(int a, int b)
{
  return (int)((double)(b-a)*ran1())+a;
}

}


//package iProlog;
class IntMap implements java.io.Serializable {
  private static final long serialVersionUID = 1L;

  private static final int FREE_KEY = 0;

  static final int NO_VALUE = 0;

  /** Keys and values */
  private int[] m_data;

  /** Do we have 'free' key in the map? */
  private boolean m_hasFreeKey;
  /** Value of 'free' key */
  private int m_freeValue;

  /** Fill factor, must be between (0 and 1) */
  private final float m_fillFactor;
  /** We will resize a map once it reaches this size */
  private int m_threshold;
  /** Current map size */
  private int m_size;

  /** Mask to calculate the original position */
  private int m_mask;
  private int m_mask2;

  IntMap() {
    this(1 << 2);
  }

  IntMap(final int size) {
    this(size, 0.75f);
  }

  IntMap(final int size, final float fillFactor) {
    if (fillFactor <= 0 || fillFactor >= 1)
      throw new IllegalArgumentException("FillFactor must be in (0, 1)");
    if (size <= 0)
      throw new IllegalArgumentException("Size must be positive!");
    final int capacity = arraySize(size, fillFactor);
    m_mask = capacity - 1;
    m_mask2 = capacity * 2 - 1;
    m_fillFactor = fillFactor;

    m_data = new int[capacity * 2];
    m_threshold = (int) (capacity * fillFactor);
  }

  final int get(final int key) {
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

  final boolean contains(final int key) {
    return NO_VALUE != get(key);
  }

  final boolean add(final int key) {
    return NO_VALUE != put(key, 666);
  }

  boolean delete(final int key) {
    return NO_VALUE != remove(key);
  }

  final boolean isEmpty() {
    return 0 == m_size;
  }

  final static void intersect0(final IntMap m, final IntMap[] maps, final IntMap[] vmaps, final IntStack r) {
    final int[] data = m.m_data;
    for (int k = 0; k < data.length; k += 2) {
      boolean found = true;
      final int key = data[k];
      if (FREE_KEY == key) {
        continue;
      }
      for (int i = 1; i < maps.length; i++) {
        final IntMap map = maps[i];
        final int val = map.get(key);

        if (NO_VALUE == val) {
          final IntMap vmap = vmaps[i];
          final int vval = vmap.get(key);
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

  final static IntStack intersect(final IntMap[] maps, final IntMap[] vmaps) {
    final IntStack r = new IntStack();

    intersect0(maps[0], maps, vmaps, r);
    intersect0(vmaps[0], maps, vmaps, r);
    return r;
  }

  // end changes

  final int put(final int key, final int value) {
    if (key == FREE_KEY) {
      final int ret = m_freeValue;
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
        rehash(m_data.length * 2); //size is set inside
      } else {
        ++m_size;
      }
      return NO_VALUE;
    } else if (k == key) //we check FREE prior to this call
    {
      final int ret = m_data[ptr + 1];
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
          rehash(m_data.length * 2); //size is set inside
        } else {
          ++m_size;
        }
        return NO_VALUE;
      } else if (k == key) {
        final int ret = m_data[ptr + 1];
        m_data[ptr + 1] = value;
        return ret;
      }
    }
  }

  final int remove(final int key) {
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
      final int res = m_data[ptr + 1];
      shiftKeys(ptr);
      --m_size;
      return res;
    } else if (k == FREE_KEY)
      return NO_VALUE; //end of chain already
    while (true) {
      ptr = ptr + 2 & m_mask2; //that's next index calculation
      k = m_data[ptr];
      if (k == key) {
        final int res = m_data[ptr + 1];
        shiftKeys(ptr);
        --m_size;
        return res;
      } else if (k == FREE_KEY)
        return NO_VALUE;
    }
  }

  final private int shiftKeys(int pos) {
    // Shift entries with the same hash.
    int last, slot;
    int k;
    final int[] data = m_data;
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

  final int size() {
    return m_size;
  }

  final private void rehash(final int newCapacity) {
    m_threshold = (int) (newCapacity / 2 * m_fillFactor);
    m_mask = newCapacity / 2 - 1;
    m_mask2 = newCapacity - 1;

    final int oldCapacity = m_data.length;
    final int[] oldData = m_data;

    m_data = new int[newCapacity];
    m_size = m_hasFreeKey ? 1 : 0;

    for (int i = 0; i < oldCapacity; i += 2) {
      final int oldKey = oldData[i];
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
  final private static long nextPowerOfTwo(long x) {
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
  final private static int arraySize(final int expected, final float f) {
    final long s = Math.max(2, nextPowerOfTwo((long) Math.ceil(expected / f)));
    if (s > 1 << 30)
      throw new IllegalArgumentException("Too large (" + expected + " expected elements with load factor " + f + ")");
    return (int) s;
  }

  //taken from FastUtil
  private static final int INT_PHI = 0x9E3779B9;

  final private static int phiMix(final int x) {
    final int h = x * INT_PHI;
    return h ^ h >> 16;
  }

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
  }
  
  public static void main(String [] args)
  {

    int seed = -1;
    if(args.length > 0)
    {
      seed = Integer.parseInt(args[0]);
      if (seed > 0) seed *= -1;
    }

    Ran1 rnd = new Ran1(seed);

	  IntMap m = new IntMap(20);	 

	  for (int i = 0;i < 10; i++)
		  m.put(i*2, rnd.random_int(1,5));

	  for (int i = 0; i < 10; i++)
	  {
		  System.out.printf("i=%d: %d\n", i, m.contains(i)?1:0);
	  }

	  for (int i = 0; i < 10; i++)
	  {
		  System.out.printf("i=%d: %d\n", i, (int)m.get(i));
	  }
      
  }

}


class IntStack {

  private int stack[];

  private int top;

  static int SIZE = 16; // power of 2

  static int MINSIZE = 1 << 15; // power of 2

  IntStack() {
    this(SIZE);
  }

  IntStack(final int size) {
    stack = new int[size];
    clear();
  }

  final int getTop() {
    return top;
  }

  final int setTop(final int top) {
    return this.top = top;
  }

  final void clear() {
    //for (int i = 0; i <= top; i++)
    //stack[i] = 0;
    top = -1;
  }

  final boolean isEmpty() {
    return top < 0;
  }

  /**
   * Pushes an element - top is incremented first than the
   * element is assigned. This means top point to the last assigned
   * element - which can be returned with peek().
   */
  final void push(final int i) {
    // IO.dump("push:"+i);
    if (++top >= stack.length) {
      expand();
    }
    stack[top] = i;
  }

  final int pop() {
    final int r = stack[top--];
    shrink();
    return r;
  }

  final int get(final int i) {
    return stack[i];
  }

  final void set(final int i, final int val) {
    stack[i] = val;
  }

  final int size() {
    return top + 1;
  }

  /**
   * dynamic array operation: doubles when full
   */
  private final void expand() {
    final int l = stack.length;
    final int[] newstack = new int[l << 1];

    System.arraycopy(stack, 0, newstack, 0, l);
    stack = newstack;
  }

  /**
  * dynamic array operation: shrinks to 1/2 if more than than 3/4 empty
  */
  private final void shrink() {
    int l = stack.length;
    if (l <= MINSIZE || top << 2 >= l)
      return;
    l = 1 + (top << 1); // still means shrink to at 1/2 or less of the heap
    if (top < MINSIZE) {
      l = MINSIZE;
    }

    final int[] newstack = new int[l];
    System.arraycopy(stack, 0, newstack, 0, top + 1);
    stack = newstack;
  }

  int[] toArray() {
    final int[] array = new int[size()];
    if (size() > 0) {
      System.arraycopy(stack, 0, array, 0, size());
    }
    return array;
  }

  public final void reverse() {
    int l = size();
    int h = l >> 1;
    // Prolog.dump("l="+l);
    for (int i = 0; i < h; i++) {
      int temp = stack[i];
      stack[i] = stack[l - i - 1];
      stack[l - i - 1] = temp;
    }
  }

  @Override
  public String toString() {
    return Arrays.toString(toArray());
  }

}

