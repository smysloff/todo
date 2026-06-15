
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

node_t *node_alloc(const char *value);
void node_free(node_t *node);
node_t *list_tail(list_t list);
node_t *list_push(list_t *list, const char *value);
void list_free(list_t list);
