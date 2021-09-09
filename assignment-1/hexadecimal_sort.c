#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE_PATH  "input.txt"
#define OUTPUT_FILE_PATH "output.txt" 


void sort(int *arr, size_t *len) {
  for (size_t i = 1; i < *len; ++i) {
    size_t k = i;
    while (k > 0 && arr[k-1] > arr[k]) {
      int tmp = arr[k-1];
      arr[k-1] = arr[k];
      arr[k] = tmp;
      --k;
    }
  }
}


int main() {
  FILE *fi, *fo;

  if ((fi = fopen(INPUT_FILE_PATH, "r")) == NULL) {
    printf("Failed to open input file!");
    return 1;
  }

  if ((fo = fopen(OUTPUT_FILE_PATH, "w")) == NULL) {
      printf("Failed to open output file!");
      fclose(fi);
      return 1;
  }

  size_t len = 0, size = 8;
  int *nums = malloc(size * sizeof(int));

  // Read hexadecimal numbers from file
  while (fscanf(fi, "%x", &nums[len++]) == 1) {
    // Allocate more memory, if it's necessary
    if (len >= size - 1) {
      size += 8;
      nums = realloc(nums, size * sizeof(int));
    }
  }

  // Decrease length by 1, because it was increased
  // on the last fscanf failed try of getting number.
  --len;

  sort(nums, &len);

  // Write to output file
  for (size_t i = 0; i < len; ++i) {
    fprintf(fo, "%X ", nums[i]);
  }
  fprintf(fo, "\n");
  
  free(nums);
  fclose(fi);
  fclose(fo);
}

