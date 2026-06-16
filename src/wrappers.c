
// file: wrappers.c

#include "wrappers.h"

void *
calloc_or_die(size_t n, size_t size)
{
  void *res;
  if (!(res = calloc(n, size)))
    die("calloc()");
  return res;
}

void
fclose_or_die(FILE *stream)
{
  if (fclose(stream) == EOF)
    die("fclose()");
}

char *
fgets_or_die(char *s, int size, FILE *restrict stream)
{
  if (!fgets(s, size, stream)) {
    if (ferror(stream))
      die("fgets()");
    return NULL;
  }
  return s;
}

FILE *
fopen_or_die(const char *restrict path, const char *restrict mode)
{
  FILE *stream;
  if (!(stream = fopen(path, mode)))
    die("fopen()");
  return stream;
}

void
fprintf_or_die(FILE *restrict stream, const char *restrict format, ...)
{
  va_list ap;
  va_start(ap, format);
  if (vfprintf(stream, format, ap) < 0)
    die("fprintf()");
  va_end(ap);
}

void
fputc_or_die(int c, FILE *stream)
{
  if (fputc(c, stream) == EOF)
    die("fputc()");
}

void
fputs_or_die(const char *restrict s, FILE *restrict stream)
{
  if (fputs(s, stream) == EOF)
    die("fputs()");
}

char *
getenv_or_die(const char *name)
{
  char *s;
  if (!(s = getenv(name)))
    die("getenv()");
  return s;
}

void *
malloc_or_die(size_t size)
{
  void *res;
  if (!(res = malloc(size)))
    die("malloc()");
  return res;
}

void
mkdir_or_die(const char *path, mode_t mode)
{
  if (mkdir(path, mode) == -1 && errno != EEXIST)
    die("mkdir()");
}

void *
realloc_or_die(void *p, size_t size)
{
  void *res;
  if (!(res = realloc(p, size)))
    die("realloc()");
  return res;
}

void
snprintf_or_die(char *str, size_t size, const char *restrict format, ...)
{
  va_list ap;
  va_start(ap, format);
  if (vsnprintf(str, size, format, ap) < 0)
    die("snprintf()");
  va_end(ap);
}
