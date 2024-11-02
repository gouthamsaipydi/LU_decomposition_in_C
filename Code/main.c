#include "functions.h"

int main()
{
    int n, user_choice;
    char filename[100];
    double **matrix, **lower, **upper;
    /*Initializing the loop dummies*/
    int i;

    printf("Namaste User\n");
    printf("-------------------------------------------------------\n");
    /* Loop until a valid matrix size is entered */
    while (1)
    {

        printf("Enter the number of rows/columns (for a square matrix): ");
        if (scanf(" %d", &n) == 1 && n > 1)
        {
            break;
        }
        else
        {
            printf("Error: Invalid matrix size. Please enter a positive integer.\n");
            while (getchar() != '\n')
            {
                continue;
            }
        }
    }

    /* Allocate memory for the matrices and check for allocation success */
    matrix = (double **)malloc(n * sizeof(double *));
    lower = (double **)malloc(n * sizeof(double *));
    upper = (double **)malloc(n * sizeof(double *));
    if (matrix == NULL || lower == NULL || upper == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    for (i = 0; i < n; i++)
    {
        matrix[i] = (double *)malloc(n * sizeof(double));
        lower[i] = (double *)calloc(n, sizeof(double));
        upper[i] = (double *)calloc(n, sizeof(double));
        if (matrix[i] == NULL || lower[i] == NULL || upper[i] == NULL)
        {
            printf("Error: Memory allocation failed.\n");
            return 1;
        }
    }

    /* Loop until a valid input method is chosen */
    while (1)
    {
        printf("How do you want to enter the data?\n");
        printf("1) From file\n");
        printf("2) From keyboard\n");
        if (scanf(" %d", &user_choice) == 1 && (user_choice == 1 || user_choice == 2))
        {
            break;
        }
        else
        {
            printf("Error: Invalid user choice. Please enter 1 or 2 \n");
            while (getchar() != '\n')
            {
                continue;
            }
        }
    }

    /* Matrix input based on user choice with error-checking */
    if (user_choice == 1)
    {
        printf("Enter the filename: ");
        scanf(" %s", filename);
        if (!readMatrixFromFile(&n, filename, matrix))
        {
            printf("Error: Failed to read matrix from file.\n");
            return 1;
        }
        printMatrix(n, matrix);
    }
    else
    {
        if (!readMatrixFromKeyboard(n, matrix))
        {
            printf("Error: Failed to read matrix from keyboard.\n");
            return 1;
        }
    }
    printf("Matrix:\n");
    printMatrix(n, matrix);
    if (calculateDeterminant(n, matrix) == 0)
    {
        printf("Matrix is singular, LU decomposition Not possible!\n");
        return 1;
    }

    /* Loop until a valid operation is chosen */
    while (1)
    {
        printf("\nChoose an operation:\n");
        printf("1) Print LU decomposition\n");
        printf("2) Calculate determinant\n");
        printf("3) Solve linear system\n");
        if (scanf(" %d", &user_choice) == 1 && user_choice >= 1 && user_choice <= 3)
            break;
        printf("Error: Invalid choice. Please enter 1, 2, or 3.\n");
    }

    /* Perform the chosen operation with error-checking */
    if (user_choice == 1)
    {
        luDecomposition(n, matrix, lower, upper);
        printf("Lower matrix:\n");
        printMatrix(n, lower);
        printf("Upper matrix:\n");
        printMatrix(n, upper);
    }
    else if (user_choice == 2)
    {
        luDecomposition(n, matrix, lower, upper);
        double det = luDeterminant(n, upper);
        printf("Determinant: %.4lf\n", det);
    }
    else if (user_choice == 3)
    {
        double *b = (double *)malloc(n * sizeof(double));
        double *x = (double *)malloc(n * sizeof(double));
        if (b == NULL || x == NULL)
        {
            printf("Error: Memory allocation failed for vectors.\n");
            return 1;
        }

        /* Loop to enter vector b */
        printf("Enter the elements of vector b:\n");
        for (i = 0; i < n; i++)
        {
            while (1)
            {
                printf("b[%d]: ", i);
                if (scanf(" %lf", &b[i]) == 1)
                    break;
                printf("Error: Invalid entry. Please enter a number.\n");
            }
        }
        luDecomposition(n, matrix, lower, upper);
        /* Solve the linear system and display results */
        solveLinearSystem(n, lower, upper, b, x);
        printf("Solution vector x:\n");
        for (i = 0; i < n; i++)
        {
            printf("%.4lf\n", x[i]);
        }
        free(b);
        free(x);
    }

    /* Free allocated memory */
    for (i = 0; i < n; i++)
    {
        free(matrix[i]);
        free(lower[i]);
        free(upper[i]);
    }
    free(matrix);
    free(lower);
    free(upper);

    return 0;
}
