
// file: list.c

#include "list.h"

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
  node_t *node;
  size_t len;

  if (!list_p || !value)
    return NULL;

  if (!(node = malloc(sizeof(node_t))))
    return NULL;

  len = strlen(value);

  if (!(node->value = malloc(len + 1))) {
    free(node);
    return NULL;
  }
  memcpy(node->value, value, len);
  node->value[len] = '\0';

  node->next = NULL;

  *list_p
    ? (list_tail(*list_p)->next = node)
    : (*list_p = node);

  return node;
}
