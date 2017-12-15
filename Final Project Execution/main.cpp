#include <stdio.h>
#include <string>
#include "Engine.h"
#include "prog.h"

using namespace std;

void run(string fname0)
{
    bool p = true;

    string fname = fname0 + ".nl";
    Prog P(fname);
	printf("CODE\n");
	P.ppCode();

	printf("RUNNING\n");
    P.run();
}

  int main(int argc, char ** args) {
    if(argc != 2)
    {
      printf("Usage: %s fname\n", args[0]);
    }
    else
    {
      run(args[1]);      
    }
	return 0;
  }


/*
public class Main {

  static void println(final Object o) {
    System.out.println(o);
  }

  static void pp(final Object o) {
    System.out.println(o);
  }

  public static void run(final String fname0) {
    final boolean p = true;

    final String fname = fname0 + ".nl";
    Engine P;

    if (p) {
      P = new Prog(fname);
      pp("CODE");
      ((Prog) P).ppCode();
    } else {
      P = new Engine(fname);
    }

    pp("RUNNING");
    final long t1 = System.nanoTime();
    P.run();
    final long t2 = System.nanoTime();
    System.out.println("time=" + (t2 - t1) / 1000000000.0);

  }



  public static void srun(final String fname0) {
    final String fname = fname0 + ".nl";
    final Prog P = new Prog(fname);

    pp("CODE");
    P.ppCode();

    pp("RUNNING");
    final long t1 = System.nanoTime();

    final Stream<Object> S = P.stream();
    S.forEach(x -> Main.pp(P.showTerm(x)));

    final long t2 = System.nanoTime();
    System.out.println("time=" + (t2 - t1) / 1000000000.0);
  }

  public static void main(final String[] args) {
		String fname=args[0];
    run(fname);
  }
}
*/