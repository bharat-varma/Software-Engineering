

#include "Engine.h"

#pragma warning(disable: 4996)

using namespace std;

#define MAXIND 3

void Mainpp(string s)
{
  printf("%s\n", s.c_str());
}

Object::Object()
  {
    type = 0;
  }


Integer::Integer(int v)
  {
    type = 1;
    this->v = v;
  }

string Integer::show()
{
	char buff[16];
	sprintf(buff, "%d", v);
	string s(buff);
	return s;
}

string OString::show()
{
	return s;
}

string OComplex::show()
{
	string s;
	s = v[0]->show();
	s += "(";
	for (int i = 1; i < v.size(); i++)
	{
		if (i > 1) s += ",";
		s += v[i]->show();
	}
	s += ")";
	return s;
}

OString::OString(string s)
  {
    type = 2;
    this->s = s;
  }


OComplex::OComplex(vector<Object *> v)
  {
    type = 3;
    this->v = v;
  }

    vector<int> my_stack ::toArray()
    {
      vector<int> v;
      for(int x : c)
      {
        v.push_back(x);
      }
      return v;
    }




	Engine::Engine(string fname) {
    //syms = new LinkedHashMap<String, Integer>();
    //slist = new ArrayList<String>();

    makeHeap();

    //trail = new IntStack();
    //ustack = new IntStack();

    clauses = dload(fname);

    cls = toNums(clauses);

    query = init();

    vmaps = vcreate(MAXIND);

    imaps = index(clauses, vmaps);
  }

	Engine::~Engine()
  {
    delete imaps;
  }

  int Engine::tag(int t, int w) {
    return -((w << 3) + t);
  }

  /**
   * removes tag after flipping sign
   */
  int Engine::detag(int w) {
    return -w >> 3;
  }

  /**
   * extracts the tag of a cell
   */
   int Engine::tagOf(int w) {
    return -w & 7;
  }

  /**
   * places an identifier in the symbol table
   */
  int Engine::addSym(string sym) {

    if(syms.find(sym) != syms.end())
      return syms[sym];
    int i = syms.size();
    syms[sym] = i;
    slist.push_back(sym);

    return i;


    /*
    Integer I = syms.get(sym);
    if (null == I) {
      final int i = syms.size();
      I = new Integer(i);
      syms.put(sym, I);
      slist.add(sym);
    }
    return I.intValue();
    */
  }

  /**
   * returns the symbol associated to an integer index
   * in the symbol table
   */
  string Engine::getSym(int w) {
    
	  if (w < 0 || w >= slist.size())
	  {
		  char buff[80];
		  sprintf(buff, "BADSYMREF=%d", w);
		  return string(buff);
	  }
     // return "BADSYMREF=" + w;

	  return slist[w]; //.get(w);
  }

  void Engine::makeHeap() {
    makeHeap(MINSIZE);
  }

  void Engine::makeHeap(int size) {
    heap.resize(size);// = new int[size];
    clear();
  }

  int Engine::getTop() {
    return top;
  }

  int Engine::setTop(int top) {
    return this->top = top;
  }

  void Engine::clear() {
    //for (int i = 0; i <= top; i++)
    //heap[i] = 0;
    top = -1;
  }

  /**
   * Pushes an element - top is incremented frirst than the
   * element is assigned. This means top point to the last assigned
   * element - which can be returned with peek().
   */
  void Engine::push(int i) {
    heap[++top] = i;
  }

  int Engine::size() {
    return top + 1;
  }

  /**
   * dynamic array operation: doubles when full
   */
    
  void Engine::expand() {
    int l = heap.size();
    heap.resize(l<<1);
    /*
    final int[] newstack = new int[l << 1];

    System.arraycopy(heap, 0, newstack, 0, l);
    heap = newstack;*/
  }

  void Engine::ensureSize(int more) {
    if (1 + top + more >= heap.size()) {
      expand();
    }

  }

  /**
  * expands a "Xs lists .." statements to "Xs holds" statements
  */

  vector<vector <string> > * Engine::maybeExpand(vector<string> Ws) {
	  char buff[128];

	  string W = Ws[0];
	  if (W.length() < 2 || !(W.substr(0, 2) == "l:"))
		  return NULL;

	  int l = Ws.size();
	  vector<vector<string> > * Rss = new vector<vector<string> >();
	  string V = W.substr(2);
	  for (int i = 1; i < l; i++)
	  {
		  vector<string> Rs(4);
		  string Vi;
		  if (i == 1)
			  Vi = V;
		  else
		  {
			  sprintf(buff, "%s__%d", V.c_str(), i - 1);
			  Vi = buff;
		  }

		  sprintf(buff, "%s__%d", V.c_str(), i);
		  string Vii = buff;
		  Rs[0] = "h:" + Vi;
		  Rs[1] = "c:list";
		  Rs[2] = Ws[i];
		  Rs[3] = i == l - 1 ? "c:nil" : "v:" + Vii;
		  Rss->push_back(Rs);

	  }


    return Rss;

  }

  /**
   * expands, if needed, "lists" statements in sequence of statements
   */
  vector<vector<string> > Engine::mapExpand(vector<vector<string> > Wss) {

	  vector<vector<string> > Rss;
	  for (int i = 0; i < Wss.size(); i++)
	  {
		  vector<string> Ws = Wss[i];
		  vector<vector<string> > *Hss = maybeExpand(Ws);

		  if (Hss == NULL)
		  {
			  vector<string> ws(Ws.size());
			  for (int j = 0; j < ws.size(); j++) {
				  ws[j] = Ws[j];
			  }
			  Rss.push_back(ws);
		  }
		  else
		  {
			  for (int j = 0; j < Hss->size(); j++)
			  {
				  vector<string> X = ((*Hss)[j]);
				  Rss.push_back(X);					  
			  }
		  }

		  }
    return Rss;
  }

  /**
   * loads a program from a .nl file of
   * "natural language" equivalents of Prolog/HiLog statements
   */
  vector<Clause> Engine::dload(string s) {
    
    bool fromFile = true;
    vector<vector<vector<string> > > Wsss = Toks::toSentences(s, fromFile);
    vector<Clause> Cs;
    for(vector<vector<string> > Wss : Wsss)
    {

      // clause starts here

      map<string, my_stack* > refs;
      vector<my_stack* > refs_list;
      my_stack cs;
      my_stack gs;

      vector<vector<string> > Rss = mapExpand(Wss);

      int k = 0;
      for (vector<string> ws : Rss) {

        // head or body element starts here

        int l = ws.size();
        gs.push(tag(R, k++));
        cs.push(tag(A, l));

        for (string w : ws) {

          // head or body subterm starts here

 if (1 == w.size()) {
            w = "c:" + w;
          }

          string L = w.substr(2);

          switch (w[0]) {
            case 'c':
              cs.push(encode(C, L));
              k++;
            break;
            case 'n':
              cs.push(encode(N, L));
              k++;
            break;
            case 'v': {

              if(refs.find(L) == refs.end())
              {
                my_stack *sp = new my_stack();
                refs[L] = sp;
                refs_list.push_back(sp);
              }
              refs[L]->push(k);
              cs.push(tag(BAD, k));
              k++;

            }
            break;
            case 'h': {


              if(refs.find(L) == refs.end())
              {
                my_stack *sp = new my_stack();
                refs[L] = sp;
                refs_list.push_back(sp);
              }
              refs[L]->push(k-1);

              cs.c[k - 1] =  tag(A, l - 1);
              gs.pop();

              //cs.push(tag(BAD, k));
              //k++;

            }
            break;
            default:
              Mainpp("FORGOTTEN=" + w);
          } // end subterm
        } // end element
      } // end clause

      // linker
     // Iterator<IntStack> K = refs.values().iterator();

      for(my_stack* Is : refs_list)
      {
        // final IntStack Is = K.next();

        // finding the A among refs
        int leader = -1;
        for (int j : Is->toArray()) {
          if (A == tagOf(cs.c[j])) {
            leader = j;
       
            break;
          }
        }
        if (-1 == leader) {
          // for vars, first V others U
          leader = (*Is).c[0];
          for (int i : Is->toArray()) {
            if (i == leader) {
              cs.c[i] = tag(V, i);
            } else {
              cs.c[i] = tag(U, leader);
            }

          }
        } else {
          for (int i : Is->toArray()) {
            if (i == leader) {
              continue;
            }
            cs.c[i] = tag(R, leader);
          }
        }
      }
/*
      while (K.hasNext()) {
        final IntStack Is = K.next();

        // finding the A among refs
        int leader = -1;
        for (final int j : Is.toArray()) {
          if (A == tagOf(cs.get(j))) {
            leader = j;

            break;
          }
        }
        if (-1 == leader) {
          // for vars, first V others U
          leader = Is.get(0);
          for (final int i : Is.toArray()) {
            if (i == leader) {
              cs.set(i, tag(V, i));
            } else {
              cs.set(i, tag(U, leader));
            }

          }
        } else {
          for (final int i : Is.toArray()) {
            if (i == leader) {
              continue;
            }
            cs.set(i, tag(R, leader));
          }
        }
      }*/

      int neck = 1 == gs.size() ? cs.size() : detag(gs.c[1]);
      vector<int> tgs = gs.toArray();

      Clause C = putClause(cs.toArray(), tgs, neck);

      Cs.push_back(C);      

    }

/*
    for (final ArrayList<ArrayList<String>> Wss : Wsss) {
      // clause starts here

      final LinkedHashMap<String, IntStack> refs = new LinkedHashMap<String, IntStack>();
      final IntStack cs = new IntStack();
      final IntStack gs = new IntStack();

      final ArrayList<String[]> Rss = mapExpand(Wss);
      int k = 0;
      for (final String[] ws : Rss) {

        // head or body element starts here

        final int l = ws.length;
        gs.push(tag(R, k++));
        cs.push(tag(A, l));

        for (String w : ws) {

          // head or body subterm starts here

          if (1 == w.length()) {
            w = "c:" + w;
          }

          final String L = w.substring(2);

          switch (w.charAt(0)) {
            case 'c':
              cs.push(encode(C, L));
              k++;
            break;
            case 'n':
              cs.push(encode(N, L));
              k++;
            break;
            case 'v': {
              IntStack Is = refs.get(L);
              if (null == Is) {
                Is = new IntStack();
                refs.put(L, Is);
              }
              Is.push(k);
              cs.push(tag(BAD, k)); // just in case we miss this
              k++;
            }
            break;
            case 'h': {
              IntStack Is = refs.get(L);
              if (null == Is) {
                Is = new IntStack();
                refs.put(L, Is);
              }
              Is.push(k - 1);
              cs.set(k - 1, tag(A, l - 1));
              gs.pop();
            }
            break;
            default:
              Main.pp("FORGOTTEN=" + w);
          } // end subterm
        } // end element
      } // end clause

      // linker
      final Iterator<IntStack> K = refs.values().iterator();

      while (K.hasNext()) {
        final IntStack Is = K.next();

        // finding the A among refs
        int leader = -1;
        for (final int j : Is.toArray()) {
          if (A == tagOf(cs.get(j))) {
            leader = j;

            break;
          }
        }
        if (-1 == leader) {
          // for vars, first V others U
          leader = Is.get(0);
          for (final int i : Is.toArray()) {
            if (i == leader) {
              cs.set(i, tag(V, i));
            } else {
              cs.set(i, tag(U, leader));
            }

          }
        } else {
          for (final int i : Is.toArray()) {
            if (i == leader) {
              continue;
            }
            cs.set(i, tag(R, leader));
          }
        }
      }

      final int neck = 1 == gs.size() ? cs.size() : detag(gs.get(1));
      final int[] tgs = gs.toArray();

      final Clause C = putClause(cs.toArray(), tgs, neck);

      Cs.add(C);

    } // end clause set

    */

    int ccount = Cs.size();
    vector<Clause> cls;
    for (int i = 0; i < ccount; i++)
      cls.push_back(Cs[i]);


/*
    Clause[] cls = new Clause[ccount];
    for (int i = 0; i < ccount; i++) {
      cls[i] = Cs.get(i);
    }
    */
    return cls;    
  }

   vector<int> Engine::toNums(vector<Clause> clauses) {
    
    int l = clauses.size();
    vector<int> cls;
    //int * cls = new int[l];
    for (int i = 0; i < l; i++) {
      cls.push_back(i);
      //cls[i] = i;
    }
    return cls;
    
  }

  /*
   * encodes string constants into symbols while leaving
   * other data types untouched
   */

  
  int Engine::encode(int t, string s) {
    int w;

    bool allDigits = true;
    
    for(const char * c = s.c_str(); *c; c++)
    {
      if(!isdigit(*c))
      {
        allDigits = false;
        break;
      }
    }

    if(allDigits)
      w = atoi(s.c_str());
    else
    {
      if(C == t)
        w = addSym(s);
      else
        return tag(BAD, 666);
    }



    /*
    try {
      w = Integer.parseInt(s);
    } catch (final Exception e) {
      if (C == t) {
        w = addSym(s);
      } else
        //pp("bad in encode=" + t + ":" + s);
        return tag(BAD, 666);
    }
    */
    return tag(t, w);
  }
  

  /**
   * true if cell x is a variable
   * assumes that variables are tagged with 0 or 1
   */


   bool Engine::isVAR( int x) {
    return tagOf(x) < 2;
  }
  

  /**
   * returns the heap cell another cell points to
   */

  
  int Engine::getRef(int x) {
    return heap[detag(x)];
  }
  

  /*
   * sets a heap cell to point to another one
   */

  void Engine::setRef(int w, int r) {
    heap[detag(w)] = r;
  }
  

  /**
   * removes binding for variable cells
   * above savedTop
   */

  
  void Engine::unwindTrail(int savedTop) {
    while (savedTop < ((int)trail.size())-1) {
      int href = trail.top();
      trail.pop();
      //final int href = trail.pop();
      // assert href is var

      setRef(href, href);
    }
  }
  

  /**
   * scans reference chains starting from a variable
   * until it points to an unbound root variable or some
   * non-variable cell
   */


  int Engine::deref(int x) {
    while (isVAR(x)) {
      int r = getRef(x);
      if (r == x) {
        break;
      }
      x = r;
    }
    return x;
  }


  /**
   * raw display of a term - to be overridden
   */

  string Engine::showTerm(int x) {
    return showTerm(exportTerm(x));
  }
  

  /**
   * raw display of a externalized term
   */


  // todo

    string Engine::showTerm(Object *O) {
      return O->show();
    /*
    if (O instanceof Object[])
      return Arrays.deepToString((Object[]) O);
    return O.toString();
    */
  }
  

  /**
   * prints out content of the trail
   */

  /*
  void ppTrail() {
    for (int i = 0; i <= trail.size()-1; i++) {
      final int t = trail.get(i);
      Main.pp("trail[" + i + "]=" + showCell(t) + ":" + showTerm(t));
    }
  }
  */

  /**
   * builds an array of embedded arrays from a heap cell
   * representing a term for interaction with an external function
   * including a displayer
   */

  
  Object * Engine::exportTerm(int x) {
    x = deref(x);

    int t = tagOf(x);
    int w = detag(x);

    Object * res = NULL;
    switch (t) {
      case C:
        res = new OString(getSym(w));
      break;
      case N:
        res = new Integer(w);
      break;
      case V:
        //case U:
        res = new OString("V" + w);
      break;
      case R: {

        int a = heap[w];
        if (A != tagOf(a))
          return new OString("*** should be A, found=" + showCell(a));
        int n = detag(a);


        vector<Object *> vec;
  //      pObject * arr = new pObject[n];
        int k = w + 1;
        for (int i = 0; i < n; i++) {
          int j = k + i;
          vec.push_back(exportTerm(heap[j]));
          //arr[i] = exportTerm(heap[j]);
        }
        res = new OComplex(vec);
//        res = arr;
      }
      break;
      default:
        res = new OString("*BAD TERM*" + showCell(x));
    }
    return res;
  }


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
  
  string Engine::showCell(int w) {
    int t = tagOf(w);
    int val = detag(w);
    string s;
    switch (t) {
      case V:
        s = "v:" + val;
      break;
      case U:
        s = "u:" + val;
      break;
      case N:
        s = "n:" + val;
      break;
      case C:
        s = "c:" + getSym(val);
      break;
      case R:
        s = "r:" + val;
      break;
      case A:
        s = "a:" + val;
      break;
      default:
        s = "*BAD*=" + w;
    }
    return s;
  }
  

  /**
   * a displayer for cells
   */

/*
  String showCells(final int base, final int len) {
    final StringBuffer buf = new StringBuffer();
    for (int k = 0; k < len; k++) {
      final int instr = heap[base + k];

      buf.append("[" + (base + k) + "]");
      buf.append(showCell(instr));
      buf.append(" ");
    }
    return buf.toString();
  }

  String showCells(final int[] cs) {
    final StringBuffer buf = new StringBuffer();
    for (int k = 0; k < cs.length; k++) {
      buf.append("[" + k + "]");
      buf.append(showCell(cs[k]));
      buf.append(" ");
    }
    return buf.toString();
  }
*/

  /**
  * to be overridden as a printer of a spine
  */
  void Engine::ppc(Spine C) {
    // override
  }

  /**
   * to be overridden as a printer for current goals
   * in a spine
   */

  /*
  void ppGoals(final IntList gs) {
    // override
  }
  */

  /**
   * to be overriden as a printer for spines
   */
  /*
  void ppSpines() {
    // override
  }
  */

  /**
   * unification algorithm for cells X1 and X2 on ustack that also takes care
   * to trail bindigs below a given heap address "base"
   */

  bool Engine::unify(int base) {
    while (!ustack.empty()) {
      int x1 = deref(ustack.top());
      ustack.pop();
      int x2 = deref(ustack.top());
      ustack.pop();
      if (x1 != x2) {
        int t1 = tagOf(x1);
        int t2 = tagOf(x2);
        int w1 = detag(x1);
        int w2 = detag(x2);

        if (isVAR(x1)) { // unb. var. v1 
          if (isVAR(x2) && w2 > w1) { // unb. var. v2 
            heap[w2] = x1;
            if (w2 <= base) {
              trail.push(x2);
            }
          } else { // x2 nonvar or older
            heap[w1] = x2;
            if (w1 <= base) {
              trail.push(x1);
            }
          }
        } else if (isVAR(x2)) { // x1 is NONVAR 
          heap[w2] = x1;
          if (w2 <= base) {
            trail.push(x2);
          }
        } else if (R == t1 && R == t2) { // both should be R
          if (!unify_args(w1, w2))
            return false;
        } else
          return false;
      }
    }
    return true;
  }

  bool Engine::unify_args(int w1, int w2) {
    int v1 = heap[w1];
    int v2 = heap[w2];
    // both should be A
    int n1 = detag(v1);
    int n2 = detag(v2);
    if (n1 != n2)
      return false;
    int b1 = 1 + w1;
    int b2 = 1 + w2;
    for (int i = n1 - 1; i >= 0; i--) {
      int i1 = b1 + i;
      int i2 = b2 + i;
      int u1 = heap[i1];
      int u2 = heap[i2];
      if (u1 == u2) {
        continue;
      }
      ustack.push(u2);
      ustack.push(u1);
    }
    return true;
  }
  

  /**
   * places a clause built by the Toks reader on the heap
   */

  
  Clause Engine::putClause(vector <int> cs, vector<int> gs, int neck) {
    int base = size();
    int b = tag(V, base);
    int len = cs.size();
    pushCells(b, 0, len, cs);
    for (int i = 0; i < gs.size(); i++) {
      gs[i] = relocate(b, gs[i]);
    }
    vector<int> xs = getIndexables(gs[0]);
    return Clause(len, gs, base, neck, xs);
  }
  

  /**
   * relocates a variable or array reference cell by b
   * assumes var/ref codes V,U,R are 0,1,2
   */
  
   int Engine::relocate(int b, int cell) {
    return tagOf(cell) < 3 ? cell + b : cell;
  }


  /**
   * pushes slice[from,to] of array cs of cells to heap
   */
  
  void Engine::pushCells(int b, int from, int to, int base) {
    ensureSize(to - from);
    for (int i = from; i < to; i++) {
      push(relocate(b, heap[base + i]));
    }
  }
  

  /**
   * pushes slice[from,to] of array cs of cells to heap
   */
  
  void Engine::pushCells(int b, int from, int to, vector<int> & cs) {
    ensureSize(to - from);
    for (int i = from; i < to; i++) {
      push(relocate(b, cs[i]));
    }
  }


  /**
   * copies and relocates head of clause at offset from heap to heap
   */
  
  int Engine::pushHead(int b, Clause & C) {
    pushCells(b, 0, C.neck, C.base);
    int head = C.hgs[0];
    return relocate(b, head);
  }
  

  /**
   * copies and relocates body of clause at offset from heap to heap
   * while also placing head as the first element of array gs that
   * when returned contains references to the toplevel spine of the clause
   */
  
  vector<int> Engine::pushBody(int b, int head, Clause &C) {
    pushCells(b, C.neck, C.len, C.base);
    int l = C.hgs.size();
    vector<int> gs;
    gs.push_back(head);
    for (int k = 1; k < l; k++) {
      int cell = C.hgs[k];
      gs.push_back(relocate(b, cell));
    }
    return gs;
  }
  

  /**
   * makes, if needed, registers associated to top goal of a Spine
   * these registers will be reused when matching with candidate clauses
   * note that xs contains dereferenced cells - this is done once for
   * each goal's toplevel subterms
   */
  
  void Engine::makeIndexArgs(Spine * G, int goal) {
    
    if (NULL != G->xs)
      return;
      

    int p = 1 + detag(goal);
    int n = min(MAXIND, detag(getRef(goal)));

    vector<int> * xs = new vector<int>(MAXIND);
    for (int i = 0; i < n; i++) {
      int cell = deref(heap[p + i]);
      (*xs)[i] = cell2index(cell);
    }
/*
    int[] xs = new int[MAXIND];

    for (int i = 0; i < n; i++) {
      int cell = deref(heap[p + i]);
      xs[i] = cell2index(cell);
    }*/

    G->xs = xs;

    if (NULL == imaps)
      return;
    vector<int> * cs = IMap<int>::get(imaps, vmaps, *xs);
    G->cs = *cs;
    delete cs;
  }

  vector<int> Engine::getIndexables(int ref) {
     int p = 1 + detag(ref);
     int n = detag(getRef(ref));
     vector<int> xs(MAXIND);
    for (int i = 0; i < MAXIND && i < n; i++) {
      int cell = deref(heap[p + i]);
      xs[i] = cell2index(cell);
    }
    return xs;
  }

  int Engine::cell2index(int cell) {
    int x = 0;
    int t = tagOf(cell);
    switch (t) {
      case R:
        x = getRef(cell);
      break;
      case C:
      case N:
        x = cell;
      break;
      // 0 otherwise - assert: tagging with R,C,N <>0
    }
    return x;
  }


  /**
   * tests if the head of a clause, not yet copied to the heap
   * for execution could possibly match the current goal, an
   * abstraction of which has been place in xs
   */
  
  bool Engine::match(vector<int> *xs, Clause & C0) {
    for (int i = 0; i < MAXIND; i++) {
      int x = (*xs)[i];
      int y = C0.xs[i];
      if (0 == x || 0 == y) {
        continue;
      }
      if (x != y)
        return false;
    }
    return true;
  }
  

  /**
   * transforms a spine containing references to choice point and
   * immutable list of goals into a new spine, by reducing the
   * first goal in the list with a clause that successfully
   * unifies with it - in which case places the goals of the
   * clause at the top of the new list of goals, in reverse order
   */

    Spine *Engine::unfold(Spine * G) {

	int ttop = trail.size() - 1;
    int htop = getTop();
    int base = htop + 1;

    int goal = G->gs[0];
    //IntList.head(G.gs);

    makeIndexArgs(G, goal);

    int last = G->cs.size();
    for (int k = G->k; k < last; k++) {
      Clause C0 = clauses[G->cs[k]];

      if (!match(G->xs, C0))
        continue;

      int base0 = base - C0.base;
      int b = tag(V, base0);
      int head = pushHead(b, C0);

      ustack = my_stack(); // set up unification stack

      ustack.push(head);
      ustack.push(goal);

      if (!unify(base)) {
        unwindTrail(ttop);
        setTop(htop);
        continue;
      }
      vector<int> gs = pushBody(b, head, C0);

      vector<int> newgs;
      for(int i = 1; i < gs.size(); i++)
        newgs.push_back(gs[i]);
      for(int i = 1; i < G->gs.size(); i++)
        newgs.push_back(G->gs[i]);


      //IntList newgs = IntList.tail(IntList.app(gs, IntList.tail(G.gs)));

      vector<int> tail;
      for(int i = 1; i < G->gs.size(); i++)
        tail.push_back(G->gs[i]);

      G->k = k + 1;
      if (!newgs.empty())
        return new Spine(gs, base, tail, ttop, 0, cls);
      else
        return answer(ttop);
    } // end for
    return NULL;
  }


  /**
   * extracts a query - by convention of the form
   * goal(Vars):-body to be executed by the engine
   */
  
  Clause Engine::getQuery() {
    return clauses[clauses.size() - 1];
  }
  

  /**
   * returns the initial spine built from the
   * query from which execution starts
   */
  
  Spine * Engine::init() {
    int base = size();

    Clause G = getQuery();
    vector<int> empty;
    Spine *Q = new Spine(G.hgs, base, empty, trail.size()-1, 0, cls);
    spines.push(Q);
    return Q;
  }
  

  /**
   * returns an answer as a Spine while recording in it
   * the top of the trail to allow the caller to retrieve
   * more answers by forcing backtracking
   */
  
  Spine * Engine::answer( int ttop) {
    return new Spine(spines.c[0]->hd, ttop);
//    return new Spine(spines[0].hd, ttop);
  }
  

  /**
   * detects availability of alternative clauses for the
   * top goal of this spine
   */
  
  bool Engine::hasClauses(Spine *S) {
    return S->k < S->cs.size();
  }
  

  /**
   * true when there are no more goals left to solve
   */

  
  bool Engine::hasGoals(Spine * S) {
    return !S->gs.empty();
  }
  

  /**
   * removes this spines for the spine stack and
   * resets trail and heap to where they where at its
   * creating time - while undoing variable binding
   * up to that point
   */

  
   void Engine::popSpine() {
    Spine *G = spines.top();
    spines.pop();
    unwindTrail(G->ttop);
    setTop(G->base - 1);
  }
  

  /**
   * main interpreter loop: starts from a spine and works
   * though a stream of answers, returned to the caller one
   * at a time, until the spines stack is empty - when it
   * returns null
   */

  
  Spine * Engine::yield() {
    while (!spines.empty()) {
      Spine *G = spines.top();
      if (!hasClauses(G)) {
        popSpine(); // no clauses left
        continue;
      }
      Spine *C = unfold(G);
      if (NULL == C) {
        popSpine(); // no matches
        continue;
      }
      if (hasGoals(C)) {
        spines.push(C);
        continue;
      }
      return C; // answer
    }
    return NULL;
  }


  /**
   * retrieves an answers and ensure the engine can be resumed
   * by unwinding the trail of the query Spine
   * returns an external "human readable" representation of the answer
   */

  
  Object * Engine::ask() {
    Spine * tmp = yield();
    //delete tmp;
    if (NULL == tmp)
      return NULL;
    query = tmp;


    Spine * tmp2 = answer(query->ttop);
    int res = tmp2->hd;
    delete tmp2;

    //int res = answer(query.ttop).hd;
    Object * R = exportTerm(res);
    unwindTrail(query->ttop);
    return R;
  }
  

  /**
   * initiator and consumer of the stream of answers
   * generated by this engine
   */
  
  void Engine::run() {
    int ctr = 0;
    for (;; ctr++) {
      Object *A = ask();
      if (NULL == A) {
        break;
      }
      if(ctr<5) 
      {
        char buffer[512];
		string s = showTerm(A);
        sprintf(buffer,"[%d]]*** ANSWER = %s", ctr, s.c_str());
        Mainpp(buffer);
      }
      //char buffer[512];
      //sprintf(buffer,"[%l]]*** ANSWER = %s", ctr, showTerm(A));
     // if(ctr<5) Mainpp("[" + ctr + "] " + "*** ANSWER=" + showTerm(A));
    }
    if(ctr>5) Mainpp("...");
	char buff[128];
	sprintf(buff, "TOTAL ANSWERS=%d", ctr);
    Mainpp(buff);
  }

  // indexing extensions - ony active if START_INDEX clauses or more


  vector<IntMap> Engine::vcreate(int l) {
    vector <IntMap> vss(l); // = new IntMap[l];
    /*
    for (int i = 0; i < l; i++) {
      vss[i] = new IntMap();
    }
    */
    return vss;
  }

  void Engine::put(vector<IMap<int> > imaps, vector<IntMap> vss, vector<int> keys, int val) {
    for (int i = 0; i < imaps.size(); i++) {
       int key = keys[i];
      if (key != 0) {
        IMap<int>::put(imaps, i, key, val);
      } else {
        vss[i].add(val);
      }
    }
  }
  
  vector<IMap<int> > * Engine::index(vector<Clause> clauses, vector<IntMap> vmaps) {
    if (clauses.size() < START_INDEX)
      return NULL;

    vector<IMap<int> > *imaps = IMap<int>::create(vmaps.size());
    for (int i = 0; i < clauses.size(); i++) {
      Clause c = clauses[i];

      put(*imaps, vmaps, c.xs, i + 1); // $$$ UGLY INC

    }
    Mainpp("INDEX");
    Mainpp(IMap<int>::show(*imaps));
    for(IntMap m : vmaps)
    {
      Mainpp(m.show());
    }
    Mainpp("");
    return imaps;
  }
  


/*
int main()
{
  Engine P("fname");
}
*/
