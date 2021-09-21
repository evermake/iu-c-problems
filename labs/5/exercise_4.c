/*
  Program that writes user birthday and stores it in 2 bytes.
*/
#include <stdio.h>


struct Birthday {
  unsigned short day   : 5;  // 2^5 = 32 (minimum 31)
  unsigned short month : 4;  // 2^4 = 16 (minimum 12)
  unsigned short year  : 7;  // 2^7 = 128 (minimum 99)
};


int main() {
  struct Birthday birthday;
  unsigned short day, month, year;
  printf("Enter your birthday in format dd-mm-yy\n");
  scanf("%hu-%hu-%hu", &day, &month, &year);
  birthday.day = day;
  birthday.month = month;
  birthday.year = year;

  printf("Your birthday: %hu.%hu.%hu\n", birthday.day, birthday.month, birthday.year);
  printf("Bytes occupied: %lu\n", sizeof(birthday));
}

