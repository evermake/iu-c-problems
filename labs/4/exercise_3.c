/*
  Reads integers from stdin separated by spaces until end 
  of string or line and writes these numbers to stdout without
  duplicates.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
  size_t size = 8, len = 0;
  int *nums = malloc(size * sizeof(int));

  int n;
  char c;
  // Read numbers to array
  while (scanf("%d%c", &n, &c)) {
    // Allocate more memory, if necessary
    if (len >= size - 1) {
      nums = realloc(nums, (size += 8) * sizeof(int));
    }
    nums[len++] = n;

    // End reading if last char is end of string or line
    if (c == '\0' || c == '\n') break;
  }
  
  // Create an array of existing numbers
  int *existing = malloc(len * sizeof(int));
  size_t existing_len = 0;

  for (size_t i = 0; i < len; ++i) {
    int num = nums[i];

    // Check is a number already exists
    bool is_duplicate = false;
    for (size_t k = 0; k < existing_len; ++k) {
      if (existing[k] == num) {
        is_duplicate = true;
        break;
      }
    }

    // If it's not, add it to array of existing numbers and print it
    if (!is_duplicate) {
      existing[existing_len++] = num;
      printf("%d ", num);
    }
  }
  printf("\n");

  free(nums);
  free(existing);
}

