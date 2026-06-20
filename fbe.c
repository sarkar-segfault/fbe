#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_IFS
#define MAX_IFS 32
#endif

#ifndef MAX_ELSES
#define MAX_ELSES MAX_IFS
#endif

#define FAIL_FAST(msg)                                                                                                         \
  {                                                                                                                            \
    fputs(msg "\n", stderr);                                                                                                   \
    exit(EXIT_FAILURE);                                                                                                        \
  }

#define PASS_FAST(msg)                                                                                                         \
  {                                                                                                                            \
    fputs(msg "\n", stderr);                                                                                                   \
    exit(EXIT_SUCCESS);                                                                                                        \
  }

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
  if (opts.to < opts.from)
    FAIL_FAST("-t is smaller than -f");

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
        break;
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
  opts_t opts = (opts_t){.separator = "\n", .from = 1, .to = 100};

  if (argc < 2) {
    opts.ifs[0] = (if_t){.divisor = 15, .string = "FizzBuzz"};
    opts.ifs[1] = (if_t){.divisor = 5, .string = "Buzz"};
    opts.ifs[2] = (if_t){.divisor = 3, .string = "Fizz"};
    opts.separator = "\n";
    opts.elses[0] = "$";

    fb(opts);
    return 0;
  }

  size_t ifno = 0;
  size_t elseno = 0;

  for (int i = 1; i < argc; i++) {
    const char *arg = argv[i];

    if (!strcmp(arg, "-h")) {
      PASS_FAST("fbe (fizzbuzz engine)\n"
                "when not given any options, runs the standard fizzbuzz\n"
                "options\n"
                "     -h                     print this message and exit\n"
                "     -i <divisor> <message> add if condition of <divisor> and <message>\n"
                "     -e <message>           add an else block that runs if no ifs match\n"
                "     -t <number>            configure the number at which counting ends\n"
                "     -f <number>            configure the number at which counting starts\n"
                "     -s <separator>         configure the separator used for printing matches");
    }

    else if (!strcmp(arg, "-i")) {
      if (ifno >= MAX_IFS)
        FAIL_FAST("too many -i <divisor> <message>");

      const char *divisor = argv[++i];
      if (!divisor)
        FAIL_FAST("no <divisor> after -i");

      const char *message = argv[++i];
      if (!message)
        FAIL_FAST("no <message> after -i <divisor>");

      char *end;
      errno = 0;
      size_t div = strtoull(divisor, &end, 10);

      if (errno || end == divisor)
        FAIL_FAST("invalid <divisor> in -i <divisor> <message>");

      opts.ifs[ifno++] = (if_t){.divisor = div, .string = message};
    }

    else if (!strcmp(arg, "-e")) {
      if (ifno >= MAX_IFS)
        FAIL_FAST("too many -e <message>");

      const char *message = argv[++i];
      if (!message)
        FAIL_FAST("no <message> after -e");

      opts.elses[elseno++] = (else_t)message;
    }

    else if (!strcmp(arg, "-t")) {
      const char *to = argv[++i];
      if (!to)
        FAIL_FAST("no <number> after -t");

      char *end;
      errno = 0;
      size_t t = strtoull(to, &end, 10);

      if (errno || end == to)
        FAIL_FAST("invalid <number> in -t <number>");
      opts.to = t;
    }

    else if (!strcmp(arg, "-f")) {
      const char *from = argv[++i];
      if (!from)
        FAIL_FAST("no <number> after -f");

      char *end;
      errno = 0;
      size_t f = strtoull(from, &end, 10);

      if (errno || end == from)
        FAIL_FAST("invalid <number> in -f <number>");
      opts.from = f;
    }

    else if (!strcmp(arg, "-s")) {
      const char *separator = argv[++i];
      if (!separator)
        FAIL_FAST("no <separator> after -s");

      opts.separator = separator;
    }
  }

  fb(opts);
  return 0;
}
