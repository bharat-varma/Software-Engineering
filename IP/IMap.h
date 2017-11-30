#ifndef __IMAP_H__
#define __IMAP_H__

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include "IntMap.h"

using namespace std;

template <class K>
class IMap  {

public:

  static const long serialVersionUID = 1L;

  map<K, IntMap*> mp;
  vector<K> mp_lst;

  IMap();

  void clear();

  bool put(K key, int val);

  IntMap get(K key);

  IntMap putget(K key);

  bool remove(K key, int val);

  bool remove(K key);

  int size();

  //set<K> keySet();

  static string show(vector<IMap<int> > imaps);

  static vector<IMap<int> > * create(int l);

  static bool put(vector<IMap<int> > imaps, int pos, int key, int val);
  static bool put(vector<IMap<int> > *imaps, int pos, int key, int val);

  static vector<int> * get(vector<IMap<int> > *iMaps, vector<IntMap> vmaps, vector<int> keys);

  string show();
};
#endif
