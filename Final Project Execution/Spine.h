#ifndef __SPINE_H__
#define __SPINE_H__

#include <vector>
#include <stdio.h>

using namespace std;

/**
 * runtime representation of an immutable list of goals
 * together with top of heap and trail pointers
 * and current clause tried out by head goal
 * as well as registers associated to it
 *
 * note that parts of this immutable lists
 * are shared among alternative branches
 */
class Spine {

public:

  /**
   * creates a spine - as a snapshot of some runtime elements
   */
  Spine(vector<int> gs0,  int base, vector<int> gs,  int ttop,  int k, vector<int> cs);

  /**
   * creates a specialized spine returning an answer (with no goals left to solve)
   */
  Spine(int hd, int ttop);

  Spine();

  int hd; // head of the clause to which this corresponds
  int base; // top of the heap when this was created

  vector<int> gs; // goals - with the top one ready to unfold
  int ttop; // top of the trail when this was created

  int k;

  vector<int> *xs; // index elements
  vector<int> cs; // array of  clauses known to be unifiable with top goal in gs
};

#endif
