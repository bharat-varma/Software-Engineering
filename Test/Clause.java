
/**
 * representation of a clause
 */
class Clause {
  Clause(final int len, final int[] hgs, final int base, final int neck, final int[] xs) {
    this.hgs = hgs; // head+goals pointing to cells in cs
    this.base = base; // heap where this starts
    this.len = len; // length of heap slice
    this.neck = neck; // first after the end of the head
    this.xs = xs; // indexables in head
  }

  final int len;
  final int[] hgs;
  final int base;
  final int neck;
  final int[] xs;
  
  
  public static void main(String [] args)
  {
      int [] hgs = {0,1,2};
      int [] xs = {-1};
      Clause C = new Clause (10,hgs,0,-1,xs);

	System.out.printf("Len: %d\n", C.len);
	  System.out.printf("Base: %d\n", C.base);
	  System.out.printf("Neck: %d\n", C.neck);
	  System.out.printf("hgs:\n");
	  for (int x : C.hgs) System.out.printf("%d ", x);
	  System.out.printf("\nxs:\n");
	  for (int x : C.xs) System.out.printf("%d ", x);
	  System.out.printf("\n");
  }
}
