#include <stdio.h>
#include <stdlib.h>

void insertion_sort(int arr[], const size_t n) {
	for (size_t i = 1; i < n; i++) {
		int k = i;
		while (k > 0 && arr[k] < arr[k-1]) {
			int tmp = arr[k-1];
			arr[k-1] = arr[k];
			arr[k] = tmp;
			k--;
		}
	}
}

int main() {
	printf("Enter numbers (0 - terminating):\n");

	size_t n = 0, size = 8 * sizeof(int);
	int *nums = malloc(size);
	if (nums == NULL) {
		printf("Memory error!\n");
		exit(1);
	}

	// Fill array
	int i;
	scanf("%d", &i);
	while (i) {
 		scanf("%d", &i);
		if (n >= size - 1) {
			size += 8 * sizeof(int);
			nums = realloc(nums, size);
			if (nums == NULL) {
				printf("Memory error!\n");
				exit(1);
			}
		}
		nums[n++] = i;
	}
	
	insertion_sort(nums, n);

	for (size_t k = 0; k < n; k++) {
		printf(" %d ", nums[k]);
	}
	printf("\n");
}

