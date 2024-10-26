#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

/* Function to read matrix from file */
int readMatrixFromFile(const char *filename, double matrix[MAX_SIZE][MAX_SIZE], int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File does not exist\n");
        return 0;
    }

    char line[1000];
    int row = 0, col = 0;
    *n = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, "[, ");
        col = 0;
        while (token != NULL) {
            if (isdigit(token[0]) || token[0] == '-' || token[0] == '.' || token[0] == '+') {
                matrix[row][col] = strtod(token, NULL);
                col++;
            }
            token = strtok(NULL, "[], ");
        }
        if (col > 0) {
            row++;
            if (*n == 0) {
                *n = col;
            }
        }
    }

    if (row != *n) {
        printf("Error: Matrix is not square. Rows: %d, Columns: %d\n", row, *n);
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int readMatrixfromKeyboard(int rows, double matrix[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            scanf("%lf", &matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}

/* Function to calculate determinant */
double calculateDeterminant(double matrix[MAX_SIZE][MAX_SIZE], int n) {
    double det = 1;
    double temp[MAX_SIZE][MAX_SIZE];
    memcpy(temp, matrix, sizeof(temp));

    for (int i = 0; i < n; i++) {
        if (temp[i][i] == 0) {
            return 0; // Singular matrix
        }
        for (int j = i + 1; j < n; j++) {
            double factor = temp[j][i] / temp[i][i];
            for (int k = 0; k < n; k++) {
                temp[j][k] -= factor * temp[i][k];
            }
        }
        det *= temp[i][i];
    }
    return det;
}

/* LU Decomposition using Doolittle's algorithm */
void luDecomposition(double matrix[MAX_SIZE][MAX_SIZE], double lower[MAX_SIZE][MAX_SIZE],
                     double upper[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int k = i; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++) {
                sum += (lower[i][j] * upper[j][k]);
            }
            upper[i][k] = matrix[i][k] - sum;
        }

        for (int k = i; k < n; k++) {
            if (i == k) {
                lower[i][i] = 1;
            } else {
                double sum = 0;
                for (int j = 0; j < i; j++) {
                    sum += (lower[k][j] * upper[j][i]);
                }
                lower[k][i] = (matrix[k][i] - sum) / upper[i][i];
            }
        }
    }
}

/* Function to print matrix */
void printMatrix(double matrix[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n, user_choice, n_rows;
    char filename[100];
    double matrix[MAX_SIZE][MAX_SIZE], lower[MAX_SIZE][MAX_SIZE], upper[MAX_SIZE][MAX_SIZE];

    printf("How do you want to enter the data?\n");
    printf("1) Through the file\n");
    printf("2) Through the keyboard\n");
    scanf("%d", &user_choice);

    switch (user_choice) {
    case 1:
        printf("Enter the filename containing the matrix: ");
        scanf("%s", filename);

        if (readMatrixFromFile(filename, matrix, &n) == 0) {
            return 1;
        }

        printf("Original Matrix is:\n");
        printMatrix(matrix, n);

        if (calculateDeterminant(matrix, n) == 0) {
            printf("Error: Matrix is singular. LU decomposition is not possible.\n");
            return 1;
        }

        memset(lower, 0, sizeof(lower));
        memset(upper, 0, sizeof(upper));

        luDecomposition(matrix, lower, upper, n);

        printf("\nLower Triangular Matrix:\n");
        printMatrix(lower, n);

        printf("\nUpper Triangular Matrix:\n");
        printMatrix(upper, n);
        break;

    default:
        printf("Enter the number of rows/columns you want to enter\n");
        printf("Mind you that LU decomposition only works for square matrices\n");
        scanf("%d", &n_rows);

        memset(matrix, 0, sizeof(matrix));
        memset(lower, 0, sizeof(lower));
        memset(upper, 0, sizeof(upper));

        readMatrixfromKeyboard(n_rows, matrix);

        if (calculateDeterminant(matrix, n_rows) == 0) {
            printf("Error: Matrix is singular. LU decomposition is not possible.\n");
            return 1;
        }

        luDecomposition(matrix, lower, upper, n_rows);

        printf("Matrix:\n");
        printMatrix(matrix, n_rows);

        printf("\nLower Triangular Matrix:\n");
        printMatrix(lower, n_rows);

        printf("\nUpper Triangular Matrix:\n");
        printMatrix(upper, n_rows);
        break;
    }

    return 0;
}

// Things to be added
// What if the user adds some mathematical constants for the matrix entries?
// Error checking i.e., the square matrix should be non singular