#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>


double psi(double t);
double phi(double x);
double fun(double t, double x);


int main()
{
    double X, T;
    int I, J;
    int a;

    std::cin >> X >> T >> I >> J >> a;

    double h = X / I;
    double d = T / J;

    std::vector<std::vector<double>> y;
    y.resize(I);

    for (int j = 0; j < J; j++)
    {
        y[j].resize(J);
        y[0][j] = psi(j * d);
    }

    for (int i = 0; i < I; i++)
    {
        y[i][0] = phi(i * h);
    }

    clock_t starttime = clock();

    for (int j = 0; j < J - 1; j++)
    {
        for (int i = 0; i < I - 1; i++)
        {
            y[i + 1][j + 1] = (2 * fun((j + 0.5) * d, (i + 0.5) * h) - 1 / d * (y[i][j + 1] - y[i][j] - y[i + 1][j]) - 1 / h * a * (y[i + 1][j] - y[i][j] - y[i][j + 1])) / (1 / d + a / h);
        }
    }
    clock_t endtime = clock();
    printf("Time of calc = %lf\n", (double) (endtime-starttime) / CLOCKS_PER_SEC);
    //for (int i = 0; i < I; i++)
    //{
        //for (int j = 0; j < J; j++)
        //{
    printf("Last el = %10.5lf\n", y[I - 1][J - 1]);
        //}
        //printf("\n");
    //}
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