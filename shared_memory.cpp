#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

int main(void)
{
    int nodesize, noderank;
    int size, rank;
    int tablesize, localtablesize;
    int *table, *localtable;

    MPI_Comm allcomm, nodecomm;

    MPI_Aint winsize;
    int windisp;

    allcomm = MPI_COMM_WORLD;

    MPI_Win wintable;

    tablesize = 10;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(allcomm, &size);
    MPI_Comm_rank(allcomm, &rank);

    // Create node-local communicator

    MPI_Comm_split_type(allcomm, MPI_COMM_TYPE_SHARED, rank,
                        MPI_INFO_NULL, &nodecomm);

    MPI_Comm_size(nodecomm, &nodesize);
    MPI_Comm_rank(nodecomm, &noderank);

    // Only rank 0 on a node actually allocates memory

    localtablesize = 0;

    if (noderank == 0)
        localtablesize = tablesize;

    MPI_Win_allocate_shared(localtablesize * sizeof(int), sizeof(int),
                            MPI_INFO_NULL, nodecomm, &localtable, &wintable);

    // need to get local pointer valid for table on rank 0

    table = localtable;

    if (noderank != 0)
    {
        MPI_Win_shared_query(wintable, 0, &winsize, &windisp, &table);
    }

    // All table pointers should now point to copy on noderank 0

    // Initialise table on rank 0 with appropriate synchronisation

    MPI_Win_fence(0, wintable);

    MPI_Win_fence(0, wintable);

    if (noderank == 0)
    {
        for (int i = 0; i < tablesize; i++)
        {
            printf("table[%d] = %d\n", i, table[i]);
        }
    }

    MPI_Finalize();
}