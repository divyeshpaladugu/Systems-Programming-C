#include <stdio.h>
#include <stdlib.h>

double two_d_random(int n)
{
    int visited[2 * n + 1][2 * n + 1];

    for (int i = 0; i < 2 * n + 1; i++) {
        for (int j = 0; j < 2 * n + 1; j++) {
            visited[i][j] = 0;
        }
    }

    int x = 0, y = 0;
    visited[n][n] = 1;

    while (1) {
        int r = rand() % 4;

        if (r == 0) y -= 1; 
        else if (r == 1) x += 1; 
        else if (r == 2) y += 1; 
        else if (r == 3) x -= 1; 

        if (x == -n || x == n || y == -n || y == n) {
            break; 
        }

        visited[x + n][y + n] = 1;
    }

    int total_points = (2 * n - 1) * (2 * n - 1);
    int visited_count = 0;

    for (int i = 1; i < 2 * n; i++) {
        for (int j = 1; j < 2 * n; j++) {
            if (visited[i][j] == 1) {
                visited_count++;
            }
        }
    }

    double fraction_visited = (double)visited_count / total_points;

    return fraction_visited;
}

// Do not change the code below
int main(int argc, char *argv[])
{
    int trials = 1000;
    int i, n, seed;
    if (argc == 2) seed = atoi(argv[1]);
    else seed = 12345;

    srand(seed);
    for(n=1; n<=64; n*=2)
    {
        double sum = 0.;
        for(i=0; i < trials; i++)
        {
            double p = two_d_random(n);
            sum += p;
        }
        printf("%d %.3lf\n", n, sum/trials);
    }
    return 0;
}
