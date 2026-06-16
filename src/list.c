
// file: list.c

#include "list.h"

node_t *
node_alloc(const char *value)
{
  node_t *node;
  size_t size;

  node = calloc_or_die(1, sizeof(*node));

  if (value) {
    size = strlen(value) + 1;
    node->value = malloc_or_die(size);
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

  node = node_alloc(value);

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

void
list_foreach(list_t list, void (cb)(node_t *node))
{
  for (; list; list = list->next)
    cb(list);
}

node_t *
list_index(list_t list, size_t index) {
  for (size_t i = 1; list && i < index; ++i)
    list = list->next;
  return list;
}
