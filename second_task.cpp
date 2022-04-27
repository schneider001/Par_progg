#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  int n;
  int myid;
  int numprocs;
  int i;
  int rc;
  long double drob, drobSum=0, Result, sum;
 
  n = atoi(argv[1]);

  if (rc= MPI_Init(&argc, &argv)) 
  { 
    cout << "Ошибка запуска, выполнение остановлено " << endl;
    MPI_Abort(MPI_COMM_WORLD, rc);
  } 

MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
MPI_Comm_rank(MPI_COMM_WORLD,&myid); 

printf("Communicator size= %d My rank= %d\n", numprocs, myid);

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  for (i = myid + 1; i <= n; i += numprocs)
  { 
    drob = 1. / i;
    drobSum += drob;
  }

MPI_Reduce(&drobSum, &Result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
cout.precision(20);
  if (myid == 0)
  {   
    cout << Result << endl;   
  }

MPI_Finalize();
return 0;
}