#include <stdio.h>
#include <stdbool.h>


void swap(int *arr, size_t a, size_t b) {
  int tmp = arr[a];
  arr[a] = arr[b];
  arr[b] = tmp;
}



void sort_indices(int *arr, int *indices, size_t size) {
  /*
    Sorts array indices by values of array arr in descending order.
  */

  for (size_t i = 1; i < size; ++i) {
    size_t k = i;
    while (k > 0 && arr[indices[k - 1]] < arr[indices[k]]) {
      swap(indices, k - 1, k);
      --k;
    }
  }
}


int main() {
  int chars[128] = {0};
  int indices[128];
  for (size_t i = 0; i < 128; ++i) indices[i] = i;

  char c;
  while ((c = getchar()) != EOF) {
    if (c > 32) chars[c] += 1;
  }

  sort_indices(chars, indices, 128);

  size_t exists = 0;
  for (int k = 0; k < 128; ++k) {
    int i = indices[k];
    if (chars[i] == 0) break;
    printf("%c ", (char) i);
    ++exists;
    // printf("%c: %d\n", (char) i, chars[i]);
  }
  printf("\n");
  
  bool not_empty = true;
  while (not_empty) {
    not_empty = false;
    for (size_t k = 0; k < exists; ++k) {
      int i = indices[k];
      if (chars[i]) {
        --chars[i];
        not_empty = true;
        printf(". ");
      } else {
        printf("  ");
      }
    }
    printf("\n");
  }
}

