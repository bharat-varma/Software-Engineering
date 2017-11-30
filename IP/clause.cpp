#include <cstdio>
#include <vector>
#include <cstdlib>

using namespace std;

/**
 * representation of a clause
 */
class Clause {

public:

  Clause(int len,  vector<int> & hgs, int base,  int neck, vector<int> xs);

  int len;
  vector<int> hgs;
  int base;
  int neck;
  vector<int> xs;
  //  int * xs;
};

  Clause::Clause(int len,  vector<int> & hgs, int base,  int neck,  vector<int> xs) {
    this->hgs = hgs; // head+goals pointing to cells in cs
    this->base = base; // heap where this starts
    this->len = len; // length of heap slice
    this->neck = neck; // first after the end of the head
    this->xs = xs; // indexables in head
  }



#define IA 16807
#define IM 2147483647
#define IQ 127773
#define IR 2836
#define NTAB 32
#define EPS (1.2E-07)
#define MAX(a,b) (a>b)?a:b
#define MIN(a,b) (a<b)?a:b

double ran1(int *idum)
{
    int j,k;
	static int iv[NTAB],iy=0;
	void nrerror();
        static double NDIV = 1.0/(1.0+(IM-1.0)/NTAB);
        static double RNMX = (1.0-EPS);
        static double AM = (1.0/IM);

	if ((*idum <= 0) || (iy == 0)) {
		*idum = MAX(-*idum,*idum);
                for(j=NTAB+7;j>=0;j--) {
			k = *idum/IQ;
			*idum = IA*(*idum-k*IQ)-IR*k;
			if(*idum < 0) *idum += IM;
			if(j < NTAB) iv[j] = *idum;
		}
		iy = iv[0];
	}
	k = *idum/IQ;
	*idum = IA*(*idum-k*IQ)-IR*k;
	if(*idum<0) *idum += IM;
	j = iy*NDIV;
	iy = iv[j];
	iv[j] = *idum;
	return MIN(AM*iy,RNMX);
}

int random_int(int *seed, int a, int b)
{
	return (b-a)*ran1(seed)+a;
}

  int main(int argc, char **argv)
  {

  	  int seed = -1;

  	  if(argc>1)
  	  {
  	  	seed = atoi(argv[1]);
  	  	if(seed > 0) seed = -seed;
  	  }

	  vector<int> hgs, xs;

	  int hgs_size = random_int(&seed, 3, 6);
	  for(int i=0;i<hgs_size;i++)
	  	hgs.push_back(random_int(&seed,1,10));

	  xs.push_back(-1);
	  Clause C(random_int(&seed,5,15),hgs,0,-1,xs);

	  printf("Len: %d\n", C.len);
	  printf("Base: %d\n", C.base);
	  printf("Neck: %d\n", C.neck);
	  printf("hgs:\n");
	  for (int x : C.hgs) printf("%d ", x);
	  printf("\nxs:\n");
	  for (int x : C.xs) printf("%d ", x);
	  printf("\n");

	  return 0;
  }

