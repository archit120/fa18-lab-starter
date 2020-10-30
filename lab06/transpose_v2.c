#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int min(int x, int y)
{
    return x > y ? y : x;
}


/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src)
{
    int blocks = (n + blocksize - 1) / blocksize;
    int rem = n % blocksize;
    for (int i = 0; i < blocks; i++)
    {
        for (int j = 0; j < blocks; j++)
        {
            int sx = blocksize * i, sy = blocksize * j;
            for (int dx = sx; dx < min(n, sx + blocksize); dx++)
            {
                for (int dy = sy; dy < min(n, sy + blocksize); dy++)
                {
                    dst[dy + dx * n] = src[dx + dy * n];
                    // printf("%d %d\n", dx, dy);
                }
            }
        }
    }
}


// This algorithm has more spatial locality compared to previous one but there are more copy instructions
// As a consequence its slower for smaller blocksizes than algo 1 but faster at bigger blocks.
void transpose_blocking(int n, int blocksize, int *dst, int *src)
{
    // YOUR CODE HERE
    int *dst2 = (int *)malloc(blocksize * blocksize * sizeof(int));
    int blocks = (n + blocksize - 1) / blocksize;
    int rem = n % blocksize;
    for (int i = 0; i < blocks; i++)
    {
        for (int j = 0; j < blocks; j++)
        {
            int sx = blocksize * i, sy = blocksize * j;
            for (int dy = sy; dy < min(n, sy + blocksize); dy++)
            {

                for (int dx = sx; dx < min(n, sx + blocksize); dx++)
                {
                    dst2[dy - sy + (dx - sx) * blocksize] = src[dx + dy * n];
                    // printf("%d %d\n", dx, dy);
                }
            }

            for (int dx = sx; dx < min(n, sx + blocksize); dx++)
            {

                for (int dy = sy; dy < min(n, sy + blocksize); dy++)
                {
                    dst[dy + dx * n] = dst2[dy - sy + (dx - sx) * blocksize];
                }
            }
        }
    }

    free(dst2);
}

void transpose_blocking_v2(int n, int blocksize, int *dst, int *src)
{
    // YOUR CODE HERE
}

void benchmark(int *A, int *B, int n, int blocksize,
               void (*transpose)(int, int, int *, int *), char *description)
{

    int i, j;
    printf("Testing %s: ", description);

    /* initialize A,B to random integers */
    // srand( time( NULL ) );
    for (i = 0; i < n * n; i++)
        A[i] = rand();
    for (i = 0; i < n * n; i++)
        B[i] = rand();

    /* measure performance */
    struct timeval start, end;

    gettimeofday(&start, NULL);
    transpose(n, blocksize, B, A);
    gettimeofday(&end, NULL);

    double seconds = (end.tv_sec - start.tv_sec) +
                     1.0e-6 * (end.tv_usec - start.tv_usec);
    printf("%g milliseconds\n", seconds * 1e3);

    /* check correctness */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (B[j + i * n] != A[i + j * n])
            {
                printf("Error!!!! Transpose does not result in correct answer!!\n");
                exit(-1);
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: transpose <n> <blocksize>\nExiting.\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int blocksize = atoi(argv[2]);

    /* allocate an n*n block of integers for the matrices */
    int *A = (int *)malloc(n * n * sizeof(int));
    int *B = (int *)malloc(n * n * sizeof(int));

    /* run tests */
    benchmark(A, B, n, blocksize, transpose_naive, "naive transpose");
    benchmark(A, B, n, blocksize, transpose_blocking, "transpose with blocking");

    /* release resources */
    free(A);
    free(B);
    return 0;
}
