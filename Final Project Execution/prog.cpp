#include "prog.h"


	Prog::Prog(string fname) : Engine(fname) {
	}

	string Prog::showTerm(Object & O)
	{
		if (O.type == 3)
		{
			return st0((OComplex*&)O);
		}

		if (O.type == 2) return ((OString*)&O)->s;
		if (O.type == 1)
		{
			return ((Integer*)&O)->show();
		}
	}

	bool Prog::isListCons(string sr) {
		//	string sr = name->show();
		return sr == "." || sr == "[|]" || sr == "list";
		//return ".".equals(name) || "[|]".equals(name) || "list".equals(name);
	}

	string Prog::maybeNull(Object * O) {
		if (NULL == O)
			return "$null";
		if (O->type == 3)
			return st0((OComplex *)O);
		return O->show();
	}


	bool Prog::isOp(string  sr) {
		//string sr = name->show();
		return sr == "/" || sr == "-" || sr == "+" || sr == "=";
		//return "/".equals(name) || "-".equals(name) || "+".equals(name) || "=".equals(name);
	}



	 string Prog::st0(OComplex * args) {
		string buf;

		string name = args->v[0]->show();
		if (args->v.size() == 3 && isOp(name)) {
			buf += "(";
			buf += maybeNull(args->v[0]);
			buf += " ";
			buf += name;
			buf += " ";
			buf += maybeNull(args->v[1]);
			buf += ")";
		}
		else if (args->v.size() == 3 && isListCons(name)) {
			buf += "[";
			{
				buf += maybeNull(args->v[1]);
				Object * tail = args->v[2];
				for (;;) {

					if (tail->show() == "[]" || tail->show() == "nil") break;

					if (!tail->type == 3)
					{
						buf += "|";
						buf += maybeNull(tail);
						break;
					}

					OComplex * list = (OComplex *)tail;

					if (!(list->v.size() == 3 && isListCons(list->v[0]->show()))) {
						buf += "|";
						buf += maybeNull(tail);
						//buf.append('|');
						//buf.append(maybeNull(tail));
						break;
					}
					else {
						//if (i > 1)
						buf += ",";
						buf += maybeNull(list->v[1]);
						//buf.append(',');
						//buf.append(maybeNull(list->v[1]));
						tail = list->v[2];
					}
				}
			}
			buf += "]";
		}
		else if (args->v.size() == 2 && name == "$VAR") {
			buf += "_";
			buf += args->v[1]->show();
		}
		else {
			string qname = maybeNull(args->v[0]);
			buf.append(qname);
			buf.append("(");
			for (int i = 1; i < args->v.size(); i++) {
				Object * O = args->v[i];
				buf.append(maybeNull(O));
				if (i < args->v.size() - 1) {
					buf += ",";
				}
			}
			buf.append(")");
		}
		return buf;
	}

	void Prog::ppCode() {
		printf("\nSYMS:\n");
		print_syms();
		//printf(syms);
		printf("\nCLAUSES:\n");

		for (int i = 0; i < clauses.size(); i++) {

			Clause C = clauses[i];
			printf("[%d]:%s\n", i, showClause(C).c_str());
			//pp("[" + i + "]:" + showClause(C));
		}
		printf("\n");
		//pp("");
	}


	string Prog::showClause(Clause &  s) {
		string buf;
		int l = s.hgs.size();
		char buff[512];
		sprintf(buff, "---base:[%d] neck: %d-----\n%s\n%s :- [", s.base, s.neck,
			showCells(s.base, s.len).c_str(),
			showCell(s.hgs[0]).c_str());

		buf += buff;

		//buf.append("---base:[" + s.base + "] neck: " + s.neck + "-----\n");
		//buf.append(); // TODO
		//buf.append("\n");
		//buf.append(showCell(s.hgs[0]));

		//buf.append(" :- [");
		for (int i = 1; i < l; i++) {

			int e = s.hgs[i];
			buf.append(showCell(e));
			if (i < l - 1) {
				buf.append(", ");
			}
		}

		buf.append("]\n");

		//Integer shgs();

		//;

		buf.append(((Engine *)this)->showTerm(s.hgs[0]));
		if (l > 1) {
			buf.append(" :- \n");
			for (int i = 1; i < l; i++) {
				int e = s.hgs[i];
				buf.append("  ");
				Integer eint(e);
				buf.append(((Engine *)this)->showTerm(e));
				buf.append("\n");
			}
		}
		else {
			buf.append("\n");
		}
		return buf;
	}

