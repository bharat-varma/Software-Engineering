#ifndef __PROG_H__
#define __PROG_H__

#include <string>
#include "Engine.h"
#include "clause.h"

using namespace std;

class Prog : public Engine
{
public:

	Prog(string fname);

	string showTerm(Object & O);

	static bool isListCons(string sr);
	static string maybeNull(Object * O);

	static bool isOp(string  sr);

	static string st0(OComplex * args);

	void ppCode();

	
	string showClause(Clause &  s);



};

#endif

