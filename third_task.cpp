#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int commsize, world_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int number;
    if (world_rank == 0)
    {
        number = 0;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent number %d to process 1\n", number);
        MPI_Recv(&number, 1, MPI_INT, 9, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 0 received number %d from process 9\n",
               number);
    }
    else if (world_rank == 1)
    {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        printf("Process 1 sent number %d to process 2\n", number);
    }
    else if (world_rank == 2)
    {
        MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 2 received number %d from process 1\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        printf("Process 2 sent number %d to process 3\n", number);
    }
    else if (world_rank == 3)
    {
        MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 3 received number %d from process 2\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
        printf("Process 3 sent number %d to process 4\n", number);
    }
    else if (world_rank == 4)
    {
        MPI_Recv(&number, 1, MPI_INT, 3, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 4 received number %d from process 3\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
        printf("Process 4 sent number %d to process 5\n", number);
    }
    else if (world_rank == 5)
    {
        MPI_Recv(&number, 1, MPI_INT, 4, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 5 received number %d from process 4\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 6, 0, MPI_COMM_WORLD);
        printf("Process 5 sent number %d to process 6\n", number);
    }
    else if (world_rank == 6)
    {
        MPI_Recv(&number, 1, MPI_INT, 5, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 6 received number %d from process 5\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 7, 0, MPI_COMM_WORLD);
        printf("Process 6 sent number %d to process 7\n", number);
    }
    else if (world_rank == 7)
    {
        MPI_Recv(&number, 1, MPI_INT, 6, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 7 received number %d from process 6\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 8, 0, MPI_COMM_WORLD);
        printf("Process 7 sent number %d to process 8\n", number);
    }
    else if (world_rank == 8)
    {
        MPI_Recv(&number, 1, MPI_INT, 7, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 8 received number %d from process 7\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 9, 0, MPI_COMM_WORLD);
        printf("Process 8 sent number %d to process 9\n", number);
    }
    else if (world_rank == 9)
    {
        MPI_Recv(&number, 1, MPI_INT, 8, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 9 received number %d from process 8\n",
               number);
        number += world_rank;
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Process 9 sent number %d to process 0\n", number);
    }
    MPI_Finalize();
}