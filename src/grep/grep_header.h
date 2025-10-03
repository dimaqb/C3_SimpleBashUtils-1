#ifndef GREP_HEADER
#define GREP_HEADER
#define _GNU_SOURCE
#include <regex.h>
#include <stdio.h>
typedef struct {
  char *pattern;
  size_t size;
  int regex;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
} options;

char *append_patterns(char *string, size_t *size, char const *expr,
                      size_t size_expr);
char *append_patterns_from_file(char *filename, size_t *size, char *string);
options getOptions(int argc, char *argv[]);
void grep(int argc, char *argv[], options opt);
void grep_file(FILE *file, const char *filename, regex_t *preg, options opt,
               int coun_of_files);
void grep_l(FILE *file, const char *filename, regex_t *preg, options opt);
void grep_count(FILE *file, const char *filename, regex_t *preg, options opt,
                int coun_of_files);
void grep_invert(FILE *file, const char *filename, regex_t *preg, options opt,
                 int coun_of_files);
void grep_printt_matched(FILE *file, const char *filename, regex_t *preg,
                         options opt, int coun_of_files);

#endif