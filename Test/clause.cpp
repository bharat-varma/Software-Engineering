  #include "clause.h"

#include <cstdio>

  Clause::Clause(int len,  vector<int> & hgs, int base,  int neck,  vector<int> xs) {
    this->hgs = hgs; // head+goals pointing to cells in cs
    this->base = base; // heap where this starts
    this->len = len; // length of heap slice
    this->neck = neck; // first after the end of the head
    this->xs = xs; // indexables in head
  }


#ifdef EXE

  int main()
  {
	  vector<int> hgs, xs;
	  hgs.push_back(0);
	  hgs.push_back(1);
	  hgs.push_back(2);
	  xs.push_back(-1);
	  Clause C(10,hgs,0,-1,xs);

	  printf("Len: %d\n", C.len);
	  printf("Base: %d\n", C.base);
	  printf("Neck: %d\n", C.neck);
	  printf("hgs:\n");
	  for (int x : C.hgs) printf("%d ", x);
	  printf("\nxs:\n");
	  for (int x : C.xs) printf("%d ", x);
	  printf("\n");

	  return 0;
  }

#endif