
// file: wrappers.c

#include "wrappers.h"

char *
Getenv(const char *name)
{
  char *s;
  if (!(s = getenv(name)))
    die("getenv()");
  return s;
}

void
Snprintf(char *str, size_t size, const char *restrict format, ...)
{
  va_list ap;
  va_start(ap, format);
  if (vsnprintf(str, size, format, ap) < 0)
    die("snprintf()");
  va_end(ap);
}

void
Mkdir(const char *path, mode_t mode)
{
  if (mkdir(path, mode) == -1 && errno != EEXIST)
    die("mkdir()");
}

void
Fclose(FILE *stream)
{
  if (fclose(stream) == EOF)
    die("fclose()");
}

FILE *
Fopen(const char *restrict path, const char *restrict mode)
{
  FILE *stream;
  if (!(stream = fopen(path, mode)))
    die("fopen()");
  return stream;
}

void
Fputc(int c, FILE *stream)
{
  if (fputc(c, stream) == EOF)
    die("fputc()");
}

void
Fputs(const char *restrict s, FILE *restrict stream)
{
  if (fputs(s, stream) == EOF)
    die("fputs()");
}

void Fprintf(FILE *restrict stream, const char *restrict format, ...)
{
  va_list ap;
  va_start(ap, format);
  if (vfprintf(stream, format, ap) < 0)
    die("fprintf()");
  va_end(ap);
}

char *
Fgets(char *s, int size, FILE *restrict stream)
{
  if (!fgets(s, size, stream)) {
    if (ferror(stream))
      die("fgets()");
    return NULL;
  }
  return s;
}

long long
Strtoll(const char *restrict nptr, char **restrict endptr, int base)
{
  long long res;

  errno = 0;
  res = strtoll(nptr, endptr, base);

  if (endptr && *endptr == nptr)
    die("strtoll(): no digits found");

  if (errno == ERANGE)
    die("strtoll(): out of range");

  return res;
}
