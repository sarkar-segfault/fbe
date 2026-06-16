#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef MAX_IFS
#define MAX_IFS 32
#endif

#ifndef MAX_ELSES
#define MAX_ELSES MAX_IFS
#endif

typedef struct if_t {
  size_t divisor;
  const char *string;
} if_t;

typedef const char *else_t;

typedef struct opts_t {
  if_t ifs[MAX_IFS];
  else_t elses[MAX_ELSES];
  const char *separator;
  size_t from, to;
} opts_t;

void fb(const opts_t opts) {
  bool matched;
  size_t i;

  for (i = opts.from; i <= opts.to; i++) {
    matched = false;

    for (size_t j = 0; j < MAX_IFS; j++) {
      if_t it = opts.ifs[j];

      if (!it.string)
        break;

      if (i % it.divisor == 0) {
        printf("%s%s", it.string, opts.separator);
        matched = true;
      }
    }

    if (!matched) {
      for (size_t k = 0; k < MAX_ELSES; k++) {
        else_t it = opts.elses[k];

        if (!it)
          break;

        if (!strcmp(it, "$"))
          printf("%zu%s", i, opts.separator);
        else
          printf("%s%s", it, opts.separator);
      }
    }
  }
}

int main(const int argc, const char *const *argv) {
  opts_t opts = (opts_t){0};

  if (argc < 2) {
    opts.ifs[0] = (if_t){.divisor = 3, .string = "Fizz"};
    opts.ifs[1] = (if_t){.divisor = 5, .string = "Buzz"};
    opts.ifs[2] = (if_t){.divisor = 15, .string = "FizzBuzz"};
    opts.separator = "\n";
    opts.elses[0] = "$";
    opts.to = 100;
    opts.from = 1;
  }

  fb(opts);

  return 0;
}
