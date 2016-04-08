#include <mpi.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <cstdlib>

#define TAG 0
#define SEND(number, id) MPI_Send(&number, 1, MPI_INT, id, TAG, MPI_COMM_WORLD)
#define RECV(number, id) MPI_Recv(&number, 1, MPI_INT, id, TAG, MPI_COMM_WORLD, &stat)

int main(int argc, char * argv[])
{
	MPI_Status stat;
	int cpu_id;
	int p_count;
	double start, end;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &cpu_id);


	if(cpu_id == 0)
	{
		std::string first, second;
		std::fstream file("numbers");

		std::getline(file, first);
		std::getline(file, second);

		int length_diff = abs(first.length() - second.length());

		if(first.length() < second.length())
			first = std::string(length_diff, '0') + first;
		else if(second.length() < first.length())
			second = std::string(length_diff, '0') + second;

	}

	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	start = MPI_Wtime();


	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	end = MPI_Wtime();
	fprintf(stderr, "%lf\n", end-start);

	MPI_Finalize();

	return 0;
}
