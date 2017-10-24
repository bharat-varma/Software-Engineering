
#include <string>
#include <unordered_map>
#include <vector>
#include <boost/optional.hpp>

namespace iProlog
{

	template<typename K>
	class IMap final : public java::io::Serializable
	{
  private:
	  static constexpr long long serialVersionUID = 1LL;

	  const std::unordered_map<K, IntMap*> map;

  public:
	  IMap() : map(std::unordered_map<K, IntMap*>())
	  {
	  }

	  void clear()
	  {
		map.clear();
	  }

	  bool put(K const key, int const val)
	  {
		IntMap *vals = map[key];
		if (nullptr == vals)
		{
		  vals = new IntMap();
		  map.emplace(key, vals);
		}
		return vals->add(val);
	  }

	  IntMap *get(K const key)
	  {
		IntMap *s = map[key];
		if (nullptr == s)
		{
		  s = new IntMap();
		}
		return s;
	  }

	  bool remove(K const key, int const val)
	  {
		IntMap * const vals = get(key);
		constexpr bool ok = vals->delete(val);
		if (vals->isEmpty())
		{
		  map.erase(key);
		}
		return ok;
	  }

	  bool remove(K const key)
	  {
		return nullptr != map.erase(key);
	  }
	  int size()
	  {
		const Iterator<K> I = map.keySet().begin();
		int s = 0;
		while (I->hasNext())
		{
		  constexpr K key = I->next();
		  IntMap * const vals = get(key);
		  s += vals->size();
			I++;
		}
		return s;
	  }

	  Set<K> *keySet()
	  {
		return map.keySet();
	  }

	  Iterator<K> *keyIterator()
	  {
		return keySet()->begin();
	  }

	  std::wstring toString() override
	  {

		return map.toString();
	  }

	  // specialization for array of int maps

	  static std::vector<IMap<int>*> create(int const l)
	  {
		IMap<int> * const first = new IMap<int>();

		std::vector<IMap<int>*> imaps = static_cast<std::vector<IMap<int>*>>(java::lang::reflect::Array::newInstance(first->getClass(), l));
		//new IMap[l];
		imaps[0] = first;
		for (int i = 1; i < l; i++)
		{
		  imaps[i] = new IMap<int>();
		}
		return imaps;
	  }

	  static bool put(std::vector<IMap<int>*> &imaps, int const pos, int const key, int const val)
	  {
		return imaps[pos]->put(boost::optional<int>(key), val);
	  }

	  static std::vector<int> get(std::vector<IMap<int>*> &iMaps, std::vector<IntMap*> &vmaps, std::vector<int> &keys)
	  {
		constexpr int l = iMaps.size();
		const std::vector<IntMap*> ms = std::vector<IntMap*>();
		const std::vector<IntMap*> vms = std::vector<IntMap*>();

		for (int i = 0; i < l; i++)
		{
		  constexpr int key = keys[i];
		  if (0 == key)
		  {
			continue;
		  }
		  //Main.pp("i=" + i + " ,key=" + key);
		  IntMap * const m = iMaps[i]->get(boost::optional<int>(key));
		  //Main.pp("m=" + m);
		  ms.push_back(m);
		  vms.push_back(vmaps[i]);
		}
		std::vector<IntMap*> ims(ms.size());
		std::vector<IntMap*> vims(vms.size());

		for (int i = 0; i < ims.size(); i++)
		{
		  IntMap * const im = ms[i];
		  ims[i] = im;
		  IntMap * const vim = vms[i];
		  vims[i] = vim;
		}
		//Main.pp("-------ims=" + Arrays.toString(ims));
		//Main.pp("-------vims=" + Arrays.toString(vims));

		IntStack * const cs = IntMap::intersect(ims, vims); // $$$ add vmaps here
		const std::vector<int> is = cs->toArray();
		for (int i = 0; i < is.size(); i++)
		{
		  is[i] = is[i] - 1;
		}
		Arrays::sort(is);
		return is;
	  }

	  static std::wstring show(std::vector<IMap<int>*> &imaps)
	  {
//There is no native C++ equivalent to 'toString':
		return Arrays->toString(imaps);
	  }

	  static std::wstring show(std::vector<int> &is)
	  {

		return Arrays->toString(is);
	  }

	  /*
	  public static void main(final String[] args) {
	    final IMap<Integer>[] imaps = create(3);
	    put(imaps, 0, 10, 100);
	    put(imaps, 1, 20, 200);
	    put(imaps, 2, 30, 777);
	    put(imaps, 0, 10, 1000);
	    put(imaps, 1, 20, 777);
	    put(imaps, 2, 30, 3000);
	    put(imaps, 0, 10, 777);
	    put(imaps, 1, 20, 20000);
	    put(imaps, 2, 30, 30000);
	    put(imaps, 0, 10, 888);
	    put(imaps, 1, 20, 888);
	    put(imaps, 2, 30, 888);
	    put(imaps, 0, 10, 0);
	    put(imaps, 1, 20, 0);
	    put(imaps, 2, 30, 0);
	    //Main.pp(show(imaps));
	    //final int[] keys = { 10, 20, 30 };
	    //Main.pp("get=" + show(get(imaps, keys)));
	    final IMap<Integer>[] m = create(4);
	    Engine.put(m, new int[] { -3, -4, 0, 0 }, 0);
	    Engine.put(m, new int[] { -3, -21, 0, -21 }, 1);
	    Engine.put(m, new int[] { -19, 0, 0, 0 }, 2);
	    
	    final int[] ks = new int[] { -3, -21, -21, 0 };
	    Main.pp(show(m));
	    Main.pp("ks=" + Arrays.toString(ks));
	    
	    Main.pp("get=" + show(get(m, ks)));
	  }*/

	};
}

		

