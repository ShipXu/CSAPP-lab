/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    int ii, jj;
    int a0, a1, a2, a3, a4, a5, a6, a7;

    if (M == 64 && N == 64) {
        for (ii = 0; ii < N; ii += 8) {
            for (jj = 0; jj < M; jj += 8) {
                for (i = ii; i < ii + 4; i++) {
                    a0 = A[i][jj];
                    a1 = A[i][jj + 1];
                    a2 = A[i][jj + 2];
                    a3 = A[i][jj + 3];
                    a4 = A[i][jj + 4];
                    a5 = A[i][jj + 5];
                    a6 = A[i][jj + 6];
                    a7 = A[i][jj + 7];
                    B[jj][i] = a0;
                    B[jj + 1][i] = a1;
                    B[jj + 2][i] = a2;
                    B[jj + 3][i] = a3;
                    B[jj][i + 4] = a4;
                    B[jj + 1][i + 4] = a5;
                    B[jj + 2][i + 4] = a6;
                    B[jj + 3][i + 4] = a7;
                }

                for (i = ii + 4; i < ii + 8; i++) {
                    a0 = A[i][jj];
                    a1 = A[i][jj + 1];
                    a2 = A[i][jj + 2];
                    a3 = A[i][jj + 3];
                    a4 = A[i][jj + 4];
                    a5 = A[i][jj + 5];
                    a6 = A[i][jj + 6];
                    a7 = A[i][jj + 7];
                    B[jj + 4][i - 4] = a0;
                    B[jj + 4 + 1][i - 4] = a1;
                    B[jj + 4 + 2][i - 4] = a2;
                    B[jj + 4 + 3][i - 4] = a3;
                    B[jj + 4][i] = a4;
                    B[jj + 4 + 1][i] = a5;
                    B[jj + 4 + 2][i] = a6;
                    B[jj + 4 + 3][i] = a7;
                }

                for (j = jj; j < jj + 4; j++) {
                    a0 = B[j + 4][ii];
                    a1 = B[j + 4][ii + 1];
                    a2 = B[j + 4][ii + 2];
                    a3 = B[j + 4][ii + 3];
                    a4 = B[j][ii + 4];
                    a5 = B[j][ii + 5];
                    a6 = B[j][ii + 6];
                    a7 = B[j][ii + 7];
                    B[j][ii + 4] = a0;
                    B[j][ii + 5] = a1;
                    B[j][ii + 6] = a2;
                    B[j][ii + 7] = a3;
                    B[j + 4][ii] = a4;
                    B[j + 4][ii + 1] = a5;
                    B[j + 4][ii + 2] = a6;
                    B[j + 4][ii + 3] = a7;
                }
            }
        }
    }
    else {
        for (ii = 0; ii < N; ii += 8) {
            for (j = 0; j < M; j += 8) {
                if ((ii >= (N / 8) * 8) && (N % 8)) {
                    for (i = (N / 8) * 8; i < N; i++) {
                        if ((j >= (M / 8) * 8) && (M % 8)) {
                            j = (M / 8) * 8;
                            if (M % 8 >= 4) {
                                a0 = A[i][j];
                                a1 = A[i][j + 1];
                                a2 = A[i][j + 2];
                                a3 = A[i][j + 3];
                                B[j][i] = a0;
                                B[j + 1][i] = a1;
                                B[j + 2][i] = a2;
                                B[j + 3][i] = a3;
                                j += 4;
                            }
                            for (; j < M; j++) {
                                B[j][i] = A[i][j];
                            }
                        }
                        else {
                            a0 = A[i][j];
                            a1 = A[i][j + 1];
                            a2 = A[i][j + 2];
                            a3 = A[i][j + 3];
                            a4 = A[i][j + 4];
                            a5 = A[i][j + 5];
                            a6 = A[i][j + 6];
                            a7 = A[i][j + 7];
                            B[j][i] = a0;
                            B[j + 1][i] = a1;
                            B[j + 2][i] = a2;
                            B[j + 3][i] = a3;
                            B[j + 4][i] = a4;
                            B[j + 5][i] = a5;
                            B[j + 6][i] = a6;
                            B[j + 7][i] = a7;
                        }
                    }
                }
                else {
                    for (i = ii; i < ii + 8; i++) {
                        if ((j >= (M / 8) * 8) && (M % 8)) {
                            j = (M / 8) * 8;
                            if (M % 8 >= 4) {
                                a0 = A[i][j];
                                a1 = A[i][j + 1];
                                a2 = A[i][j + 2];
                                a3 = A[i][j + 3];
                                B[j][i] = a0;
                                B[j + 1][i] = a1;
                                B[j + 2][i] = a2;
                                B[j + 3][i] = a3;
                                j += 4;
                            }
                            for (; j < M; j++) {
                                B[j][i] = A[i][j];
                            }
                        }
                        else {
                            a0 = A[i][j];
                            a1 = A[i][j + 1];
                            a2 = A[i][j + 2];
                            a3 = A[i][j + 3];
                            a4 = A[i][j + 4];
                            a5 = A[i][j + 5];
                            a6 = A[i][j + 6];
                            a7 = A[i][j + 7];
                            B[j][i] = a0;
                            B[j + 1][i] = a1;
                            B[j + 2][i] = a2;
                            B[j + 3][i] = a3;
                            B[j + 4][i] = a4;
                            B[j + 5][i] = a5;
                            B[j + 6][i] = a6;
                            B[j + 7][i] = a7;
                        }
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{

    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

    // int ei = 
    // int bsize = 4;
    // int esize = bsize * (n / bsize);


    // for (ii = 0; ii < M / 4; ii++)
}

int min(int a, int b) {
    return a > b? a : b;
}

/* 
 * block trans
 */
char block_trans_desc[] = "block scan transpose";
void block_trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    int ii, jj;
    int a0, a1, a2, a3, a4, a5, a6, a7;

    for (ii = 0; ii < N; ii += 8) {
        for (jj = 0; jj < M; jj += 8) {
            for (i = ii; i < ii + 4; i++) {
                a0 = A[i][jj];
                a1 = A[i][jj + 1];
                a2 = A[i][jj + 2];
                a3 = A[i][jj + 3];
                a4 = A[i][jj + 4];
                a5 = A[i][jj + 5];
                a6 = A[i][jj + 6];
                a7 = A[i][jj + 7];
                B[jj][i] = a0;
                B[jj + 1][i] = a1;
                B[jj + 2][i] = a2;
                B[jj + 3][i] = a3;
                B[jj][i + 4] = a4;
                B[jj + 1][i + 4] = a5;
                B[jj + 2][i + 4] = a6;
                B[jj + 3][i + 4] = a7;
            }

            for (i = ii + 4; i < ii + 8; i++) {
                a0 = A[i][jj];
                a1 = A[i][jj + 1];
                a2 = A[i][jj + 2];
                a3 = A[i][jj + 3];
                a4 = A[i][jj + 4];
                a5 = A[i][jj + 5];
                a6 = A[i][jj + 6];
                a7 = A[i][jj + 7];
                B[jj + 4][i - 4] = a0;
                B[jj + 4 + 1][i - 4] = a1;
                B[jj + 4 + 2][i - 4] = a2;
                B[jj + 4 + 3][i - 4] = a3;
                B[jj + 4][i] = a4;
                B[jj + 4 + 1][i] = a5;
                B[jj + 4 + 2][i] = a6;
                B[jj + 4 + 3][i] = a7;
            }

            for (j = jj; j < jj + 4; j++) {
                a0 = B[j + 4][ii];
                a1 = B[j + 4][ii + 1];
                a2 = B[j + 4][ii + 2];
                a3 = B[j + 4][ii + 3];
                a4 = B[j][ii + 4];
                a5 = B[j][ii + 5];
                a6 = B[j][ii + 6];
                a7 = B[j][ii + 7];
                B[j][ii + 4] = a0;
                B[j][ii + 5] = a1;
                B[j][ii + 6] = a2;
                B[j][ii + 7] = a3;
                B[j + 4][ii] = a4;
                B[j + 4][ii + 1] = a5;
                B[j + 4][ii + 2] = a6;
                B[j + 4][ii + 3] = a7;
            }
        }
    }
}

/* 
 * column-trans - A column-wise transpose function.
 */
char column_trans_desc[] = "column-wise scan transpose";
void column_trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (j = 0; j < M; j++) {
        for (i = 0; i < N; i++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/* 
 * half-trans - A half transpose function.
 */
char half_trans_desc[] = "half scan transpose";
void half_trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    // int ii, work_amount_n;
    // int bsize;
    
    // bsize = 8;
    // work_amount_n = (N / bsize) * bsize;

    for (i = 0; i < N; i++) {
        for (j = i; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }

    }

    for (j = 0; j < M; j++) {
        for (i = j + 1; i < N; i++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
    // for (ii = 0; ii < work_amount_n; ii += bsize) {
    //     for (j = 0; j < M; j++) {
    //         for (i = ii; i < ((ii + bsize <= work_amount_n)?(ii + bsize):(work_amount_n)); i++) {
    //             tmp = A[i][j];
    //             B[j][i] = tmp;
    //         }
    //     }
    // }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

    registerTransFunction(block_trans, block_trans_desc);
    // registerTransFunction(half_trans, half_trans_desc);
    // registerTransFunction(column_trans, column_trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

