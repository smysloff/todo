
// file: list.h

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_s {
  struct node_s *next;
  char *value;
} node_t;

typedef node_t * list_t;

node_t *list_tail(list_t list);
node_t *list_push(list_t *list, const char *value);
