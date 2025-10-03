#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grep_header.h"

char *append_patterns(char *string, size_t *size, char const *expr,
                      size_t size_expr) {
  size_t len = *size;
  string = realloc(string, len + size_expr + 4);
  string[len] = '(';
  memcpy(string + len + 1, expr, size_expr);
  len += size_expr + 1;
  string[len] = ')';
  string[len + 1] = '|';
  string[len + 2] = '\0';
  len += 2;
  *size = len;
  return string;
}

char *append_patterns_from_file(char *filename, size_t *size, char *string) {
  char *line = NULL;
  size_t length = 0;
  size_t len = *size;
  FILE *file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "grep: ");
    perror(filename);
    exit(errno);
  }
  while (getline(&line, &length, file) > 0) {
    int strlength = strlen(line);
    if (line[strlength - 1] == 10) {
      line[strlength - 1] = '\0';
      strlength--;
    }
    string = realloc(string, len + strlength + 4);
    string[len] = '(';
    memcpy(string + len + 1, line, strlength);
    len += strlength + 1;
    string[len] = ')';
    string[len + 1] = '|';
    string[len + 2] = '\0';
    len += 2;
    *size = len;
  }
  free(line);
  return string;
}

options getOptions(int argc, char *argv[]) {
  options opt = {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int o;
  opt.pattern = malloc(1);
  opt.pattern[0] = '\0';
  size_t pattern_size = 0;
  while ((o = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (o) {
      case 'e':
        opt.pattern =
            append_patterns(opt.pattern, &pattern_size, optarg, strlen(optarg));
        opt.regex |= REG_EXTENDED;
        break;
      case 'i':
        opt.regex |= REG_ICASE;
        break;
      case 'v':
        opt.v = 1;
        break;
      case 'c':
        opt.c = 1;
        break;
      case 'l':
        opt.l = 1;
        break;
      case 'n':
        opt.n = 1;
        break;
      case 'h':
        opt.h = 1;
        break;
      case 's':
        opt.s = 1;
        break;
      case 'f':
        opt.pattern =
            append_patterns_from_file(optarg, &pattern_size, opt.pattern);
        opt.regex |= REG_EXTENDED;
        break;
      case 'o':
        opt.o = 1;
        break;
    }
  }
  opt.size = pattern_size;
  if (opt.size > 0) opt.pattern[opt.size - 1] = '\0';
  return opt;
}

void grep(int argc, char *argv[], options opt) {
  (void)argc;
  regex_t storage;
  regex_t *preg = &storage;
  char error[100];
  int err;
  if (opt.size == 0) {
    if ((err = regcomp(preg, argv[0], opt.regex)) > 0) {
      regerror(err, preg, error, 100);
      fprintf(stderr, "./s21_grep: %s\n", error);
      exit(err);
    }
  } else {
    if ((err = regcomp(preg, opt.pattern, opt.regex)) > 0) {
      regerror(err, preg, error, 100);
      fprintf(stderr, "./s21_grep: %s\n", error);
      exit(err);
    }
  }
  free(opt.pattern);
  /*if(argc == (opt.size ? 2 : 1)){
      grep_file(stdin, "", preg, opt, 0);
  }*/
  int count_of_files = 0;
  if (opt.h)
    count_of_files = 0;
  else
    for (char **filename = argv + (opt.size ? 0 : 1); filename != &argv[argc];
         filename++)
      count_of_files++;
  for (char **filename = argv + (opt.size ? 0 : 1); filename != &argv[argc];
       filename++) {
    FILE *file = fopen(*filename, "rb");
    if (file == NULL && !opt.s) {
      fprintf(stderr, "./s21_grep: ");
      perror(*filename);
      continue;
    } else if (file == NULL && opt.s)
      continue;
    if (opt.l)
      grep_l(file, *filename, preg, opt);
    else if (opt.c)
      grep_count(file, *filename, preg, opt, count_of_files);
    else if (opt.v)
      grep_invert(file, *filename, preg, opt, count_of_files);
    else if (opt.o)
      grep_printt_matched(file, *filename, preg, opt, count_of_files);
    else
      grep_file(file, *filename, preg, opt, count_of_files);
    fclose(file);
  }
  regfree(preg);
}

void grep_file(FILE *file, const char *filename, regex_t *preg, options opt,
               int count_of_files) {
  char *line = NULL;
  size_t length = 0;
  regmatch_t match;
  int number_of_line = 0, number_of_last_line = 0;
  while (getline(&line, &length, file) > 0) {
    int reg = regexec(preg, line, 1, &match, 0);
    number_of_line++;
    if (count_of_files > 1) {
      if (!reg) {
        number_of_last_line = number_of_line;
        if (opt.n)
          printf("%s:%d:%s", filename, number_of_line, line);
        else
          printf("%s:%s", filename, line);
      }
    } else {
      if (!reg) {
        number_of_last_line = number_of_line;
        if (opt.n)
          printf("%d:%s", number_of_line, line);
        else
          printf("%s", line);
      }
    }
  }
  if (number_of_last_line == number_of_line) printf("\n");
  free(line);
}

void grep_invert(FILE *file, const char *filename, regex_t *preg, options opt,
                 int count_of_files) {
  char *line = NULL;
  size_t length = 0;
  regmatch_t match;
  int number_of_line = 0, number_of_last_line = 0;
  while (getline(&line, &length, file) > 0) {
    number_of_line++;
    int reg = regexec(preg, line, 1, &match, 0);
    if (count_of_files > 1) {
      if (reg) {
        number_of_last_line = number_of_line;
        if (opt.n)
          printf("%s:%d:%s", filename, number_of_line, line);
        else
          printf("%s:%s", filename, line);
      }
    } else {
      if (reg) {
        number_of_last_line = number_of_line;
        if (opt.n)
          printf("%d:%s", number_of_line, line);
        else
          printf("%s", line);
      }
    }
  }
  if (number_of_last_line == number_of_line) printf("\n");
  free(line);
}

void grep_l(FILE *file, const char *filename, regex_t *preg, options opt) {
  char *line = NULL;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    int reg = regexec(preg, line, 1, &match, 0);
    if (opt.v) {
      if (reg) {
        printf("%s\n", filename);
        break;
      }
    } else {
      if (!reg) {
        printf("%s\n", filename);
        break;
      }
    }
  }
  free(line);
}

void grep_count(FILE *file, const char *filename, regex_t *preg, options opt,
                int count_of_files) {
  char *line = NULL;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    int reg = regexec(preg, line, 1, &match, 0);
    if (opt.v) {
      if (reg == REG_NOMATCH) count++;
    } else {
      if (!reg) count++;
    }
  }
  if (count_of_files > 1) {
    printf("%s:%d\n", filename, count);
  } else {
    printf("%d\n", count);
  }
  free(line);
}

void grep_printt_matched(FILE *file, const char *filename, regex_t *preg,
                         options opt, int count_of_files) {
  char *line = NULL;
  size_t length = 0;
  regmatch_t match;
  int number_of_line = 0;
  while (getline(&line, &length, file) > 0) {
    int reg = regexec(preg, line, 1, &match, 0);
    char *matched = line + match.rm_so;
    if (count_of_files > 1) {
      if (!reg) {
        while (!regexec(preg, matched, 1, &match, 0)) {
          int d = match.rm_eo - match.rm_so;
          if (opt.n)
            printf("%s:%d:%.*s\n", filename, number_of_line, d,
                   matched + match.rm_so);
          else
            printf("%s:%.*s\n", filename, d, matched + match.rm_so);
          matched = matched + match.rm_eo;
        }
      }
    } else {
      if (!reg) {
        while (!regexec(preg, matched, 1, &match, 0)) {
          int d = match.rm_eo - match.rm_so;
          if (opt.n)
            printf("%d:%.*s\n", number_of_line, d, matched + match.rm_so);
          else
            printf("%.*s\n", d, matched + match.rm_so);
          matched = matched + match.rm_eo;
        }
      }
    }
  }
  free(line);
}