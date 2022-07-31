#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int nthreads, i, tid;
    float total;

    /**SOLUTION:
     * All variables except "i" should be private because otherwise each thread
     * tries to access the same "total" instance and "tid" instance. That's why
     * last print prints same thread number (which is number of thread last wrote
     * to "tid".
     */
    //#pragma omp parallel
    #pragma omp parallel private(total, nthreads, tid) shared(i)
    {
        tid = omp_get_thread_num();

        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

        /* Here we want to print when threads start calculating total. So we put
         * barrier before print.
         */
        //printf("Thread %d is starting...\n", tid);
        
        #pragma omp barrier
        printf("Thread %d is starting...\n", tid);

        total = 0.0;

        #pragma omp for schedule(dynamic, 10)
        for (i = 0; i < 1000000; i++)
            total = total + i*1.0;

        printf ("Thread %d is done! Total= %e\n", tid, total);
    }
}
