#include "Spine.h"

using namespace std;

Spine::Spine(vector<int> gs0,  int base, vector<int> gs,  int ttop,  int k, vector<int> cs) {
//Spine::Spine(int * gs0, int gs_n, int base, vector<int> gs, int ttop, int k, int * cs) {
    hd = gs0[0];
    this->base = base;

	vector<int> tmp;

    for(int i=1;i<gs0.size();i++)
		tmp.push_back(gs0[i]);
    tmp.insert(tmp.end(), gs.begin(), gs.end());
	this->gs = tmp;
    //IntList.tail(IntList.app(gs0, gs)); // prepends the goals of clause with head hs
    this->ttop = ttop;
    this->k = k;
    this->cs = cs;

	this->xs = NULL;//new vector<int>();
  }
Spine::Spine(int hd, int ttop) {
    this->hd = hd;
    base = 0;
    //gs = IntList.empty;
    this->ttop = ttop;

    this->xs  = NULL;

    k = -1;
    //cs = NULL;
  }

Spine::Spine()
{
  //cs = NULL;
  xs = NULL;
  k = -1;
  
}

  