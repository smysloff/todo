
// file: main.c

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "util.h"
#include "wrappers.h"

#include "config.h"


#ifndef PATH_MAX
# define PATH_MAX 4096
#endif


const char *get_state_dir(void);
const char *get_state_file(void);

void process(const char *app, const char *cmd, const char *val1, const char *val2);
void cmd_usage(const char *app);
void cmd_show(void);
void cmd_new(void);
void cmd_add(const char *value);
void cmd_del(const char *value);
void cmd_mov(const char *val1, const char *val2);
void cmd_swap(const char *val1, const char *val2);


int
main(int argc, char *argv[])
{
  switch (argc) {

    case 1:
      process(argv[0], "show", NULL, NULL);
      break;

    case 2:
      process(argv[0], argv[1], NULL, NULL);
      break;

    case 3:
      process(argv[0], argv[1], argv[2], NULL);
      break;

    case 4:
      process(argv[0], argv[1], argv[2], argv[3]);
      break;

    default:
      process(argv[0], "help", NULL, NULL);
  }

  return EXIT_SUCCESS;
}


const char *get_state_dir(void)
{
  static char state_dir[PATH_MAX];
  Snprintf(state_dir, sizeof(state_dir), "%s/%s", Getenv("HOME"), STATE_DIR);
  Mkdir(state_dir, 0700);
  return state_dir;
}

const char *get_state_file(void)
{
  static char state_file[PATH_MAX];
  Snprintf(state_file, sizeof(state_file), "%s/%s", get_state_dir(), STATE_FILE);
  return state_file;
}

void
process(const char *app, const char *cmd, const char *val1, const char *val2)
{
  if ((!strcmp(cmd, "help") || !strcmp(cmd, "--help")) && !val1 && !val2)
    return cmd_usage(app);

  if (!strcmp(cmd, "show") && !val1 && !val2)
    return cmd_show();

  if (!strcmp(cmd, "new") && !val1 && !val2)
    return cmd_new();

  if (!strcmp(cmd, "add") && val1 && !val2)
    return cmd_add(val1);

  if ((!strcmp(cmd, "del") || !strcmp(cmd, "delete")) && val1 && !val2)
    return cmd_del(val1);

  if ((!strcmp(cmd, "mov") || !strcmp(cmd, "move")) && val1 && val2)
    return cmd_mov(val1, val2);

  if (!strcmp(cmd, "swap") && val1 && val2)
    return cmd_swap(val1, val2);

  cmd_usage(app);
}

void
cmd_usage(const char *app)
{
  Fprintf(stdout, "Usage: %s <command> [arg1 [arg2]]\n", app);
  Fprintf(stdout, "<show> - show list if it exists\n");
  Fprintf(stdout, "<new> - create a new list\n");
  Fprintf(stdout, "<add> <value> - add a new item\n");
  Fprintf(stdout, "<del> <index> - delete an item by its index\n");
  Fprintf(stdout, "<mov> <index> <index> - move item from index to index\n");
  Fprintf(stdout, "<swap> <index> <index> - swap two items\n");
  Fprintf(stdout, "<help> - show this message\n");
}

void
cmd_show(void)
{
  FILE *stream;
  char buf[BUF_SIZE];

  if (!(stream = fopen(get_state_file(), "r")))
    return;

  for (size_t i = 1; Fgets(buf, sizeof(buf) - 1, stream); ++i)
    Fprintf(stdout, "%zu) %s", i, buf);

  Fclose(stream);
}

void
cmd_new(void)
{
  Fclose(
    Fopen(get_state_file(), "w"));
}

void
cmd_add(const char *value)
{
  FILE *stream = Fopen(get_state_file(), "a");
  Fputs(value, stream);
  Fputc('\n', stream);
  Fclose(stream);
  cmd_show();
}

void
cmd_del(const char *value)
{
  FILE *stream;
  list_t list = NULL;
  node_t *node;
  char buf[BUF_SIZE];
  char *endptr;
  long long n;


  // read file to linked list

  stream = Fopen(get_state_file(), "r");

  n = Strtoll(value, &endptr, 10);
  if (n <= 0 || n > INT_MAX)
    die("Strtoll()");

  for (size_t i = 1; Fgets(buf, sizeof(buf) - 1, stream); ++i) {

    if (i == (size_t) n) // deletion
      continue;          // is not putting item in new list

    if (!list_push(&list, buf))
      die("list_push()");
  }

  Fclose(stream);


  // write new list to file

  stream = Fopen(get_state_file(), "w");

  while (list) {
    Fputs(list->value, stream);
    node = list;
    list = list->next;
    node_free(node);
  }

  Fclose(stream);


  // display list

  cmd_show();
}

void
cmd_mov(const char *val1, const char *val2)
{
  FILE *stream;
  long long from, to, i;
  char *endptr;
  list_t list = NULL;
  node_t *node, *movable = NULL;
  char buf[BUF_SIZE];


  from = Strtoll(val1, &endptr, 10);
  if (from <= 0 || from > INT_MAX)
    die("Strtoll()");

  to = Strtoll(val2, &endptr, 10);
  if (to <= 0 || to > INT_MAX)
    die("Strtoll()");


  // read from the file

  stream = Fopen(get_state_file(), "r");

  for (i = 1; Fgets(buf, sizeof(buf) - 1, stream); ++i) {

    if (i == from) {
      if (!(movable = node_alloc(buf)))
        die("node_alloc()");
    }

    else if (!list_push(&list, buf))
      die("list_push()");
  }

  Fclose(stream);


  // write to the file

  if (movable) {

    stream = Fopen(get_state_file(), "w");

    for (i = 1; list; ++i) {

      if (to == i) {
        Fputs(movable->value, stream);
        node_free(movable);
      }

      else {
        Fputs(list->value, stream);
        node = list;
        list = list->next;
        node_free(node);
      }

    }

    if (to >= i) {
      Fputs(movable->value, stream);
      node_free(movable);
    }

    Fclose(stream);

  }


  // display list

  cmd_show();
}

void
cmd_swap(const char *val1, const char *val2)
{
  FILE *stream;
  char *endptr;
  list_t list = NULL;
  node_t *node = NULL;
  long long n1, n2;
  node_t *node1, *node2;
  char buf[BUF_SIZE];

  n1 = Strtoll(val1, &endptr, 10);
  if (n1 <= 0 || n1 > INT_MAX)
    return cmd_show();

  n2 = Strtoll(val2, &endptr, 10);
  if (n2 <= 0 || n2 > INT_MAX)
    return cmd_show();

  if (n1 == n2)
    return cmd_show();

  stream = Fopen(get_state_file(), "r");
  while (Fgets(buf, sizeof(buf) - 1, stream)) {
    if (!list_push(&list, buf)) {
      list_free(list);
      return cmd_show();
    }
  }
  Fclose(stream);

  if (!(node1 = list_index(list, n1))) {
    list_free(list);
    return cmd_show();
  }

  if (!(node2 = list_index(list, n2))) {
    list_free(list);
    return cmd_show();
  }

  swap(node1->value, node2->value);

  stream = Fopen(get_state_file(), "w");
  while (list) {
    Fputs(list->value, stream);
    node = list;
    list = list->next;
    node_free(node);
  }
  Fclose(stream);

  cmd_show();
}
