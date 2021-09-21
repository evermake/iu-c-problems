/*
  Program that reads unsigned long long int from stdin,
  encodes and decodes by swapping neighboring bytes using
  union.
*/

#include <stdio.h>

typedef unsigned long long int ulli;

union Number {
  ulli number;
  char bytes[sizeof(ulli)];
};

void encrypt(union Number *num) {
  for (size_t i = 0; i < sizeof(union Number) - 1; i+=2) {
    char tmp = num->bytes[i];
    num->bytes[i] = num->bytes[i+1];
    num->bytes[i+1] = tmp;
  }
}

int main() {
  union Number num;
  
  printf("Enter number: ");
  scanf("%lli", &num.number);

  encrypt(&num);
  printf("Encrypted value: %llu\n", num.number);

  encrypt(&num);
  printf("Decrypted value: %llu\n", num.number);
}

