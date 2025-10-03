#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "grep_header.h"

int main(int argc, char *argv[]) {
  options opt = getOptions(argc, argv);
  argv += optind;
  argc -= optind;
  if (argc == 0) {
    fprintf(stderr, "Usage: grep [OPTION]... PATTERNS [FILE]...\n");
    exit(1);
  }
  grep(argc, argv, opt);
  return 0;
}