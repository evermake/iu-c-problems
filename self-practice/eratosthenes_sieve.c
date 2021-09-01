#include <stdio.h>
#include <stdbool.h>

int main() {
	printf("Enter required max n:\n");
	int n;
	scanf("%d", &n);

	bool sieve[n];
	for (int i = 0; i < n; i++) {
		sieve[i] = 1;
	}

	for (int i = 2; i*i < n; i++) {
		if (!sieve[i]) continue;
		for (int k = i*2; k < n; k += i) {
			sieve[k] = 0;
		}
	}

	printf("\nPrime numbers from 2 to %d:\n", n);
	for (int i = 2; i < n; i++) {
		if (sieve[i]) printf(" %d ", i); 
	}
	printf("\n");
}

