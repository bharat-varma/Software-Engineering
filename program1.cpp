#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <sys/time.h>

using namespace std;

int main() {
       
        int a;
        int n;
        int k=0;
        int b=1;
        double t;
        struct timeval s,e;
        printf("Enter a:");
        scanf("%d",&a);
        printf("Enter n:");
        scanf("%d",&n);
        gettimeofday(&s,0);
        
       gettimeofday(&e, 0);
       printf("\n b is : %d\n", b);
       //calculate time taken
       t=(e.tv_sec + e.tv_usec*0.000001) - (s.tv_sec + s.tv_usec*0.000001);
        printf("time taken : %f\n", t);
        return 0;
}
