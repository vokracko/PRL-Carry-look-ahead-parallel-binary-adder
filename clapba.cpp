#include <mpi.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <cstdlib>

#define TAG 0
#define SEND(number, id) MPI_Send(&number, 1, MPI_INT, id, TAG, MPI_COMM_WORLD)
#define RECV(number, id) MPI_Recv(&number, 1, MPI_INT, id, TAG, MPI_COMM_WORLD, &stat)
#define PARENT ((cpu_id - 1) / 2)
#define LEFT_CHILD (cpu_id * 2 + 1)
#define RIGHT_CHILD (cpu_id * 2 + 2)
#define STOP 0
#define GENERATE 1
#define PROPAGATE 2

bool leaf(int i, int n)
{
	return i >= (n-1);
}

int op(int left, int right)
{
		if(right == STOP)
			return STOP;
		else if(right == PROPAGATE)
			return left;
		else
			return GENERATE;
}

int main(int argc, char * argv[])
{
	MPI_Status stat;
	int cpu_id;
	int p_count;
	int n;
	double start, end;
	int x, y;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &cpu_id);

	n = p_count / 2 + 1;

	if(cpu_id == 0)
	{
		int recv_id = p_count - 1;
		std::string first, second;
		std::fstream file("numbers");

		std::getline(file, first);
		std::getline(file, second);

		int length_diff = abs(first.length() - second.length());


		if(first.length() < second.length())
			first = std::string(length_diff, '0') + first;
		else if(second.length() < first.length())
			second = std::string(length_diff, '0') + second;

		for(int i = 0; i < first.length(); ++i)
		{
			x = first[i] == '1';
			y = second[i] == '1';
			SEND(x, recv_id);
			SEND(y, recv_id);
			recv_id--;
		}
	}

	int msb_proc = cpu_id == p_count - 1;
	int val;

	#ifdef TIMEBENCH
		MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
		start = MPI_Wtime();
	#endif

	if(leaf(cpu_id, n))
	{
		RECV(x, 0);
		RECV(y, 0);

		if(x == 1 && y == 1)
			val = GENERATE;
		else if(x == 0 && y == 0)
			val = STOP;
		else
			val = PROPAGATE;

		SEND(val, PARENT);
		RECV(val, PARENT);
	}
	else
	{
		int left, right;
		RECV(left, LEFT_CHILD);
		RECV(right, RIGHT_CHILD);

		val = op(left, right);

		if(cpu_id != 0)
		{
			SEND(val, PARENT);
			RECV(val, PARENT);
		}
		else
			val = PROPAGATE;

		right = op(val, left);
		left = val;
		SEND(left, LEFT_CHILD);
		SEND(right, RIGHT_CHILD);
	}

	if(leaf(cpu_id, n))
	{
		if(val == PROPAGATE)
			val = STOP;

		int bit = x + y + val;
		printf("%d:%d\n", cpu_id, bit % 2);

		if(msb_proc && bit > 1)
			printf("overflow\n");
	}

	#ifdef TIMEBENCH
		MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
		end = MPI_Wtime();
		fprintf(stderr, "%lf\n", end-start);
	#endif

	MPI_Finalize();

	return 0;
}
