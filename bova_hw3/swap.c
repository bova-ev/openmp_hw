#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

//NOTE!!!

//Additional task "Change initial array without creating a new one"
//is implemented.

int main(int argc, char* argv[])
{
    //"i"          - Variable for cycle.
    //"id"         - Thread's id.
    //"size"       - Size of the initial array.
    //"offset"     - Index of the element that ith thread have to remember
    //               before (i+1)th thread changes it (element).
    //"threads"    - Number of threads.
    //"block_size" - Size of block that every thread recieves.
    int i = 0;
    int id = 0;
    int size = 100000;
    int offset = 0;
    int threads = 0;
    int block_size = 0;

    //"left"   - Value of (i-1)th element. Used as buffer.
    //"middle" - Value of ith element. Used as buffer.
    //"buf"    - Contains value of the first element of (i+1)th thread's block
    //           for ith block. Used for final computation of ith thread.
    float left = 1.0;
    float middle = 1.0;
    float buf = 1.0;

    //This code generates initial array for this problem.
    float a[size];
    for(i = 0; i < size; i++)
        a[i] = (float)(i + 1);

    #pragma omp parallel private(id, offset, threads, left, middle, buf) \
     shared(a)
    {
        id = omp_get_thread_num();
        threads = omp_get_num_threads();

        //With static scheduling we distribute work among threads in very
        //certain order. This variable helps to compute size of the block
        //every thread gets.
        block_size = (int)(size / threads) + (size % threads != 0);

        //Now we compute index of element that ith thread needs to know from
        //(i+1)th thread before (i+1)th changes value of this element. In "buf"
        //we store value of this element. 
        offset = block_size * (id + 1);
        buf = ((offset < size) ? a[offset] : 1.0);

        //Before any computations we need to remember value of element that is
        //left to the one each thread starts from. In addition, we check if
        //we can reach the element we are looking for.
        left = (((block_size * id) - 1 >= 0) ? a[(block_size * id) - 1] : 1.0);

        #pragma omp for schedule(static, block_size) 
        for(i = 0; i < size; i++)
        {    
            middle = a[i];

            //Checking if we are trying to change first or last element.
            if(i - 1 > 0 && i + 1 < size)
            {
                //Checking if we hit the end of the block.
                if(i != offset - 1)
                    a[i] = (left * a[i] * a[i + 1]) / 3.0;
                else
                    a[i] = (left * a[i] * buf) / 3.0;
            }

            left = middle;
        }
    }
    return 0;
}

