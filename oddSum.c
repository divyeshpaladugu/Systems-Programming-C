#include <stdio.h>
#include <stdlib.h>

int oddSumHelp(int count, int bound, int value)
{

	// base cases
	if (count == 0 && value == 0) {
		return 1;
	}

	if (count == 0 || value < 0 || bound <= 0) {
		return 0;
	}

	// makes sure bound is odd
	if (bound % 2 == 0) bound -= 1;

	// include the largest odd number
	if (oddSumHelp(count - 1, bound - 2, value - bound)) {
		printf("%d ", bound);
		return 1;
	}

	// Try the next odd number (bound - 2)
	return oddSumHelp(count, bound - 2, value);
}

// Do not change the code below
void oddSum(int count, int bound, int value)
{
	if (value <= 0 || count <= 0 || bound <= 0) return;

	if (bound % 2 == 0) bound -= 1;

	if (!oddSumHelp(count, bound, value)) {
		printf("No solutions.\n");
	} else {
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	if (argc != 4) return -1;

	int count = atoi(argv[1]);
	int bound = atoi(argv[2]);
	int value = atoi(argv[3]);

	oddSum(count, bound, value);
	return 0;
}
