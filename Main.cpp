#include <string>
#include <vector>
#include <iostream>

namespace iProlog
{
	class Main
	{

  public:
	  static void println(void *const o);

	  static void pp(void *const o);

	  static void run(const std::wstring &fname0);

	  static void srun(const std::wstring &fname0);

	  static void main(std::vector<std::wstring> &args);
	};

	void Main::run(const std::wstring &fname0)
	{
	  constexpr bool p = true;

	  const std::wstring fname = fname0 + L".nl";
	  Engine *P;

	  if (p)
	  {
		P = new Prog(fname);
		pp(L"CODE");
		(static_cast<Prog*>(P))->ppCode();
	  }
	  else
	  {
		P = new Engine(fname);
	  }

	  pp(L"RUNNING");
	  constexpr long long t1 = System::nanoTime();
	  P->run();
	  constexpr long long t2 = System::nanoTime();
	  std::wcout << L"time=" << (t2 - t1) / 1000000000.0 << std::endl;

	}


	void Main::main(std::vector<std::wstring> &args)
	{
		  std::wstring fname = args[0];
	  run(fname);
	}
}
