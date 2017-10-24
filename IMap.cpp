
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
