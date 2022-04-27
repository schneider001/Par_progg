#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include "mpi.h"

#define NUM_OF_THREADS 16
#define TABLESIZE 300

double psi(double t);
double phi(double x);
double fun(double t, double x);
int one_d(int i, int j, int tablesize);
int *two_d(int k, int tablesize);
int **alloc_2d_int(int rows, int cols);
int **calculation_seq(int tablesize);
void calculation_values(double *table, int *threads_id, int tablesize, int noderank, int seq);


int main(void)
{
    int nodesize, noderank;
    int size, rank;
    int tablesize, localtablesize;
    double *table, *localtable;

    MPI_Comm allcomm, nodecomm;

    MPI_Aint winsize;
    int windisp;

    allcomm = MPI_COMM_WORLD;

    MPI_Win wintable;

    tablesize = TABLESIZE;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(allcomm, &size);
    MPI_Comm_rank(allcomm, &rank);

    MPI_Comm_split_type(allcomm, MPI_COMM_TYPE_SHARED, rank,
                        MPI_INFO_NULL, &nodecomm);

    MPI_Comm_size(nodecomm, &nodesize);
    MPI_Comm_rank(nodecomm, &noderank);

    localtablesize = 0;

    if (noderank == 0)
        localtablesize = tablesize;

    MPI_Win_allocate_shared(localtablesize * localtablesize * sizeof(double), sizeof(double),
                            MPI_INFO_NULL, nodecomm, &localtable, &wintable);

    table = localtable;

    if (noderank != 0)
    {
        MPI_Win_shared_query(wintable, 0, &winsize, &windisp, &table);
    }

    double startwtime = 0.0;
    double endwtime;

    int cols = ((tablesize - 1) * (tablesize - 1)
                                    / NUM_OF_THREADS) + 1;

    double X = 1;
    double T = 1;
    double a = 2;

    double h = X / tablesize;
    double d = T / tablesize;

    int **threads_id;
    int **threads_id_local;
    
    if (noderank == 0)
    {   
        for (int k = 0; k < tablesize * tablesize; k++)
        {
            table[k] = -6666.0;
        }

        for (int j = 0; j < tablesize; j++)
        {
            table[one_d(0, j, tablesize)] = psi(j * d);
        }

        for (int i = 0; i < tablesize; i++)
        {
            table[one_d(i, 0, tablesize)] = phi(i * h);
        }

        threads_id = calculation_seq(tablesize);

        for (int n = 1; n < NUM_OF_THREADS; n++)
        {
            MPI_Send(&(threads_id[0][0]), NUM_OF_THREADS * cols, MPI_INT, n, 0, nodecomm);
        }
    }

    if (noderank != 0)
    {
        threads_id_local = alloc_2d_int(NUM_OF_THREADS, cols);
        MPI_Recv(&(threads_id_local[0][0]), NUM_OF_THREADS * cols, MPI_INT, 0, 0, nodecomm,
                 MPI_STATUS_IGNORE);
    }

    if (noderank == 0)
    { 
        startwtime = MPI_Wtime();

        for (int seq = 0; seq < cols; seq++)
        {
            calculation_values(table, threads_id[0], tablesize, noderank, seq);
        }

        //for (int k = 0; k < tablesize * tablesize; k++)
            //{
                //printf("%10.5lf  ", table[k]);
                //if ((k + 1) % tablesize == 0)
                //{
                    //printf("\n");
                //}
            //}
            //printf("\n\n");
    }

    if (noderank != 0)
    {
        for (int seq = 0; seq < cols; seq++)
        {
        calculation_values(table, threads_id_local[noderank], tablesize, noderank, seq);
        }
    }


    /*
    if (noderank == 1)
    {
        int *threads_id1 = (int *)calloc(cols, sizeof(int));
        MPI_Recv(threads_id1, cols, MPI_INT, 0, 0, nodecomm,
                 MPI_STATUS_IGNORE);

        int seq = 0;
        while(seq < cols) {
        MPI_Recv(&seq,  1, MPI_INT, 0, 0, nodecomm,
                MPI_STATUS_IGNORE);
        calculation_values(table, threads_id1, tablesize, noderank, seq);
        }
    }

    if (noderank == 2)
    {
        int *threads_id2 = (int *)calloc(cols, sizeof(int));
        MPI_Recv(threads_id2, cols, MPI_INT, 0, 0, nodecomm,
                 MPI_STATUS_IGNORE);

        int seq = 0;
        while(seq < cols) {
        MPI_Recv(&seq,  1, MPI_INT, 0, 0, nodecomm,
                MPI_STATUS_IGNORE);
        calculation_values(table, threads_id2, tablesize, noderank, seq);
        }
    }

    if (noderank == 3)
    {
        int *threads_id3 = (int *)calloc(cols, sizeof(int));
        MPI_Recv(threads_id3, cols, MPI_INT, 0, 0, nodecomm,
                 MPI_STATUS_IGNORE);

        int seq = 0;
        while(seq < cols) {
        MPI_Recv(&seq,  1, MPI_INT, 0, 0, nodecomm,
                MPI_STATUS_IGNORE);
        calculation_values(table, threads_id3, tablesize, noderank, seq);
        }
    }
    */

    if (noderank == 0) {
        //for (int k = 0; k < tablesize * tablesize; k++)
        //{
            endwtime = MPI_Wtime();
            printf("Time of calc = %lf\n", endwtime-startwtime); 
            printf("Last el = %10.5lf\n", table[tablesize * tablesize - 1]);
            //if ((k + 1) % tablesize == 0)
           // {
            //}
        //}
    }

    MPI_Finalize();
}


double psi(double t)
{
    return exp(-t);
}


double phi(double x)
{
    return cos(M_PI * x);
}


double fun(double t, double x)
{
    return x + t;
}


int one_d(int i, int j, int tablesize)
{
    return i * tablesize + j;
}


int *two_d(int k, int tablesize) {
    int *indexes = (int *)calloc(2, sizeof(int));
    indexes[0] = k / tablesize;
    indexes[1] = k % tablesize;

    return indexes;
}


int **alloc_2d_int(int rows, int cols) {
    int *data = (int *)calloc(rows*cols, sizeof(int));
    int **array= (int **)calloc(rows, sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}


int **calculation_seq(int tablesize)
{
    int cols = ((tablesize - 1) * (tablesize - 1)
                                    / NUM_OF_THREADS) + 1;

    int **threads_id = alloc_2d_int(NUM_OF_THREADS, cols);

    for (int n = 0; n < NUM_OF_THREADS; n++)
    {
        for (int k = 0; k < cols; k++)
        {
            threads_id[n][k] = -1;
        }
    }

    threads_id[0][0] = one_d(1, 1, tablesize);

    int i = 1;
    int j = 1;
    int n = 0;
    int sum = i + j;
    while (!(j == i && j == tablesize - 1))
    {
        if (j == tablesize - 1)
        {
            int sw = i;
            i = j;
            j = sw + 1;
        }
        else if (i != 1)
        {
            i--;
            j++;
        }
        else
        {
            i = sum;
            j = 1;
            sum++;
        }
        n++;

        for (int k = 0; k < cols; k++)
        {
            if (threads_id[n % NUM_OF_THREADS][k] == -1)
            {   
                threads_id[n % NUM_OF_THREADS][k] = one_d(i, j, tablesize);
                break;
            }
        }
    }

    return threads_id;
}


void calculation_values(double *table, int *threads_id, int tablesize, int noderank, int seq)
{
    double X = 1;
    double T = 1;
    double a = 2;

    double h = X / tablesize;
    double d = T / tablesize;

    int k = threads_id[seq];
    if (k == -1) {
        return;
    }

    int *indexes = two_d(k, tablesize);
    int i = indexes[0];
    int j = indexes[1];

    while (table[one_d(i - 1, j - 1, tablesize)] == -6666.0 || 
           table[one_d(i - 1, j, tablesize)] == -6666.0 ||
           table[one_d(i, j - 1, tablesize)] == -6666.0) {}

    table[one_d(i, j, tablesize)] = 
        (2 * fun((j - 0.5) * d, (i - 0.5) * h)
        -1 / d * (table[one_d(i - 1, j, tablesize)]
        -table[one_d(i - 1, j - 1, tablesize)] 
        -table[one_d(i, j - 1, tablesize)])
        -1 / h * a * (table[one_d(i, j - 1, tablesize)] 
        -table[one_d(i - 1, j - 1, tablesize)]
        -table[one_d(i - 1, j, tablesize)]))
        / (1 / d + a / h);
}




/*void calculation_values(double *table, int *threads_id, int tablesize, int noderank)
{
    double X = 1;
    double T = 1;
    double a = 2;

    double h = X / tablesize;
    double d = T / tablesize;

    int k = 0;

    int cols = ((tablesize - 1) * (tablesize - 1)
                                    / NUM_OF_THREADS) + 1;

        

    for (int i = 0; i < tablesize; i++)
    {
        for (int j = 0; j < tablesize; j++)
        {   
            if (threads_id[k] == one_d(i + 1, j + 1, tablesize)) {
                k++;

                while (table[one_d(i, j, tablesize)] == -6666.0 ||
                       table[one_d(i + 1, j, tablesize)] == -6666.0 ||
                       table[one_d(i, j + 1, tablesize)] == -6666.0)
                       {
                           printf("%d  %d    %d\n", i + 1, j + 1, noderank);
                       }

                table[one_d(i + 1, j + 1, tablesize)] = 
                    (2 * fun((j + 0.5) * d, (i + 0.5) * h)
                    -1 / d * (table[one_d(i, j + 1, tablesize)]
                    -table[one_d(i, j, tablesize)] 
                    -table[one_d(i + 1, j, tablesize)])
                    -1 / h * a * (table[one_d(i + 1, j, tablesize)] 
                    -table[one_d(i, j, tablesize)]
                    -table[one_d(i, j + 1, tablesize)]))
                    / (1 / d + a / h);
            }
        }
    }
    
    printf("%d\n", noderank);
    for (int k = 0; k < tablesize * tablesize; k++)
        {
            printf("%15lf  ", table[k]);
            if ((k + 1) % tablesize == 0)
            {
                printf("\n");
            }
        }
}
*/
/*int *calculation_seq(int tablesize)
{
    int *threads_id = (int *)calloc(tablesize * tablesize, sizeof(int));
    for (int i = 0; i < tablesize * tablesize; i++)
    {
        threads_id[i] = -1;
    }
    threads_id[tablesize + 1] = 0;

    int i = 1;
    int j = 1;
    int n = 0;
    int sum = i + j;
    while (!(j == i && j == tablesize - 1))
    {
        if (j == tablesize - 1)
        {
            int sw = i;
            i = j;
            j = sw + 1;
        }
        else if (i != 1)
        {
            i--;
            j++;
        }
        else
        {
            i = sum;
            j = 1;
            sum++;
        }
        n++;
        threads_id[one_d(i, j, tablesize)] = n % 4;
    }

    return threads_id;
}*/