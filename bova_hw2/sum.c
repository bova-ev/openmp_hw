#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

//NOTE!!!

//Additional task "Pass "N" through arguments" is implemented.
//Additional task "Use reduction and schedule" is implemented.

int main(int argc, char* argv[])
{
    //"sum" stands for sum of N natural numbers.
    //"i" is counter for cycle.
    int N = 0;
    int sum = 0;
    int i = 0;

    //If there are no arguments set N to 100.

    if(argv[1] == NULL)
        N = 100;
    else
        N = atoi(argv[1]);

    //Using sum as parameter of reduction to perform distributed
    //calculation of sum with all threads. Schedule static distributes
    //calculations among threads automatically.

    #pragma omp parallel reduction(+:sum)
    {
        #pragma omp for schedule(static)
        for(i = 1; i <= N; i++)
            sum += i;
    }

    printf("Sum of first %d natural numbers is %d\n", N, sum);

    return 0;
}

