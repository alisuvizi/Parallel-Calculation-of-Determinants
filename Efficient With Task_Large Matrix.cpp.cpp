#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <conio.h>

using namespace std;

// Dimension of input square matrix 
#define N 50
// Function to get determinant of matrix 
int  determinantOfMatrix(double** mat, int n)
{
	int index;
	double  num1, num2; // Initialize result 
	double  det = 1;
	double total = 1;
	// temporary array for storing row 
	
	double *temp = (double *)calloc(n+1, sizeof(double));

	//loop for traversing the diagonal elements 
	for (int i = 0; i < n; i++)
	{
		index = i; // initialize the index 

		//finding the index which has non zero value 
		while (mat[index][i] == 0 && index < n) {
			index++;

		}
		if (index == n) // if there is non zero element 
		{
			// the determinat of matrix as zero 
			continue;

		}
		if (index != i)
		{
			//loop for swaping the diagonal element row and index row 
			for (int j = 0; j < n; j++)
			{
				swap(mat[index][j], mat[i][j]);
			}
			//determinant sign changes when we shift rows 
			//go through determinant properties 
			det = det * pow(-1, index - i);
		}

		//storing the values of diagonal row elements 
		for (int j = 0; j < n; j++)
		{
			temp[j] = mat[i][j];

		}
		//traversing every row below the diagonal element 
		for (int j = i + 1; j < n; j++)
		{
			num1 = temp[i]; //value of diagonal element 
			num2 = mat[j][i]; //value of next row element 

			//traversing every column of row 
			// and multiplying to every row 
			for (int k = 0; k < n; k++)
			{
				//multiplying to make the diagonal 
				// element and next row element equal
#pragma omp task 
				mat[j][k] = (num1 * mat[j][k]) - (num2 * temp[k]);

			}
#pragma omp taskwait

			total = total * num1; // Det(kA)=kDet(A); 
		}

	}

	//mulitplying the diagonal elements to get determinant 
	for (int i = 0; i < n; i++)
	{
		det = det * mat[i][i];

	}
	return (det / total); //Det(kA)/k=Det(A); 

	
}

// Driver code
double det;
int main()
{
	double f;
	omp_set_num_threads(8);
	double **mat = (double **)calloc(N, sizeof(double*));
	for (int q = 0; q < N; q++)
		{
			mat[q] = (double*)calloc(N, sizeof(double*));
		}

	//double det = 0;
	for (int  i = 0; i < N; i++)
	{
		for (int  j = 0; j < N; j++)
		{
			mat[i][j] = (rand() % 10);
		}
	}


	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout<< mat[i][j]<<"\t";
		}
		cout << "\n";
	}

	cout << "\n start ---------- \n ";

#pragma omp parallel
	{
#pragma omp single
		{
			det = determinantOfMatrix(mat, N);

		}
	}
	
	printf("Determinant of the matrix is : ");
	cout << det;
	//printf("DET=%f", f);
	getch();
	return 0;

}
