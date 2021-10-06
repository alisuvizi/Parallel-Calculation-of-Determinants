#include<iostream>
#include<conio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>

using namespace std;

int determinant(int **matrix, int n, int k)
{
	int det = 0;//hasel determinan
	int m = n - 1;//andis submatris
	int **submatrix = (int**)calloc(m , sizeof(int*));
	for (int i = 0; i < m; i++)
		submatrix[i] = (int*)calloc(m , sizeof(int));

	for (int i = 1, p = 0; i< n; i++, p++)
		for (int j = 0, q = 0; j < n; j++)
		{
			if (j == k)
				continue;//sakht submatris va hazf sotoon mordenazar
			submatrix[p][q] = matrix[i][j];
			q++;
		}

		//return submatrix[0][0]*(submatrix[1][1] *submatrix[2][2]- submatrix[1][2] * submatrix[2][1]) - submatrix[0][1] *(submatrix[1][0] * submatrix[2][2] - submatrix[1][2] * submatrix[2][0])+ submatrix[0][2]*(submatrix[1][0] * submatrix[2][1] - submatrix[1][1] * submatrix[2][0]);
	//else if (m == 0)
		//return 1;
	else
	{
		for (int x = 0; x<m; x++)
			det += pow(-1, x) * submatrix[0][x] * determinant(submatrix, m, x);//mohasebe sumatris
	}

	
	return det;

}

int main()
{
	int n = 10;//abaad matris
	int N_T = omp_get_max_threads();//tedad nakh
	

	int *det = (int *)calloc(n , sizeof(int));//faza baray har task(zarb deraye dar submatris)


	int **matrix = (int**)calloc(n , sizeof(int *));
	//fill the matris
	for (int i = 0; i < n; i++)
		matrix[i] = (int *)calloc(n , sizeof(int));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = rand() % 10;

	

	omp_set_num_threads(N_T);

#pragma omp parallel 
	{
		int id = omp_get_thread_num();//id har nakh
		int N = omp_get_num_threads();//tedad nakh ekhtesas yafte
		int t = n / N;//teded deraye ekhtesas yafte be har nakh
		int j = id*t;//shoroe andis deraye dar har nakh
		if (id == N - 1)
			t += n%N;//ektesas deraye hay baghimande be nakh akhar
		int w = t / 2;//tedad deraye ekhtesas yafte be har task har nakh
		int q = w;//agar ekhtesas deraye ha be task ha baghimande bedahad (2+3)
		for (int i = 0; i < 2; i++)//har nakh daray 2 task
		{
#pragma omp task
			{
				int g = j;//shoroee andis deraye dar har task
				if (i == 1)
				{
					q = 2 * w + t % 2;//ahe 5 deraye bein 2 task bere: 2ta be avali va 3ta be dovomi
					g += w;
				}
				for (int k = i*w; k < q; k++)//q:payan andis deraye dar har task
				{
					det[g] = matrix[0][g] * pow(-1, g)*determinant(matrix, n, g);//be ezay har task
					g++;
				}
			}
		}
	}

	int Det = 0;
	for (int i = 0; i < n; ++i)
		Det += det[i];
	/*cout << "Determinant of the matrix is: " << Det;
	getch();*/
	return 0;
}