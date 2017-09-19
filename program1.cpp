#include <iostream>
#include <stdio.h>

using namespace std;

int main() {

        int a,n;
        int k=0,b=1;
        printf ("Enter a: ");
        scanf ("%d", &a);

        printf ("Enter n: ");
        scanf ("%d", &n);

        // calculate nth power of a
        while (k < n )
        {
        k = k + 1;
        b=b*a;
        printf("\n b is : %d\n", b);
        }
         
        printf("\n b is : %d\n", b);
        return 0;
}
