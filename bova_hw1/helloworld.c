#include<stdio.h>
#include<omp.h>

//NOTE!!!

//Additional task "Print thread names in reversed
//order" is implemented.

int main(int argc, char* argv[])
{
    //"myid" stands for name of thread
    //"i" is counter for cycle
    
    int myid = -1;
    int i = 0;

    //Parallel secton starts below. Variables "myid" 
    //and "i" are private because otherwise threads 
    //start to change them in unpredictable order, 
    //thus killing structural output.

    #pragma omp parallel private(i, myid)
    {
        myid = omp_get_thread_num();

        //In this loop every thread counts from the
        //highest thread id to zero, and if counter
        //"i" is equal to id of thread, thread prints
        //hello message. Barrier is used to sync
        //threads after each iteration of for cycle.

        for(i = omp_get_num_threads() - 1; i >= 0; i--)
        {
            if(i == myid)
                printf("I'm process %d!\n", myid);

            #pragma omp barrier
        }
    }

    return 0;
}

