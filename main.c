
// file: main.c

#include <errno.h>  // errno
#include <stddef.h> // size_t, NULL
#include <stdio.h>  // stdout, stderr, EOF, fprintf(), fopen(), fclose(), fgets(), fputs(), fputc(), perror(), clearerr()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE, atoll()
#include <string.h> // strerror()
#include <unistd.h> // ftruncate()
#include <stdarg.h> // ..., va_list, va_start(), va_end()
#include <stdbool.h> // true, false


// globals

#define BUF_SIZE 4096

char filename[BUF_SIZE] = "todo.list";


// linked list

typedef struct node_s {
  struct node_s *next;
  char *value;
} node_t;

typedef node_t * list_t;

node_t *list_tail(list_t list);
node_t *list_push(list_t *list, const char *value);


// functions

void die(const char *msg, ...);
//char *read_stream(FILE *stream);

void usage(const char *app);
void display(void);
void process(const char *app, const char *cmd, const char *value);
void cmd_new(void);
void cmd_add(const char *value);
void cmd_del(const char *value);


// main

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


// functions

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


void
die(const char *msg, ...)
{
  va_list ap;

  fprintf(stderr, "error:");
  if (msg) {
    va_start(ap, msg);
    fprintf(stderr, " %s", msg);
    if (errno)
      fprintf(stderr, ",");
    va_end(ap);
  }
  if (errno) fprintf(stderr, " %s", strerror(errno));
  fprintf(stderr, "\n");
}

//char *
//read_stream(FILE *stream)
//{
//  char *res, *tmp;
//  char buf[BUF_SIZE];
//  size_t total_bytes;
//  size_t read_bytes;
//
//  if (!stream)
//    return NULL;
//
//  res = NULL;
//  total_bytes = 0;
//
//  while ((read_bytes = fread(buf, 1, sizeof(buf) - 1, stream))) {
//    if (!(tmp = realloc(res, total_bytes + read_bytes + 1))) {
//      free(res);
//      return NULL;
//    }
//    res = tmp;
//    memcpy(&res[total_bytes], buf, read_bytes);
//    total_bytes += read_bytes;
//    res[total_bytes] = '\0';
//  }
//
//  if (ferror(stream) && errno != EINTR) {
//    free(res);
//    return NULL;
//  }
//
//  return res;
//}

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
  char buf[BUF_SIZE];
  FILE *stream;

  if (!(stream = fopen(filename, "r"))) {

    if (!(stream = fopen(filename, "a")))
      die("fopen()");

    if (fclose(stream) == EOF)
      die("fclose()");

    return;
  }

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

  if (!(stream = fopen(filename, "w")))
    die("fopen()");

  if (fclose(stream) == EOF)
    die("flose()");
}

void
cmd_add(const char *value)
{
  FILE *stream;

  if (!(stream = fopen(filename, "a")))
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
  size_t n, i;


  // read file to linked list

  if (!(stream = fopen(filename, "r+")))
    die("fopen()");

  if (!(n = (size_t) atoll(value)))
    die("atoll()");

  list = NULL;
  i = 1;

  while (fgets(buf, sizeof(buf) - 1, stream)) {

    if (i++ == n)
      continue;

    if (!list_push(&list, buf))
      die("list_push()");
  }


  // write new list to file

  fflush(stream);
  ftruncate(fileno(stream), 0);
  clearerr(stream);
  fseek(stream, 0, SEEK_SET);

  while (list) {
    fputs(list->value, stream);
    node = list;
    list = list->next;
    free(node->value);
    free(node);
  }


  // cleanup and exit

  if (fclose(stream) == EOF)
    die("fclose()");

  display();
}
