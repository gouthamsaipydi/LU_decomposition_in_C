#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Function to read matrix from file */
int readMatrixFromFile(int *n, const char *filename, double **matrix);

/* Function to read matrix from keyboard */
int readMatrixFromKeyboard(int n, double **matrix);

/* Function to calculate determinant */
double calculateDeterminant(int n, double **matrix);

/* Function to calculate determinant of LU */
double luDeterminant(int n, double **upper);

/* LU Decomposition using Doolittle's algorithm */
void luDecomposition(int n, double **matrix, double **lower, double **upper);

/*Function to solve the Linear system*/
int solveLinearSystem(int n, double **lower, double **upper, double *b, double *x);

/* Function to print matrix */
void printMatrix(int n, double **matrix);
