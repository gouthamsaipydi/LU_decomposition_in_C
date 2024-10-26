#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_SIZE 100
#define MAX_EXPR_LEN 100

// Function to evaluate a mathematical expression
double evaluateExpression(const char* expr) {
    char sanitized[MAX_EXPR_LEN];
    int j = 0;
    for (int i = 0; expr[i]; i++) {
        if (!isspace(expr[i])) {
            sanitized[j++] = expr[i];
        }
    }
    sanitized[j] = '\0';

    // This is a simplified parser. In a real-world scenario, you'd want a more robust solution.
    if (strstr(sanitized, "sin(") == sanitized) {
        return sin(evaluateExpression(sanitized + 4));
    } else if (strstr(sanitized, "cos(") == sanitized) {
        return cos(evaluateExpression(sanitized + 4));
    } else if (strstr(sanitized, "tan(") == sanitized) {
        return tan(evaluateExpression(sanitized + 4));
    } else if (strstr(sanitized, "log(") == sanitized) {
        return log(evaluateExpression(sanitized + 4));
    } else if (strstr(sanitized, "exp(") == sanitized) {
        return exp(evaluateExpression(sanitized + 4));
    } else if (strstr(sanitized, "sqrt(") == sanitized) {
        return sqrt(evaluateExpression(sanitized + 5));
    }
    
    // If it's not a function, treat it as a number
    return atof(sanitized);
}

// Function to read matrix from file
int readMatrixFromFile(const char* filename, double matrix[MAX_SIZE][MAX_SIZE], int* n) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    char line[1000];
    int row = 0, col = 0;
    *n = 0;

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, "[], \n");
        col = 0;
        while (token != NULL) {
            if (isdigit(token[0]) || token[0] == '-' || token[0] == '.' || 
                isalpha(token[0])) { // Check for function names
                matrix[row][col] = evaluateExpression(token);
                col++;
            }
            token = strtok(NULL, "[], \n");
        }
        if (col > 0) {
            row++;
            if (*n == 0) *n = col;  // Set n to the number of columns in the first row
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

// Function to perform LU decomposition
void luDecomposition(double matrix[MAX_SIZE][MAX_SIZE], double lower[MAX_SIZE][MAX_SIZE], 
                     double upper[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        // Upper Triangular
        for (int k = i; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++) {
                sum += (lower[i][j] * upper[j][k]);
            }
            upper[i][k] = matrix[i][k] - sum;
        }

        // Lower Triangular
        for (int k = i; k < n; k++) {
            if (i == k) {
                lower[i][i] = 1; // Diagonal as 1
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

// Function to print matrix
void printMatrix(double matrix[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    double matrix[MAX_SIZE][MAX_SIZE], lower[MAX_SIZE][MAX_SIZE], upper[MAX_SIZE][MAX_SIZE];
    int n;
    char filename[100];

    printf("Enter the filename containing the matrix: ");
    scanf("%s", filename);

    if (!readMatrixFromFile(filename, matrix, &n)) {
        return 1;
    }

    printf("Original Matrix:\n");
    printMatrix(matrix, n);

    // Initialize L and U matrices
    memset(lower, 0, sizeof(lower));
    memset(upper, 0, sizeof(upper));

    luDecomposition(matrix, lower, upper, n);

    printf("\nLower Triangular Matrix:\n");
    printMatrix(lower, n);

    printf("\nUpper Triangular Matrix:\n");
    printMatrix(upper, n);

    return 0;
}