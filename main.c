
// file: main.c

#include <errno.h>  // errno
#include <stddef.h> // size_t, NULL
#include <stdio.h>  // stdout, stderr, EOF, fprintf(), fopen(), fclose(), fgets(), fputs(), fputc(), perror(), clearerr()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE, atoll()
#include <string.h> // strerror()
#include <unistd.h> // ftruncate()


// globals

#define BUF_SIZE 248 // 256 - sizeof(void *) for x64

char filename[BUF_SIZE] = "todo.list";


// types

typedef struct node_s {
  struct node_s *next;
  char value[BUF_SIZE];
} node_t;



// functions

void die(const char *msg);
node_t *list_tail(node_t *head);
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
    case 1: display(); break;
    case 2: process(argv[0], argv[1], NULL); break;
    case 3: process(argv[0], argv[1], argv[2]); break;
    default: usage(argv[0]); break;
  }

  return EXIT_SUCCESS;
}


// functions

node_t *
list_tail(node_t *head)
{
  if (!head) return NULL;
  if (!head->next) return head;
  return head->next;
}

void
die(const char *msg)
{
  fprintf(stderr, "error:");
  if (msg) fprintf(stderr, " %s", msg);
  if (msg && errno) fprintf(stderr, ",");
  if (errno) fprintf(stderr, " %s", strerror(errno));
  fprintf(stderr, "\n");
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
  char buf[BUF_SIZE];
  FILE *stream;

  if (!(stream = fopen(filename, "r"))) {
    if (!(stream = fopen(filename, "a"))) die("fopen()");
    if (fclose(stream) == EOF) die("fclose()");
    return;
  }

  for (size_t i = 1; fgets(buf, sizeof(buf) - 1, stream); ++i)
    fprintf(stdout, "%zu) %s", i, buf);

  if (fclose(stream) == EOF) die("fclose()");
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
  if (!(stream = fopen(filename, "w"))) die("fopen()");
  if (fclose(stream) == EOF) die("flose()");
}

void
cmd_add(const char *value)
{
  FILE *stream;
  if (!(stream = fopen(filename, "a"))) die("fopen()");
  if (fputs(value, stream) == EOF) die("fputs()");
  if (fputc('\n', stream) == EOF) die("fputc()");
  if (fclose(stream) == EOF) die("fclose()");
  display();
}

void
cmd_del(const char *value)
{
  FILE *stream;
  node_t *head, *node;
  char buf[BUF_SIZE];
  size_t n;

  // read file to linked list

  if (!(stream = fopen(filename, "r+")))
    die("fopen()");

  head = NULL;
  n = (size_t) atoll(value);
  if (n == 0) die("atoll()");

  for (size_t i = 1; fgets(buf, sizeof(buf) - 1, stream); ++i) {

    if (i == n) continue;
    if (!(node = malloc(sizeof(node_t)))) die("malloc()");

    memcpy(node->value, buf, sizeof(buf));
    node->next = NULL;

    head == NULL
      ? (head = node)
      : (list_tail(head)->next = node);
  }


  // write new list to file

  fflush(stream);
  ftruncate(fileno(stream), 0);
  clearerr(stream);
  fseek(stream, 0, SEEK_SET);

  while (head) {
    fputs(head->value, stream);
    node = head;
    head = head->next;
    free(node);
  }


  // cleanup and exit

  if (fclose(stream) == EOF) die("fclose()");
  display();
}
