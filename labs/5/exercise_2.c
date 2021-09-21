/*
  Just basics of structures in C.
*/

#include <stdio.h>

struct ExamDay {
  unsigned int year;
  char month[15];
  unsigned int day;
};

struct Student {
  char name[30];
  char surname[30];
  unsigned int groupNo;
  struct ExamDay examDay;
};


void print_exam_day(struct ExamDay examDay) {
  printf("  Exam day: %u %s %u\n",
      examDay.day,
      examDay.month,
      examDay.year);
}


void print_student(struct Student student) {
  printf("Student name: %s\n", student.name);
  printf("Student surname: %s\n", student.surname);
  printf("Student group number: %u\n", student.groupNo);
  printf("Student exam day:\n");
  print_exam_day(student.examDay);
}


int main() {
  struct Student student;

  printf("Please enter student name: ");
  scanf("%s", student.name);

  printf("Surname: ");
  scanf("%s", student.surname);

  printf("Group number: ");
  scanf("%u", &student.groupNo);

  printf("And now exam day...\n");
  printf("Year: ");
  scanf("%u", &student.examDay.year);

  printf("Month: ");
  scanf("%s", student.examDay.month);

  printf("Day: ");
  scanf("%u", &student.examDay.day);

  printf("\n---------------\n");
  print_student(student);
}

