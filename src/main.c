
// file: main.c

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "util.h"
#include "list.h"

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
  const char *home_dir;

  if (!(home_dir = getenv("HOME")))
    die("getenv()");

  if (snprintf(state_dir, sizeof(state_dir), "%s/%s", home_dir, STATE_DIR) < 0)
    die("snprintf()");

  if (mkdir(state_dir, 0700) == -1 && errno != EEXIST)
    die("mkdir()");

  return state_dir;
}

const char *get_state_file(void)
{
  static char state_file[PATH_MAX];
  const char *state_dir;

  if (!(state_dir = get_state_dir()))
    die("get_state_dir()");

  if (snprintf(state_file, sizeof(state_file), "%s/%s", state_dir, STATE_FILE) < 0)
    die("snprintf()");

  return state_file;
}


void
usage(const char *app)
{
  fprintf(stdout, "usage:\n");
  fprintf(stdout, "%s - show list if it exists\n", app);
  fprintf(stdout, "%s new - create a new list\n", app);
  fprintf(stdout, "%s add <value> - add a new item\n", app);
  fprintf(stdout, "%s del <index> - delete an item by its index\n", app);
  fprintf(stdout, "%s help - show this message\n", app);
}

void
display(void)
{
  const char *state_file;
  FILE *stream;
  char buf[BUF_SIZE];

  if (!(state_file = get_state_file()))
    die("get_state_file()");

  if (!(stream = fopen(state_file, "r")))
    return;

  for (size_t i = 1; fgets(buf, sizeof(buf) - 1, stream); ++i)
    fprintf(stdout, "%zu) %s", i, buf);

  if (fclose(stream) == EOF)
    die("fclose()");
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
  FILE *stream;
  const char *state_file;

  if (!(state_file = get_state_file()))
    die("get_state_file()");

  if (!(stream = fopen(state_file, "w")))
    die("fopen()");

  if (fclose(stream) == EOF)
    die("fclose()");
}

void
cmd_add(const char *value)
{
  FILE *stream;
  const char *state_file;

  if (!(state_file = get_state_file()))
    die("get_state_file()");

  if (!(stream = fopen(state_file, "a")))
    die("fopen()");

  if (fputs(value, stream) == EOF)
    die("fputs()");

  if (fputc('\n', stream) == EOF)
    die("fputc()");

  if (fclose(stream) == EOF)
    die("fclose()");

  display();
}

void
cmd_del(const char *value)
{
  FILE *stream;
  list_t list;
  node_t *node;
  char buf[BUF_SIZE];
  char *endptr;
  long long n;
  size_t i;
  const char *state_file;


  // read file to linked list

  if (!(state_file = get_state_file()))
    die("get_state_file()");

  if (!(stream = fopen(state_file, "r+")))
    die("fopen()");

  n = strtoll(value, &endptr, 10);
  if (*endptr != '\0' || n <= 0 || n > INT_MAX)
    die("strtoll()");

  list = NULL;
  i = 1;

  while (fgets(buf, sizeof(buf) - 1, stream)) {

    if (i++ == (size_t) n)
      continue;

    if (!list_push(&list, buf))
      die("list_push()");
  }

  if (fclose(stream) == EOF)
    die("fclose()");


  // write new list to file

  if (!(stream = fopen(state_file, "w")))
    die("fopen()");

  while (list) {
    if (fputs(list->value, stream) == EOF)
      die("fputs()");
    node = list;
    list = list->next;
    free(node->value);
    free(node);
  }

  if (fclose(stream) == EOF)
    die("fclose()");


  // display list

  display();
}
