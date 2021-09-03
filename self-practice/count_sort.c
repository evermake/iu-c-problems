#include <stdio.h>

int main() {
	printf("Enter maximum value (1 - 1024):");
	size_t n;
	scanf("%d", &n);
	if (n < 1 || n > 1024) {
		printf("Incorrect value!");
		return 1;
	}
	n++;

	size_t N[n];
	for (size_t i = 0; i < n; i++) {
		N[i] = 0;
	}

	// Read array and count
	int x;
	while (1) {
		scanf("%d", &x);

		if (x == -1) break;
		else if (x < 0 || x > 1024) {
			printf("Incorrect value: %d", x);
		}

		N[x]++;
	}

	for (size_t i = 0; i < n; i++) {
		for (size_t k = 0; k < N[i]; k++) {
			printf(" %d ", i);
		}
	}
	printf("\n");
}

