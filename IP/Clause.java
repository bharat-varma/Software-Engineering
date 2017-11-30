
/**
 * representation of a clause
 */
class Ran1
{
  final int IA = 16807;
  final int IM = 2147483647;
  final int  IQ = 127773;
  final int IR  = 2836;
  final int NTAB = 32;
  final double EPS =  (1.2E-07);

  int idum;
  int [] iv;
  int iy=0;

  public Ran1(int seed)
  {
    idum = seed;
    iv = new int[NTAB];
    iy = 0;
  }

public double ran1()
{
    int j,k;
         double NDIV = 1.0/(1.0+(IM-1.0)/NTAB);
         double RNMX = (1.0-EPS);
         double AM = (1.0/IM);

  if ((idum <= 0) || (iy == 0)) {
    idum = Math.max(-idum,idum);
                for(j=NTAB+7;j>=0;j--) {
      k = idum/IQ;
      idum = IA*(idum-k*IQ)-IR*k;
      if(idum < 0) idum += IM;
      if(j < NTAB) iv[j] = idum;
    }
    iy = iv[0];
  }
  k = idum/IQ;
  idum = IA*(idum-k*IQ)-IR*k;
  if(idum<0) idum += IM;
  j = (int)((double)iy*NDIV);
  iy = iv[j];
  iv[j] = idum;
  return Math.min(AM*iy,RNMX);
}

int random_int(int a, int b)
{
  return (int)((double)(b-a)*ran1())+a;
}

}

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
    int seed = -1;
    if(args.length > 0)
    {
      seed = Integer.parseInt(args[0]);
      if (seed > 0) seed *= -1;
    }

    Ran1 rnd = new Ran1(seed);

    int hgs_size = rnd.random_int(3, 6);
    int [] hgs = new int[hgs_size];

    for(int i=0;i<hgs_size;i++)
      hgs[i] = rnd.random_int(1,10);



     // int [] hgs = {0,1,2};
      int [] xs = {-1};
      Clause C = new Clause (rnd.random_int(5,15),hgs,0,-1,xs);

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
