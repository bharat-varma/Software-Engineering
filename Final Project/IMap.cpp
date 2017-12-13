#include "IMap.h"

template<class K>
  IMap<K>::IMap() {
  }

template<class K>
  void IMap<K>::clear() {
    mp.clear();
  }

template<class K>
  bool IMap<K>::put(K key, int val) {
    if(mp.find(key) == mp.end())
    {
      mp[key] = new IntMap();
    }
    mp[key]->add(val);
    /*
    IntMap vals = mp.get(key);
    if (null == vals) {
      vals = new IntMap();
      map.put(key, vals);
    }
    return vals.add(val);
    */
	return mp[key]->add(val);
  }

template<class K>
  IntMap IMap<K>::putget(K key) {

    //IntMap *s;
    if(mp.find(key) == mp.end())
    //if(!mp.contains(key))
    {
      //s = new IntMap();
      return IntMap();
    }
    else
    {
      //s = mp[key];
      return *mp[key];
    }
    /*
    IntMap s = map.get(key);
    if (null == s) {
      s = new IntMap();
    }
    */
    //return s;
  }

template<class K>
void remove_key(map<K,IntMap*> &mp, vector<K> mp_lst, K k)
{
  auto element = mp.find(k);
  mp.erase(element);

  for(int i = 0; i < mp_lst.size(); i++)
  {
    if(mp_lst[i] == k)
    {
      mp_lst.erase(mp_lst.begin()+i);
      return;
    }
  }
}

template<class K>
  bool IMap<K>::remove(K key, int val) {
    IntMap vals = get(key);
    bool ok = vals.del(val);
    if (vals.isEmpty()) {
      remove_key(mp, mp_lst, key);
     // mp.remove(key);
    }
    return ok;
  }

template<class K>
  bool IMap<K>::remove(K key) {
    bool ret = mp.find(key) != mp.end();
    remove_key(mp, mp_lst, key);
    return ret;
    //return 0 != mp.remove(key);
  }

template<class K>
  int IMap<K>::size() {

    int s = 0;
    for(const auto & k : mp_lst)
    {
      //IntMap *mp[k]
      s += get(k).size();
    }
    /*
    Iterator<K> I = mp.keySet().iterator();
    int s = 0;
    while (I.hasNext()) {
      K key = I.next();
      IntMap vals = get(key);
      s += vals.size();
    }*/
    return s;
  }

/*
template<class K>
  set<K> IMap<K>::keySet() {
    set<K> s;
    for(const auto & k : mp_lst)
    {
      s.push(k);
    }
    return s;
  }
  */


/*
  Iterator<K> keyIterator() {
    return keySet().iterator();
  }*/

/*
  @Override
  public String toString() {
    return map.toString();
  }*/

  // specialization for array of int maps

template<class K>
  vector<IMap<int> > * IMap<K>::create(int l) {
    IMap<int> first = IMap<int>();
    vector<IMap<int> >* imaps = new vector<IMap<int> >(l);// = (IMap<int>[]) java.lang.reflect.Array.newInstance(first.getClass(), l);

    // imaps = (IMap<int>[]) java.lang.reflect.Array.newInstance(first.getClass(), l);
    //new IMap[l];
    (*imaps)[0] = first;
    for (int i = 1; i < l; i++) {
      (*imaps)[i] = IMap<int>();
    }

    return imaps;
  }

template<class K>
  bool IMap<K>::put(vector<IMap<int> > imaps, int pos, int key, int val) {
    return imaps[pos].put(key, val);
  }

template<class K>
  vector<int> * IMap<K>::get(vector<IMap<int> > *iMaps, vector<IntMap> vmaps, vector<int> keys) {
    int l = iMaps->size();
    vector<IntMap> ms; // = new ArrayList<IntMap>();
    vector<IntMap> vms; // = new ArrayList<IntMap>();

    for (int i = 0; i < l; i++) {
      int key = keys[i];
      if (0 == key) {
        continue;
      }
      //Main.pp("i=" + i + " ,key=" + key);
      IntMap m = (*iMaps)[i].get(key);
      //Main.pp("m=" + m);
      ms.push_back(m);
      vms.push_back(vmaps[i]);
    }
    vector<IntMap> ims(ms.size());// = new IntMap[ms.size()];
    vector<IntMap> vims(vms.size());// = new IntMap[vms.size()];

    for (int i = 0; i < ims.size(); i++) {
      IntMap im = ms[i];
      ims[i] = im;
      IntMap vim = vms[i];
      vims[i] = vim;
    }

    //Main.pp("-------ims=" + Arrays.toString(ims));
    //Main.pp("-------vims=" + Arrays.toString(vims));

    stack<int> *cs = IntMap::intersect(ims, vims); // $$$ add vmaps here

    //vector<int> *is = new vector<int>();
    int n = cs->size();

    //int[] is = cs.toArray();

    vector<int> *is = new vector<int>(n);
    for(int i=n-1;i>=0;i--)
    {
      (*is)[i] = cs->top();
      cs->pop();
    }
    
    delete cs;

    for (int i = 0; i < is->size(); i++) {
      (*is)[i] = (*is)[i] - 1;
    }
    sort(is->begin(), is->end());
    //java.util.Arrays.sort(is);
    return is;
  }

template<class K>
  string IMap<K>::show()
  {
    string s = "";

    for(K k : mp_lst)
    {
      //char buffer[256];
      //sprintf(buffer, "%d : ")
      //s += k + " : " + mp[k];
     // s += ",";
    }

    return s;
  }

template<class K>
  IntMap IMap<K>::get(K key) {

    if(mp.find(key) == mp.end())
    {
        return IntMap();
    }
/*
    return mp[key];
    IntMap s = map.get(key);
    if (null == s) {
      s = new IntMap();
    }
    return s;
    */
  }  



template<class K>
  string IMap<K>::show(vector<IMap<int> > imaps) {
    string s = "";
    for(auto x : imaps)
    {
      s += x.show();
    }
    return s; 
    //Arrays.toString(imaps);
  }


/*
  string show(vector<int> is) {
    return Arrays.toString(is);
  }*/



// end

/*
  int main() {
    vector<IMap<int> > *imaps = IMap<int>::create(3);

    printf("1\n");
    IMap<int>::put(*imaps, 0, 10, 100);
    IMap<int>::put(*imaps, 1, 20, 200);
    IMap<int>::put(*imaps, 2, 30, 777);

    IMap<int>::put(*imaps, 0, 10, 1000);
    IMap<int>::put(*imaps, 1, 20, 777);
    IMap<int>::put(*imaps, 2, 30, 3000);

    IMap<int>::put(*imaps, 0, 10, 777);
    IMap<int>::put(*imaps, 1, 20, 20000);
    IMap<int>::put(*imaps, 2, 30, 30000);

    IMap<int>::put(*imaps, 0, 10, 888);
    IMap<int>::put(*imaps, 1, 20, 888);
    IMap<int>::put(*imaps, 2, 30, 888);

    IMap<int>::put(*imaps, 0, 10, 0);
    IMap<int>::put(*imaps, 1, 20, 0);
    IMap<int>::put(*imaps, 2, 30, 0);

    return 0;

  }
*/

template class IMap<int>;
