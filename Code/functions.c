#include "functions.h"
/*Total Number of functions- 7 */

/*   readMatrixFromFile
   * Function to read matrix from a file
        Reads a matrix from the specified file and checks if it's square.
   * Parameters:
        n - pointer to an integer to store matrix dimension
        filename - name of the file to read the matrix from
        matrix - pointer to the matrix to populate
   * Returns:
        successful -> 1
        otherwise  -> 0
 */
int readMatrixFromFile(int *n, const char *filename, double **matrix)
{
    char line[1000]; /* Buffer to read each line in the file */
    int row = 0, col = 0;
    *n = 0;
    FILE *file;
    char *token;

    file = fopen(filename, "r"); /* Open file in read mode */
    if (file == NULL)
    {
        printf("File does not exist\n");
        return 0; /* Return 0 if file doesn't exist */
    }

    /* Read the file line by line */
    while (fgets(line, sizeof(line), file))
    {
        token = strtok(line, "[], (){}"); /* Tokenize line to extract numbers */
        col = 0;

        /* Process each token in the line */
        while (token != NULL)
        {
            /* Check if the token is a number (handles signs and decimals) */
            if (isdigit(token[0]) || token[0] == '-' || token[0] == '.' || token[0] == '+')
            {
                matrix[row][col] = strtod(token, NULL); /* Convert token to double and stores in the matrix */
                col++;
            }
            token = strtok(NULL, "[], (){}"); /* Move to next token */
        }

        /* If column count is valid, move to next row */
        if (col > 0)
        {
            row++;
            if (*n == 0)
            {
                *n = col; /* Set matrix dimension for the first row */
            }
            /* Check if the number of columns match for each row */
            if (*n != col)
            {
                printf("No. of columns don't match in the matrix within the file\n");
                return 0;
            }
        }
    }

    /* Check if rows equal columns for a square matrix */
    if (row != *n)
    {
        printf("Error: Matrix is not square. Rows: %d, Columns: %d\n", row, *n);
        fclose(file);
        return 0;
    }

    fclose(file); /* Close file after reading */
    return 1;     /* Return 1 for success */
}

/* readMatrixFromKeyboard
 * Function to read matrix from keyboard
    Prompts the user to enter matrix elements directly(row-wise).
 * Parameters:
    n - the dimension of the matrix (assumed square)
    matrix - pointer to the matrix to populate
 * Returns:
    1 if successful
 */

/* Function to read matrix from keyboard with individual prompts for each entry */
int readMatrixFromKeyboard(int n, double **matrix)
{
    int i, j;
    printf("Enter the elements of the matrix row by row:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            while (1) /* Loop until valid input is provided */
            {
                printf("matrix[%d][%d]: ", i + 1, j + 1);

                if (scanf(" %lf", &matrix[i][j]) == 1)
                {
                    while (getchar() != '\n')
                    {
                        continue;
                    }
                    break; /* Valid input received, exit loop */
                }
                else
                {
                    /*Clear the invalid input from the buffer- fflush can be used here*/
                    while (getchar() != '\n')
                    {
                        continue;
                    }

                    printf("Invalid input. Please enter a valid number.\n");
                }
            }
        }
    }
    return 1; /* Return 1 for success */
}

/*
   luDecomposition
 * LU Decomposition using Doolittle's algorithm
  Decomposes matrix A into Lower (L) and Upper (U) triangular matrices.
 * Parameters:
    n - the dimension of the matrix
    matrix - the original matrix to decompose
    lower - the resulting lower triangular matrix (L)
    upper - the resulting upper triangular matrix (U)
 */
void luDecomposition(int n, double **matrix, double **lower, double **upper)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        /* Compute the elements of the upper triangular matrix (U) */
        for (k = i; k < n; k++)
        {
            double sum = 0; /* Reset sum for each element of U */
            for (j = 0; j < i; j++)
            {
                sum += (lower[i][j] * upper[j][k]);
            }
            upper[i][k] = matrix[i][k] - sum;
        }

        /* Compute the elements of the lower triangular matrix (L) */
        for (k = i; k < n; k++)
        {
            if (i == k)
            {
                lower[i][i] = 1; /* Set diagonal of L to 1 */
            }
            else
            {
                double sum = 0; /* Reset sum for each element of L */
                for (j = 0; j < i; j++)
                {
                    sum += (lower[k][j] * upper[j][i]);
                }
                lower[k][i] = (matrix[k][i] - sum) / upper[i][i];
            }
        }
    }
}

/* solveLinearSystem
 * Function to solve a linear system using LU decomposition
 * Parameters:
    n - dimension of the matrix
    lower - lower triangular matrix (L)
    upper - upper triangular matrix (U)
    b - right-hand side vector of the system
    x - solution vector
 */

/* Modified solveLinearSystem function with error handling */

int solveLinearSystem(int n, double **lower, double **upper, double *b, double *x)
{
    double *y;
    int i, j;
    /* Input validation */
    if (n <= 0 || lower == NULL || upper == NULL || b == NULL || x == NULL)
    {
        printf("Invalid input parameters.\n");
        return 0;
    }

    /* Allocate memory for temporary vector y */
    y = (double *)malloc(n * sizeof(double));
    if (y == NULL)
    {
        printf("Memory allocation failed for vector y.\n");
        return 0;
    }

    /* Forward substitution for Ly = b */
    for (i = 0; i < n; i++)
    {
        if (lower[i][i] == 0)
        {
            printf("Error: Zero diagonal element in lower triangular matrix at row %d.\n", i);
            free(y);
            return 0;
        }

        y[i] = b[i];
        for (j = 0; j < i; j++)
        {
            y[i] -= lower[i][j] * y[j];
        }
        y[i] /= lower[i][i]; /* Division by the diagonal element */
    }

    /* Backward substitution for Ux = y */
    for (i = n - 1; i >= 0; i--)
    {
        if (upper[i][i] == 0)
        {
            printf("Error: Zero diagonal element in upper triangular matrix at row %d.\n", i);
            free(y);
            return 0;
        }

        x[i] = y[i];
        for (j = i + 1; j < n; j++)
        {
            x[i] -= upper[i][j] * x[j];
        }
        x[i] /= upper[i][i]; /* Division by the diagonal element */
    }

    free(y);
    return 1;
}

/* calculateDeterminant
  Recursive function to calculate determinant using cofactor expansion
  Parameters:
    n - dimension of the matrix
    matrix - matrix to calculate the determinant for
  Returns:
    The determinant of the matrix
 */

double calculateDeterminant(int n, double **matrix)
{
    double det = 0.0, minorDet;
    int i, j, col;
    double **subMatrix;
    int subCol;

    /* Base case for a 1x1 matrix */
    if (n == 1)
    {
        return matrix[0][0];
    }

    /* Base case for a 2x2 matrix */
    if (n == 2)
    {
        return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
    }

    /* Allocate memory for subMatrix of size (n-1) x (n-1) */
    subMatrix = (double **)malloc((n - 1) * sizeof(double *));
    for (i = 0; i < n - 1; i++)
    {
        subMatrix[i] = (double *)malloc((n - 1) * sizeof(double));
    }

    /* Recursive case: cofactor expansion along the first row */
    for (col = 0; col < n; col++)
    {
        /* Form the submatrix by excluding the current row and column */
        for (i = 1; i < n; i++)
        { /* Start from row 1 to exclude the first row */
            subCol = 0;
            for (j = 0; j < n; j++)
            {
                if (j == col)
                { /* Skip the current column */
                    continue;
                }
                subMatrix[i - 1][subCol] = matrix[i][j]; /* Copy element into submatrix */
                subCol++;
            }
        }

        /* Recursive step for the minor determinant */
        minorDet = calculateDeterminant(n - 1, subMatrix);

        /* Calculate cofactor and add to determinant */
        det += (col % 2 == 0 ? 1 : -1) * matrix[0][col] * minorDet;
    }

    /* Free allocated memory for subMatrix */
    for (i = 0; i < n - 1; i++)
    {
        free(subMatrix[i]);
    }
    free(subMatrix);

    return det;
}

/* luDeterminant
 * Function to calculate determinant using LU decomposition
 * Parameters:
    n - dimension of the matrix
    upper - upper triangular matrix (U) from LU decomposition
 * Returns:
    The determinant of the matrix (product of diagonal elements of U)
 */
double luDeterminant(int n, double **upper)
{
    double det = 1.0;
    int i;
    for (i = 0; i < n; i++)
    {
        det *= upper[i][i]; /* Product of the diagonal elements of U */
    }
    return det;
}

/* printMatrix
 * Function to print a matrix
 * Parameters:
    n - dimension of the matrix
    matrix - matrix to print
 */
void printMatrix(int n, double **matrix)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%.4f\t", matrix[i][j]); /* Print each element with 4 decimal places */
        }
        printf("\n"); /* Newline after each row */
    }
}
