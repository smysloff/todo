
// file: main.c

#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "util.h"
#include "wrappers.h"

#include "config.h"

#define VERSION "0.3.1"

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif


static const char *get_state_dir(void);
static const char *get_state_file(void);
static bool is(const char *s1, const char *s2);
static long long parse_index(const char *s);

void dispatch(const char *app, const char *cmd, const char *val1, const char *val2);
void cmd_show(const char *app);
void cmd_add(const char *app, const char *value);
void cmd_remove(const char *app, const char *value);
void cmd_move(const char *app, const char *val1, const char *val2);
void cmd_swap(const char *app, const char *val1, const char *val2);
void cmd_clear(const char *app);
void cmd_help(const char *app);
void cmd_version(const char *app);


int
main(int argc, char *argv[])
{
  switch (argc) {

    case 1:
      dispatch(argv[0], "show", NULL, NULL);
      break;

    case 2:
      if (is(argv[1], "-h") || is(argv[1], "--help")) {
        dispatch(argv[0], "help", NULL, NULL);
        break;
      }
      if (is(argv[1], "-v") || is(argv[1], "--version")) {
        dispatch(argv[0], "version", NULL, NULL);
        break;
      }
      dispatch(argv[0], argv[1], NULL, NULL);
      break;

    case 3:
      dispatch(argv[0], argv[1], argv[2], NULL);
      break;

    case 4:
      dispatch(argv[0], argv[1], argv[2], argv[3]);
      break;

    default:
      dispatch(argv[0], "help", NULL, NULL);
  }

  return EXIT_SUCCESS;
}


const char *get_state_dir(void)
{
  static char state_dir[PATH_MAX];
  snprintf_or_die(state_dir, sizeof(state_dir), "%s/%s", getenv_or_die("HOME"), STATE_DIR);
  mkdir_or_die(state_dir, 0700);
  return state_dir;
}

const char *get_state_file(void)
{
  static char state_file[PATH_MAX];
  snprintf_or_die(state_file, sizeof(state_file), "%s/%s", get_state_dir(), STATE_FILE);
  return state_file;
}

static bool
is(const char *s1, const char *s2)
{
  return !strcmp(s1, s2);
}

static long long
parse_index(const char *s)
{
  long long res;
  char *endptr;

  errno = 0;
  res = strtoll(s, &endptr, 10);

  if ((endptr && endptr == s) || errno == ERANGE)
    return 0;

  if (res <= 0 || res > INT_MAX)
    return 0;

  return res;
}


void
dispatch(const char *app, const char *cmd, const char *val1, const char *val2)
{
  if (is(cmd, "show") && !val1 && !val2)
    return cmd_show(app);

  if (is(cmd, "add") && val1 && !val2)
    return cmd_add(app, val1);

  if ((is(cmd, "rm") || is(cmd, "remove")) && val1 && !val2)
    return cmd_remove(app, val1);

  if ((is(cmd, "mv") || is(cmd, "move")) && val1 && val2)
    return cmd_move(app, val1, val2);

  if ((is(cmd, "swap")) && val1 && val2)
    return cmd_swap(app, val1, val2);

  if ((is(cmd, "clear")) && !val1 && !val2)
    return cmd_clear(app);

  if ((is(cmd, "h") || is(cmd, "help")) && !val1 && !val2)
    return cmd_help(app);

  if ((is(cmd, "v") || is(cmd, "version")) && !val1 && !val2)
    return cmd_version(app);

  cmd_help(app);
}

void
cmd_show(const char *app)
{
  FILE *stream;
  char buf[BUF_SIZE];

  if (!(stream = fopen(get_state_file(), "r")))
    return;

  for (size_t i = 1; fgets_or_die(buf, sizeof(buf) - 1, stream); ++i)
    fprintf_or_die(stdout, "%zu) %s", i, buf);

  fclose_or_die(stream);
  (void) app;
}

void
cmd_add(const char *app, const char *value)
{
  FILE *stream = fopen_or_die(get_state_file(), "a");
  fputs_or_die(value, stream);
  fputc_or_die('\n', stream);
  fclose_or_die(stream);
  cmd_show(app);
}

void
cmd_remove(const char *app, const char *value)
{
  FILE *stream;
  list_t list = NULL;
  node_t *node;
  char buf[BUF_SIZE];
  long long n;


  // read file to linked list

  stream = fopen_or_die(get_state_file(), "r");

  if (!(n = parse_index(value)))
    return cmd_show(app);

  for (size_t i = 1; fgets_or_die(buf, sizeof(buf) - 1, stream); ++i) {
    if (i == (size_t) n) // deletion
      continue;          // is not putting item in new list
    list_push(&list, buf);
  }

  fclose_or_die(stream);


  // write new list to file

  stream = fopen_or_die(get_state_file(), "w");

  while (list) {
    fputs_or_die(list->value, stream);
    node = list;
    list = list->next;
    node_free(node);
  }

  fclose_or_die(stream);


  // display list

  cmd_show(app);
}

void
cmd_move(const char *app, const char *val1, const char *val2)
{
  FILE *stream;
  long long from, to, i;
  list_t list = NULL;
  node_t *node, *movable = NULL;
  char buf[BUF_SIZE];


  if (!(from = parse_index(val1)))
    return cmd_show(app);

  if (!(to = parse_index(val2)))
    return cmd_show(app);


  // read from the file

  stream = fopen_or_die(get_state_file(), "r");

  for (i = 1; fgets_or_die(buf, sizeof(buf) - 1, stream); ++i) {
    if (i == from)
      movable = node_alloc(buf);
    else
      list_push(&list, buf);
  }

  fclose_or_die(stream);


  // write to the file

  if (movable) {

    stream = fopen_or_die(get_state_file(), "w");

    for (i = 1; list; ++i) {

      if (to == i) {
        fputs_or_die(movable->value, stream);
        node_free(movable);
      }

      else {
        fputs_or_die(list->value, stream);
        node = list;
        list = list->next;
        node_free(node);
      }

    }

    if (to >= i) {
      fputs_or_die(movable->value, stream);
      node_free(movable);
    }

    fclose_or_die(stream);
  }


  // display list

  cmd_show(app);
}

void
cmd_swap(const char *app, const char *val1, const char *val2)
{
  FILE *stream;
  list_t list = NULL;
  node_t *node = NULL;
  long long n1, n2;
  node_t *node1, *node2;
  char buf[BUF_SIZE];

  if (!(n1 = parse_index(val1)))
    return cmd_show(app);

  if (!(n2 = parse_index(val2)))
    return cmd_show(app);

  if (n1 == n2)
    return cmd_help(app);

  stream = fopen_or_die(get_state_file(), "r");
  while (fgets_or_die(buf, sizeof(buf) - 1, stream))
    list_push(&list, buf);
  fclose_or_die(stream);

  node1 = list_index(list, n1);
  node2 = list_index(list, n2);

  swap(node1->value, node2->value);

  stream = fopen_or_die(get_state_file(), "w");
  while (list) {
    fputs_or_die(list->value, stream);
    node = list;
    list = list->next;
    node_free(node);
  }
  fclose_or_die(stream);

  cmd_show(app);
}

void
cmd_clear(const char *app)
{
  fclose_or_die(
    fopen_or_die(get_state_file(), "w"));
  (void) app;
}

void
cmd_help(const char *app)
{
  fprintf_or_die(stdout, "Usage: %s <command> [arg1 [arg2]]\n", app);
  fprintf_or_die(stdout, "%s - todo without command displays the current list\n", app);
  fprintf_or_die(stdout, "<add> <value> - adds an item to the list\n");
  fprintf_or_die(stdout, "<rm> <index> - removes the item at the given index\n");
  fprintf_or_die(stdout, "<mv> <index> <index> - moves the item at index1 to position index2\n");
  fprintf_or_die(stdout, "<swap> <index> <index> - swaps the items at the two given indexes\n");
  fprintf_or_die(stdout, "<clear> - deletes all items from the current list\n");
  fprintf_or_die(stdout, "<h> - shows this help message\n");
  fprintf_or_die(stdout, "<v> - shows the current version\n");
}

void
cmd_version(const char *app)
{
  fprintf_or_die(stdout, "%s\n", VERSION);
  (void) app;
}
