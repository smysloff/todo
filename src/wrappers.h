
// file: wrappers.h

#pragma once

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util.h"

void *calloc_or_die(size_t n, size_t size);
void fclose_or_die(FILE *stream);
char *fgets_or_die(char *s, int size, FILE *stream);
FILE *fopen_or_die(const char *path, const char *mode);
void fprintf_or_die(FILE *stream, const char *format, ...);
void fputc_or_die(int c, FILE *stream);
void fputs_or_die(const char *s, FILE *stream);
char *getenv_or_die(const char *name);
void *malloc_or_die(size_t size);
void mkdir_or_die(const char *path, mode_t mode);
void *realloc_or_die(void *p, size_t size);
void snprintf_or_die(char *str, size_t size, const char *format, ...);
