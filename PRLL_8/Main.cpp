#include<mpi.h>
#include<iostream>
#include <cmath>
using namespace std;

const int N = 50000;

void initArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = i;
	}
}

int main(int argc, char **argv)
{
	int arr[N];
	int tempArr[N/2];
	double sum = 0;

	initArray(arr, N);

	MPI_Status status;
	int rank, size,
		elements_per_process,
		elements_recieved;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		int index;
		elements_per_process = N / size;

		for (int i = 1; i < size; i++) {
			index = i * elements_per_process;

			MPI_Send(&elements_per_process,
				1, MPI_INT, i, 0,
				MPI_COMM_WORLD);
			MPI_Send(&arr[index],
				elements_per_process,
				MPI_INT, i, 0,
				MPI_COMM_WORLD);
		}

		int sum = 0;
		for (int i = 0; i < elements_per_process; i++)
			sum += arr[i];
		int tmp;
		for (int i = 1; i < size; i++) {
			MPI_Recv(&tmp, 1, MPI_INT,
				MPI_ANY_SOURCE, 0,
				MPI_COMM_WORLD,
				&status);
			int sender = status.MPI_SOURCE;

			sum += tmp;
		}
		cout << "Sum is >> " << sum;
	}
	else {
		MPI_Recv(&elements_recieved,
			1, MPI_INT, 0, 0,
			MPI_COMM_WORLD,
			&status);

		MPI_Recv(&tempArr, elements_recieved,
			MPI_INT, 0, 0,
			MPI_COMM_WORLD,
			&status);

		int partial_sum = 0;
		for (int i = 0; i < elements_recieved; i++)
			partial_sum += tempArr[i];

		MPI_Send(&partial_sum, 1, MPI_INT,
			0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}