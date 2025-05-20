#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//TODO
//Implement the below function
//Simulate one particle moving n steps in random directions
//Use a random number generator to decide which way to go at every step
//When the particle stops at a final location, use the memory pointed to by grid to 
//record the number of particles that stop at this final location
//Feel free to declare, implement and use other functions when needed

void one_particle(int *grid, int n)
{


    int x = 0;
    int y = 0;
    int z = 0;

    for (int walk = 0; walk < n; walk++)
    {
        int r = rand() % 6;
		//random number 0-5 (6 outcomes)
		// number determins if moving up, down, left, right, front, back
        if (r == 0) x -= 1; 
        else if (r == 1) x += 1; 
        else if (r == 2) y -= 1; 
        else if (r == 3) y += 1;
        else if (r == 4) z -= 1;
        else if (r == 5) z += 1;

        if (x < -n) x = -n; // these make it stay inside the boundaries
        else if (x > n) x = n;

        if (y < -n) y = -n;
        else if (y > n) y = n;

        if (z < -n) z = -n;
        else if (z > n) z = n;

    }

    grid[((z + n) * (2 * n + 1) * (2 * n + 1)) + ((y + n) * (2 * n + 1)) + (x + n)] ++;
}

//TODO
//Implement the following function
//This function returns the fraction of particles that lie within the distance
//r*n from the origin (including particles exactly r*n away)
//The distance used here is Euclidean distance
//Note: you will not have access to math.h when submitting on Mimir                 

double density(int *grid, int n, double r)
{


    int all_places = 0;
    int inside = 0;

    for (int x = -n; x <= n; x++) { //checking all possible places on the grid using nested for loops
        for (int y = -n; y <= n; y++) {
            for (int z = -n; z <= n; z++) {
                if (grid[((z + n) * (2 * n + 1) * (2 * n + 1)) + ((y + n) * (2 * n + 1)) + (x + n)]>0) {
                    all_places += grid[((z + n) * (2 * n + 1) * (2 * n + 1)) + ((y + n) * (2 * n + 1)) + (x + n)];

					//squared euclidan distance from origin
                    int distance = (x*x) + (y*y) + (z*z);
					//square of radius
                    int square_of_radius = (r*n*r*n);

                    if (distance<=square_of_radius) {
                        inside += grid[((z + n) * (2 * n + 1) * (2 * n + 1)) + ((y + n) * (2 * n + 1)) + (x + n)];
                    }

                    }

            }          
        }
    }  
    return (double)inside/all_places;
}

//use this function to print results
void print_result(int *grid, int n)
{
    printf("radius density\n");
    for(int k = 1; k <= 20; k++)
    {
        printf("%.2lf   %lf\n", 0.05*k, density(grid, n, 0.05*k));
    }
}

//TODO
//Finish the following function
//See the assignment decription on Piazza for more details
void diffusion(int n, int m)
{
	//fill in a few line of code below

	//size of the grid depending on n
    int size_grid = (2*n+1)*(2*n+1)*(2*n+1);
	//memory allocation for grid
    int* grid = (int*)calloc(size_grid, sizeof(int));
	//if memory isnt allocated
    if (grid == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    //simulates movment of m particles using one_particle function
    for(int i = 1; i<=m; i++) one_particle(grid, n);

	print_result(grid, n);
	//fill in some code below

	//free memory after using calloc
    free(grid);

}

int main(int argc, char *argv[])
{
	
	if(argc != 3)
	{
		printf("Usage: %s n m\n", argv[0]);
		return 0; 
	}
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	assert(n >= 1 && n <=50);
	assert(m >= 1 && m <= 1000000);
	srand(12345);
	diffusion(n, m);
	return 0;
}