
// file: list.c

#include "list.h"

node_t *
node_alloc(const char *value)
{
  node_t *node;
  size_t size;

  if (!(node = calloc(1, sizeof(*node))))
    return NULL;

  if (value) {
    size = strlen(value) + 1;
    if (!(node->value = malloc(size))) {
      free(node);
      return NULL;
    }
    memcpy(node->value, value, size);
  }

  return node;
}

void
node_free(node_t *node)
{
  if (!node) return;
  free(node->value);
  free(node);
}

node_t *
list_tail(list_t list)
{
  if (list)
    while (list->next)
      list = list->next;
  return list;
}

node_t *
list_push(list_t *list_p, const char *value)
{
  node_t *node, *tail;

  if (!list_p)
    return NULL;

  if (!(node = node_alloc(value)))
    return NULL;

  if (*list_p) {
    tail = list_tail(*list_p);
    tail->next = node;
  }
  else
    *list_p = node;

  return node;
}

void
list_free(list_t list)
{
  node_t *node;
  while (list) {
    node = list;
    list = list->next;
    node_free(node);
  }
}
