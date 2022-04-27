#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int world_size, world_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int number;

    if (world_rank != 0)
    {
        MPI_Recv(&number, 1, MPI_INT, world_rank - 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received number %d from process %d\n",
               world_rank, number, world_rank - 1);
        number+=world_rank;
    }
    else
    {
        // Set the number's value if you are process 0
        number = 0;
    }
    MPI_Send(&number, 1, MPI_INT, (world_rank + 1) % world_size,
             0, MPI_COMM_WORLD);

    // Now process 0 can receive from the last process.
    if (world_rank == 0)
    {
        MPI_Recv(&number, 1, MPI_INT, world_size - 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received number %d from process %d\n",
               world_rank, number, world_size - 1);
    }

    MPI_Finalize();
}