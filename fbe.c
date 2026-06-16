#include <stdbool.h>
#include <stdio.h>

int main(void) {
  bool printed;

  for (int i = 1; i <= 100; i++) {
    printed = false;

    if (i % 3 == 0) {
      printf("Fizz");
      printed = true;
    }
    if (i % 5 == 0) {
      printf("Buzz");
      printed = true;
    }
    if (!printed)
      printf("%d", i);

    printf("\n");
  }

  return 0;
}
