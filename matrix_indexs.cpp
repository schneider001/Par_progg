#include <iostream>

int main()
{

    int N = 5;
    int i = 1;
    int j = 1;

    int n = 0;

    int threads_id[(N+1)*(N+1)];
    for (int i = 0; i < (N+1)*(N+1); i++){
        threads_id[i] = -1;
    } 
    threads_id[N + 2] = 0;

    int sum = i + j;
    while (!(j == i && j == N))
    {
        if (j == N)
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
        threads_id[i*(N+1)+j] = n % 4;
    }

    for (int i = 0; i < (N+1)*(N+1); i++){
        printf("%2d  ", threads_id[i]);
        if ((i + 1 ) % (N + 1) == 0) {
            printf("\n");
        }
    } 
}