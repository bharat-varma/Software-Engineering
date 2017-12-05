#ifndef __CLAUSE_H__
#define __CLAUSE_H__

#include <vector>

using namespace std;

/**
 * representation of a clause
 */
class Clause {

public:

  Clause(int len,  vector<int> & hgs, int base,  int neck, vector<int> xs);

  int len;
  vector<int> hgs;
  int base;
  int neck;
  vector<int> xs;
  //  int * xs;
};

#endif
