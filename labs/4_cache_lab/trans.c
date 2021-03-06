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
    int i, j, row, col;
    int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

    if (N == 32 && M == 32) {
        for (row = 0; row < N; row+=8) {
            for (col = 0; col < M; col+=8) {
                for (i = row; i < row+8; i++) {
                    tmp1 = A[i][col];
                    tmp2 = A[i][col+1];
                    tmp3 = A[i][col+2];
                    tmp4 = A[i][col+3];
                    tmp5 = A[i][col+4];
                    tmp6 = A[i][col+5];
                    tmp7 = A[i][col+6];
                    tmp8 = A[i][col+7];

                    B[col][i] = tmp1;
                    B[col+1][i] = tmp2;
                    B[col+2][i] = tmp3;
                    B[col+3][i] = tmp4;
                    B[col+4][i] = tmp5;
                    B[col+5][i] = tmp6;
                    B[col+6][i] = tmp7;
                    B[col+7][i] = tmp8;
                }
            }
        }
    } else if (N == 64 && M == 64) {
        for (row = 0; row < N; row+=8) {
            for (col = 0; col < M; col+=8) {
                for (i = row; i < row + 8; i+=4) {
                    for (j = col; j < col + 8; j+=4) {
                        tmp1 = A[i][j];
                        tmp2 = A[i][j+1];
                        tmp3 = A[i][j+2];
                        tmp4 = A[i][j+3];

                        B[j][i] = tmp1;

                        tmp5 = A[i+1][j];
                        tmp6 = A[i+1][j+1];
                        tmp7 = A[i+1][j+2];
                        tmp8 = A[i+1][j+3];

                        B[j][i+1] = tmp5;

                        tmp1 = A[i+2][j]; // 9
                        tmp5 = A[i+3][j]; // 13

                        B[j][i+2] = tmp1;
                        B[j][i+3] = tmp5;

                        tmp1 = A[i+2][j+1]; // 10
                        tmp5 = A[i+3][j+1]; // 14

                        B[j+1][i] = tmp2;
                        B[j+1][i+1] = tmp6;
                        B[j+1][i+2] = tmp1;
                        B[j+1][i+3] = tmp5;

                        tmp1 = A[i+2][j+2]; // 11
                        tmp5 = A[i+2][j+3]; // 12
                        tmp2 = A[i+3][j+2]; // 15

                        B[j+2][i] = tmp3;
                        B[j+2][i+1] = tmp7;
                        B[j+2][i+2] = tmp1;
                        B[j+2][i+3] = tmp2;

                        tmp1 = A[i+3][j+3];

                        B[j+3][i] = tmp4;
                        B[j+3][i+1] = tmp8;
                        B[j+3][i+2] = tmp5;
                        B[j+3][i+3] = tmp1;
                    }
                }
            }
        }
    } else {
        for (row = 0; row < N; row+=16) {
            for (col = 0; col < M; col+=16) {
                for (i = row; i < row + 16 && i < N; i++) {
                    for (j = col; j < col + 16 && j < M; j++) {
                        B[j][i] = A[i][j];
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
