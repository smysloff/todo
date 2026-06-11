
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

void usage(const char *app);
void display(void);
void process(const char *app, const char *cmd, const char *value);
void cmd_new(void);
void cmd_add(const char *value);
void cmd_del(const char *value);


int
main(int argc, char *argv[])
{
  switch (argc) {

    case 1: display();
      break;

    case 2: process(argv[0], argv[1], NULL);
      break;

    case 3: process(argv[0], argv[1], argv[2]);
      break;

    default: usage(argv[0]);
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
usage(const char *app)
{
  Fprintf(stdout, "usage:\n");
  Fprintf(stdout, "%s - show list if it exists\n", app);
  Fprintf(stdout, "%s new - create a new list\n", app);
  Fprintf(stdout, "%s add <value> - add a new item\n", app);
  Fprintf(stdout, "%s del <index> - delete an item by its index\n", app);
  Fprintf(stdout, "%s help - show this message\n", app);
}

void
display(void)
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
process(const char *app, const char *cmd, const char *value)
{
  if (!strcmp(cmd, "help") && !value)
    return usage(app);

  if (!strcmp(cmd, "new") && !value)
    return cmd_new();

  if (!strcmp(cmd, "add") && value)
    return cmd_add(value);

  if (!strcmp(cmd, "del") && value)
    return cmd_del(value);

  usage(app);
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
  display();
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

  stream = Fopen(get_state_file(), "r+");

  n = strtoll(value, &endptr, 10); // @todo Strtoll
  if (*endptr != '\0' || n <= 0 || n > INT_MAX)
    die("strtoll()");

  for (size_t i = 1; Fgets(buf, sizeof(buf) - 1, stream); ++i) {

    if (i == (size_t) n)
      continue;

    if (!list_push(&list, buf))
      die("list_push()");
  }

  Fclose(stream);


  // write new list to file

  stream = Fopen(get_state_file(), "w");

  // @todo list_print_and_free()
  while (list) {
    Fputs(list->value, stream);
    node = list;
    list = list->next;
    free(node->value);
    free(node);
  }

  Fclose(stream);


  // display list

  display();
}
