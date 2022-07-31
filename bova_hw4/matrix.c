#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

//Frees allocated matrix
void free_matrix(int **matrix, int N)
{
    int i = 0;

    for(i = 0; i < N; i++)
        free(matrix[i]);

    free(matrix);
}

//Sets matrix elements to zero
void null_matrix(int **matrix, int N)
{
    int i = 0;
    int j = 0;

    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            matrix[i][j] = 0;
}

//Matrix print for debugging
void print_matrix(int **matrix, int N)
{
    int i = 0;
    int j = 0;

    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
            printf("%2d ", matrix[i][j]);
        
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    //"N"    - Size of one dimension of matrix
    //"flag" - Shows number of mismatches between "parallel" and
    //         "sequential" matrices
    int N = 1000;
    int i = 0;
    int j = 0;
    int k = 0;
    int flag = 0;

    double start = 0;
    double finish = 0;

    //In this block we inintialize matrices
    int **A;
    int **B;
    int **C;
    int **control;

    A = (int**)calloc(N, sizeof(int*));
    B = (int**)calloc(N, sizeof(int*));
    C = (int**)calloc(N, sizeof(int*));
    control = (int**)calloc(N, sizeof(int*));

    for(i = 0; i < N; i++)
    {
        A[i] = (int*)calloc(N, sizeof(int));
        B[i] = (int*)calloc(N, sizeof(int));
        C[i] = (int*)calloc(N, sizeof(int));
        control[i] = (int*)calloc(N, sizeof(int));
    }

    srand(time(NULL));
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    null_matrix(C, N);
    null_matrix(control, N);

    //Here we calculate resulting matrix sequentially and measure
    //time of these calculations
    start = omp_get_wtime();

    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            for(k = 0; k < N; k++)
                control[i][j] += A[i][k] * B[k][j];

    finish = omp_get_wtime();
    printf("Sequential execution time: %.3f\n", finish - start);

    //Here we calculate resulting matrix in parallel. "collapse(2)"
    //distributes calculation of every C[i][j] among threads.
    start = omp_get_wtime();

    #pragma omp parallel shared(A, B, C, N, control) private(i, j, k)
    {
        #pragma omp for schedule(static) collapse(2)
        for(i = 0; i < N; i++)
            for(j = 0; j < N; j++)
            {
                for(k = 0; k < N; k++)
                    C[i][j] += A[i][k] * B[k][j];

                if(C[i][j] != control[i][j])
                    flag++;
            }
    }

    finish = omp_get_wtime();
    printf("Parallel execution time: %.3f\n", finish - start);


    if(!flag)
        printf("No errors in parallel algorythm occured.\n");
    else
        printf("%d errors occured in parallel algorythm.\n", flag);


    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C, N);
    free_matrix(control, N);

    return 0;
}

