#include<iostream>
#include"omp.h"
#include <climits>

using namespace std;

const int arr_size1 = 10000;
const int arr_size2 = 20000;
const bool printMin = true;
const bool printSum = true;

int arr[arr_size1][arr_size2];

void init_arr();
long long part_sum(int, int, int);
long long part_min(int, int, int);

int main() {
	init_arr();

	long long s = 0;
	long long m = LLONG_MAX;
	for (int i = 0; i < arr_size1; i++)
	{
		long long rowSum = 0;
		for (int j = 0; j < arr_size2; j++)
		{
			rowSum += arr[i][j];
		}
		s += rowSum;
		if (m > rowSum)
		{
			m = rowSum;
		}
	}
	cout << "sum is " << s << endl;
	cout << "min is " << m << endl;
	cout << "--------------------------------------------------------------" << endl;

	omp_set_nested(1);
	double t1 = omp_get_wtime();
#pragma omp parallel sections
	{
#pragma omp section
		{
			cout << "min 1 = " << part_min(0, arr_size1, 1) << endl;
			cout << "min 2 = " << part_min(0, arr_size1, 2) << endl;
			cout << "min 3 = " << part_min(0, arr_size1, 3) << endl;
			cout << "min 4 = " << part_min(0, arr_size1, 4) << endl;
			cout << "min 8 = " << part_min(0, arr_size1, 8) << endl;
			cout << "min 16 = " << part_min(0, arr_size1, 16) << endl;
		}

#pragma omp section
		{
			cout << "sum 1 = " << part_sum(0, arr_size1, 1) << endl;
			cout << "sum 2 = " << part_sum(0, arr_size1, 2) << endl;
			cout << "sum 3 = " << part_sum(0, arr_size1, 3) << endl;
			cout << "sum 4 = " << part_sum(0, arr_size1, 4) << endl;
			cout << "sum 8 = " << part_sum(0, arr_size1, 8) << endl;
			cout << "sum 16 = " << part_sum(0, arr_size1, 16) << endl;
		}
	}
	double t2 = omp_get_wtime();

	cout << "Total time - " << t2 - t1 << " seconds" << endl;
	return 0;
}

void init_arr() {
	for (int i = 0; i < arr_size1; i++) {
		for (int j = 0; j < arr_size2; j++) {
			arr[i][j] = (i + 1) * (j + 1);
		}
	}
}

long long part_sum(int start_index, int finish_index, int num_threads) {
	long long sum = 0;
	double t1 = omp_get_wtime();

#pragma omp parallel for reduction(+:sum) num_threads(num_threads)
	for (int i = start_index; i < finish_index; i++) {
		for (int j = 0; j < arr_size2; j++)
		{
			sum += arr[i][j];
		}
	}

	double t2 = omp_get_wtime();
	
	if (printSum)
	{
		cout << "sum " << num_threads << " threads worked - " << t2 - t1 << " seconds" << endl;
	}

	return sum;
}

long long part_min(int start_index, int finish_index, int num_threads) {
	long long min = LLONG_MAX;
	double t1 = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
	for (int i = start_index; i < finish_index; i++) {
		long long sum = 0;
		for (int j = 0; j < arr_size2; j++)
		{
			sum += arr[i][j];
		}

		if (min > sum)
		{
#pragma omp critical
			if (min > sum)
			{
				min = sum;
			}
		}
	}

	double t2 = omp_get_wtime();

	if (printMin)
	{
		cout << "min " << num_threads << " threads worked - " << t2 - t1 << " seconds" << endl;
	}

	return min;
}