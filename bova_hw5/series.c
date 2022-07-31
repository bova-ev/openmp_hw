#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

//Calculates factorial of number "n", returns double factorial
double factorial(int n)
{
    double factorial = 1;
    int i = 2;

    for(i = 2; i <= n; i++)
        factorial = (double)(i) * factorial;

    return factorial;
}

int main(int argc, char* argv[])
{
    //"i" - Iterative variable
    //"N" - Number of elements in series
    int i = 0;
    int N = 0;

    //"Sn" - Private sum for every thread
    //"S"  - Final series
    double Sn = 0.0;
    double S = 1.0;

    if(argv[1])
        N = atoi(argv[1]);
    else
        N = 100;

    //Distributing calculations between threads. Every thread
    //calculates private sum "Sn", then every thread adds it to 
    //final sum "S".
    #pragma omp parallel shared(S) private(i, Sn)
    {
        #pragma omp for schedule(guided)
        for(i = 1; i <= N; i++)
            Sn += 1.0 / factorial(i);

        #pragma omp critical
        {
            S += Sn;
        }
    }
    printf("%.9f\n", S);

    return 0;
}

