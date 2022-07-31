#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc, char* argv[])
{
    //"i"   - Iterational variable
    //"j"   - Iterational variable
    //"N"   - Number of numbers to check
    //"num" - Number of prime numbers
    //"div" - Variable for number of dividers of certain number
    int i = 0;
    int j = 0;
    int N = 0;
    int num = 0;
    int div = 0;

    if(argv[1])
        N = atoi(argv[1]);
    else
        N = 1000;

    //Each thread calculates number of dividers for number "i" and
    //check's if there are more dividers than 2. If (div == 2) then
    //current number "i" is prime.
    #pragma omp parallel shared(N, num) private(i, j, div)
    {
        #pragma omp for schedule(static)
        for(i = 1; i <= N; i++)
        {
            for(j = 1; j <= i; j++)
                if((i % j) == 0)
                    div++;

            #pragma omp critical
            {
                if(div == 2)
                    num++;
            }
          
            div = 0;
        }
    }
    printf("There are %d prime numbers in first %d natural numbers\n", num, N);

    return 0;
}

