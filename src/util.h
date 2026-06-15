
// file: util.h

#pragma once

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define swap(a, b) do { \
  typeof(a) _t = (a);   \
  (a) = (b);            \
  (b) = _t;             \
} while (0)

void die(const char *msg, ...);
