#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <string>
#include <map>
#include <stack>
#include "Spine.h"
#include "clause.h"
#include "IntMap.h"
#include "IMap.h"
#include "toks.h"

#pragma warning(disable: 4996)


using namespace std;

#define MAXIND 3

void Mainpp(string s);


class Object
{
public:
	int type;
	Object();
	virtual string show()=0;
};

class Integer : public Object
{
public:
	Integer(int v);
	int v;
	string show();
};

class OString : public Object
{
public:
	OString(string s);
	string s;
	string show();
};

class OComplex : public Object {
public:
	OComplex(vector<Object *> v);
	vector<Object *> v;
	string show();
};

class my_stack : public stack< int > {
public:
	using stack<int>::c; // expose the container

	vector<int> toArray();

};

class SpineStack : public stack< Spine* > {
public:
	using stack<Spine*>::c; 
};


/**
* Implements execution mechanism
*/
class Engine {

public:

	//  static const int MAXIND = 3; // number of index args
	static const int START_INDEX = 20;
	// switches off indexing for less then START_INDEX clauses e.g. <20

	/**
	* Builds a new engine from a natural-language style assembler.nl file
	*/
	Engine(string fname);

	/**
	* trimmed down clauses ready to be quickly relocated to the heap
	*/
	vector<Clause> clauses;

	vector<int>  cls;
	//int * cls;
	/** symbol table made of map + reverse map from ints to syms */

	map<string, int> syms;
	vector<string> slist;

	void print_syms();
	//  final LinkedHashMap<String, Integer> syms;
	// final private ArrayList<String> slist;

	/** runtime areas:
	*
	* the heap contains code for and clauses their their copies
	* created during execution
	*
	* the trail is an undo list for variable bindings
	* that facilitates retrying failed goals with alternative
	* matching clauses
	*
	* the unification stack ustack helps handling term unification non-recursively
	*
	* the spines stack contains abstractions of clauses and goals and performs the
	* functions of  both a choice-point stack and goal stack
	*
	* imaps: contains indexes for up toMAXIND>0 arg positions (0 for pred symbol itself)
	*
	* vmaps: contains clause numbers for which vars occur in indexed arg positions
	*/

	vector<int> heap;
	int top;
	static const int MINSIZE = 1 << 15; // power of 2

	my_stack trail;
	my_stack ustack;
	SpineStack spines;
	// IntStack trail;
	// IntStack ustack;
	//  ObStack<Spine> spines = new ObStack<Spine>();

	~Engine();

	Spine * query;

	vector<IMap<int> > *imaps;
	vector<IntMap> vmaps;

	/**
	* tags of our heap cells - that can also be seen as
	* instruction codes in a compiled implementation
	*/
	static const int V = 0;
	static const int U = 1;
	static const int R = 2;

	static const int C = 3;
	static const int N = 4;

	static const int A = 5;

	// G - ground?

	static const int BAD = 7;

	/**
	* tags an integer value while fliping it into a negative
	* number to ensure that untagged cells are always negative and the tagged
	* ones are always positive - a simple way to ensure we do not mix them up
	* at runtime
	*/
	static int tag(int t, int w);

	/**
	* removes tag after flipping sign
	*/
	static int detag(int w);

	/**
	* extracts the tag of a cell
	*/
	static int tagOf(int w);

	/**
	* places an identifier in the symbol table
	*/
	int addSym(string sym);

	/**
	* returns the symbol associated to an integer index
	* in the symbol table
	*/
	string getSym(int w);

	void makeHeap();

	void makeHeap(int size);

	int getTop();

	int setTop(int top);

	void clear();

	/**
	* Pushes an element - top is incremented frirst than the
	* element is assigned. This means top point to the last assigned
	* element - which can be returned with peek().
	*/
	void push(int i);

	int size();

	/**
	* dynamic array operation: doubles when full
	*/

	void expand();

	void ensureSize(int more);

	/**
	* expands a "Xs lists .." statements to "Xs holds" statements
	*/

	static vector<vector <string> > * maybeExpand(vector<string> Ws);

	/**
	* expands, if needed, "lists" statements in sequence of statements
	*/
	static vector<vector<string> > mapExpand(vector<vector<string> > Wss);

	/**
	* loads a program from a .nl file of
	* "natural language" equivalents of Prolog/HiLog statements
	*/
	vector<Clause> dload(string s);

	vector<int> toNums(vector<Clause> clauses);

	/*
	* encodes string constants into symbols while leaving
	* other data types untouched
	*/

	int encode(int t, string s);

	/**
	* true if cell x is a variable
	* assumes that variables are tagged with 0 or 1
	*/


	static bool isVAR(int x);


	/**
	* returns the heap cell another cell points to
	*/

	int getRef(int x);

	/*
	* sets a heap cell to point to another one
	*/

	void setRef(int w, int r);

	/**
	* removes binding for variable cells
	* above savedTop
	*/


	void unwindTrail(int savedTop);

	/**
	* scans reference chains starting from a variable
	* until it points to an unbound root variable or some
	* non-variable cell
	*/


	int deref(int x);
	/**
	* raw display of a term - to be overridden
	*/

	virtual string showTerm(int x);
	/**
	* raw display of a externalized term
	*/


	// todo

	string showTerm(Object *O);

	/**
	* prints out content of the trail
	*/

	/*
	void ppTrail();
	*/

	/**
	* builds an array of embedded arrays from a heap cell
	* representing a term for interaction with an external function
	* including a displayer
	*/

	typedef Object * pObject;

	Object * exportTerm(int x);

	/**
	* extracts an integer array pointing to
	* the skeleton of a clause: a cell
	* pointing to its head followed by cells pointing to its body's
	* goals
	*/

	/*
	static int[] getSpine(final int[] cs) {
	final int a = cs[1];
	final int w = detag(a);
	final int[] rs = new int[w - 1];
	for (int i = 0; i < w - 1; i++) {
	final int x = cs[3 + i];
	final int t = tagOf(x);
	if (R != t) {
	Main.pp("*** getSpine: unexpected tag=" + t);
	return null;
	}
	rs[i] = detag(x);
	}
	return rs;
	}
	*/

	/**
	* raw display of a cell as tag : value
	*/

	string showCell(int w);

	/**
	* a displayer for cells
	*/

	/*
	

	*/

	string showCells(int base, int len);
	//string showCells(vector <int> cs);

	/**
	* to be overridden as a printer of a spine
	*/
	void ppc(Spine C);

	/**
	* to be overridden as a printer for current goals
	* in a spine
	*/

	/*
	void ppGoals(final IntList gs);
	*/

	/**
	* to be overriden as a printer for spines
	*/
	/*
	void ppSpines();
	*/

	/**
	* unification algorithm for cells X1 and X2 on ustack that also takes care
	* to trail bindigs below a given heap address "base"
	*/

	bool unify(int base);

	bool unify_args(int w1, int w2);

	/**
	* places a clause built by the Toks reader on the heap
	*/


	Clause putClause(vector <int> cs, vector<int> gs, int neck);

	/**
	* relocates a variable or array reference cell by b
	* assumes var/ref codes V,U,R are 0,1,2
	*/

	static int relocate(int b, int cell);

	/**
	* pushes slice[from,to] of array cs of cells to heap
	*/

	void pushCells(int b, int from, int to, int base);

	/**
	* pushes slice[from,to] of array cs of cells to heap
	*/

	void pushCells(int b, int from, int to, vector<int> & cs);

	/**
	* copies and relocates head of clause at offset from heap to heap
	*/

	int pushHead(int b, Clause & C);

	/**
	* copies and relocates body of clause at offset from heap to heap
	* while also placing head as the first element of array gs that
	* when returned contains references to the toplevel spine of the clause
	*/

	vector<int> pushBody(int b, int head, Clause &C);
	/**
	* makes, if needed, registers associated to top goal of a Spine
	* these registers will be reused when matching with candidate clauses
	* note that xs contains dereferenced cells - this is done once for
	* each goal's toplevel subterms
	*/

	void makeIndexArgs(Spine * G, int goal);

	vector<int> getIndexables(int ref);

	int cell2index(int cell);

	/**
	* tests if the head of a clause, not yet copied to the heap
	* for execution could possibly match the current goal, an
	* abstraction of which has been place in xs
	*/

	bool match(vector<int> *xs, Clause & C0);

	/**
	* transforms a spine containing references to choice point and
	* immutable list of goals into a new spine, by reducing the
	* first goal in the list with a clause that successfully
	* unifies with it - in which case places the goals of the
	* clause at the top of the new list of goals, in reverse order
	*/

	Spine * unfold(Spine * G);
	/**
	* extracts a query - by convention of the form
	* goal(Vars):-body to be executed by the engine
	*/

	Clause getQuery();
	/**
	* returns the initial spine built from the
	* query from which execution starts
	*/

	Spine * init();
	/**
	* returns an answer as a Spine while recording in it
	* the top of the trail to allow the caller to retrieve
	* more answers by forcing backtracking
	*/

	Spine * answer(int ttop);

	/**
	* detects availability of alternative clauses for the
	* top goal of this spine
	*/

	bool hasClauses(Spine *S);

	/**
	* true when there are no more goals left to solve
	*/


	bool hasGoals(Spine * S);

	/**
	* removes this spines for the spine stack and
	* resets trail and heap to where they where at its
	* creating time - while undoing variable binding
	* up to that point
	*/


	void popSpine();

	/**
	* main interpreter loop: starts from a spine and works
	* though a stream of answers, returned to the caller one
	* at a time, until the spines stack is empty - when it
	* returns null
	*/


	Spine * yield();

	/**
	* retrieves an answers and ensure the engine can be resumed
	* by unwinding the trail of the query Spine
	* returns an external "human readable" representation of the answer
	*/


	Object * ask();
	/**
	* initiator and consumer of the stream of answers
	* generated by this engine
	*/

	void run();
	// indexing extensions - ony active if START_INDEX clauses or more


	static vector<IntMap> vcreate(int l);

	static void put(vector<IMap<int> > imaps, vector<IntMap> vss, vector<int> keys, int val);

	vector<IMap<int> > * index(vector<Clause> clauses, vector<IntMap> vmaps);

};

#endif
