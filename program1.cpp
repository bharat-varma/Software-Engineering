#include <iostream>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

using namespace std;

int main() {
       
        int a,n;
        int k=0,b=1;
        double t;
        struct timeval s,e;
        printf("Enter a:");
        scanf("%d",&a);
        printf("Enter n:");
        scanf("%d",&n);
        gettimeofday(&s,0);
       // calculate nth power of a
        while (k < n )
        {
        k = k+1;
        b=b*a;
        printf("\n b is : %d\n", b);
        }
       gettimeofday(&e, 0);
       printf("\n The nth [power of a given number is : %d\n", b);
       //calculate time taken
       t=(e.tv_sec + e.tv_usec*0.000001) - (s.tv_sec + s.tv_usec*0.000001);
        printf("time taken : %f\n", t);
        return 0;
}
