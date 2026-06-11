
// file: wrappers.h

#pragma once

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util.h"

char *Getenv(const char *name);
void Snprintf(char *str, size_t size, const char *format, ...);
void Mkdir(const char *path, mode_t mode);
void Fclose(FILE *stream);
FILE *Fopen(const char *path, const char *mode);
void Fputc(int c, FILE *stream);
void Fputs(const char *s, FILE *stream);
void Fprintf(FILE *stream, const char *format, ...);
char *Fgets(char *s, int size, FILE *stream);
