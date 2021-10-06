#include<iostream>
#include<conio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

double DETERMINANT(int **matrix, int n, int column);
void crout(int **A, double **&L, int n);

int main()
{
	int n = 50, NUM_TASKS, NUM_THREADS = 4;
	srand(time(NULL));

	cout << "Enter n (nxn matrix):\n";
	cin >> n;

	int **matrix = (int **)malloc(n * sizeof(int*));
	for (int i = 0;i < n;++i)
		matrix[i] = (int*)malloc(n * sizeof(int));

	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;++j)
			matrix[i][j] = rand() % 10;


	omp_set_num_threads(NUM_THREADS);

	NUM_TASKS = 2 * NUM_THREADS;
	if (NUM_TASKS > n)
		NUM_TASKS = NUM_THREADS;

	int RES = n%NUM_TASKS;
	int step = n / NUM_TASKS;
	int *START = new int[NUM_TASKS];
	int *END = new int[NUM_TASKS];
	for (int i = 0;i < NUM_TASKS;END[i++] = 0);
	for (int i = 0;i <RES;END[NUM_TASKS - 1 - i++] = 1);
	END[0] = step;
	START[0] = 0;
	for (int i = 1;i < NUM_TASKS;++i)
	{
		START[i] = END[i - 1];
		END[i] += START[i] + step;
	}

	int *Deteminant = new int[NUM_THREADS];

	for (int i = 0;i < NUM_THREADS;Deteminant[i++] = 0);

#pragma omp parallel
	{
		int Num_threads = omp_get_num_threads();
		int id = omp_get_thread_num();
		for (int i = id;i < NUM_TASKS;i += Num_threads)
		{
#pragma omp task
			{
				int start = START[i];
				int end = END[i];
				for (;start < end;++start)
					Deteminant[id] += (pow(-1, start)*matrix[0][start] * DETERMINANT(matrix, n, start));
			}
		}
	}

	for (int i = 0;i < n;++i, cout << "\n")
	for (int j = 0;j < n;++j)
	cout << matrix[i][j] << " ";

	for (int i = 1;i < NUM_THREADS;i++)
		Deteminant[0] += Deteminant[i];

	cout << "\n\nDETERMINANT= " << Deteminant[0];

	_getch();
	return 0;
}

double DETERMINANT(int **matrix, int n, int column)
{
	int **tempmatrix = (int **)malloc((--n) * sizeof(int*));
	double **L = (double **)malloc(n * sizeof(double*));
	for (int k = 0;k < n;++k)
	{
		tempmatrix[k] = (int*)malloc(n * sizeof(int));
		L[k] = (double*)malloc(n * sizeof(double));
	}

	for (int k = 1, x = 0;k <= n;++k, x = 0)
		for (int j = 0;j <= n;++j)
			if (j != column)
				tempmatrix[k - 1][x++] = matrix[k][j];


	crout(tempmatrix, L, n);

	for (int i = 1;i < n;++i)
		L[0][0] *= L[i][i];

	return L[0][0];
}

void crout(int **A, double **&L, int n)
{
	int i, j, k;
	double sum = 0;

	double **U = (double**)malloc(n * sizeof(double*));
	for (i = 0;i < n;++i)
		U[i] = (double*)malloc(n * sizeof(n * sizeof(double)));

	for (i = 0;i < n;++i)
		for (j = 0;j < n;++j)
		{
			U[i][j] = 0;
			L[i][j] = 0;
		}


	for (i = 0; i < n; i++)
	{
		U[i][i] = 1;
	}

	for (j = 0; j < n; j++)
	{
		for (i = j; i < n; i++)
		{
			sum = 0;
			for (k = 0; k < j; k++)
			{
				sum = sum + L[i][k] * U[k][j];
			}
			L[i][j] = A[i][j] - sum;
		}

		for (i = j; i < n; i++)
		{
			sum = 0;
			for (k = 0; k < j; k++) {
				sum = sum + L[j][k] * U[k][i];
			}
			if (L[j][j] == 0) {
				printf("det(L) close to 0!\n Can't divide by 0...\n");
				exit(EXIT_FAILURE);
			}
			U[j][i] = (A[j][i] - sum) / L[j][j];
		}
	}
}