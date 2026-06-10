
// file: util.c

#include "util.h"

[[noreturn]] void
die(const char *msg, ...)
{
  va_list ap;

  fprintf(stderr, "error:");

  if (msg) {
    fprintf(stderr, " ");
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
  }

  if (errno)
    fprintf(stderr, "%s %s", msg ? "," : "", strerror(errno));

  fprintf(stderr, "\n");

  exit(EXIT_FAILURE);
}
