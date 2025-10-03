#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "cat_header.h"

options getOptions(int argc, char *argv[]) {
  options opt = {0, 0, 0, 0, 0, 0};
  int currentOption;
  const struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                        {"number", 0, NULL, 'n'},
                                        {"squeeze-blank", 0, NULL, 's'},
                                        {NULL, 0, NULL, 0}};

  while ((currentOption =
              getopt_long(argc, argv, "benstvTE", long_options, NULL)) != -1) {
    switch (currentOption) {
      case 'b':
        opt.numberNonBlank = 1;
        break;
      case 'e':
        opt.showEndl = 1;
        opt.showNonPrintable = 1;
        break;
      case 'v':
        opt.showNonPrintable = 1;
        break;
      case 'E':
        opt.showEndl = 1;
        break;
      case 'n':
        opt.numberAll = 1;
        break;
      case 's':
        opt.sqeeze_blank = 1;
        break;
      case 't':
        opt.showTabs = 1;
        opt.showNonPrintable = 1;
        break;
      case 'T':
        opt.showTabs = 1;
        break;
    }
  }
  return opt;
}

void setEndl(const char *table[256]) { table['\n'] = "$\n"; }

void setTab(const char *table[256]) { table['\t'] = "^I"; }

void setNonPrinting(const char *table[256]) {
  const char *first[] = {"^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G", "^H"};
  const char *second[] = {"^K", "^L", "^M", "^N",  "^O", "^P", "^Q",
                          "^R", "^S", "^T", "^U",  "^V", "^W", "^X",
                          "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  const char *third[] = {
      "^?",   "M-^@", "M-^A",  "M-^B", "M-^C", "M-^D", "M-^E", "M-^F", "M-^G",
      "M-^H", "M-^I", "M-^J",  "M-^K", "M-^L", "M-^M", "M-^N", "M-^O", "M-^P",
      "M-^Q", "M-^R", "M-^S",  "M-^T", "M-^U", "M-^V", "M-^W", "M-^X", "M-^Y",
      "M-^Z", "M-^[", "M-^\\", "M-^]", "M-^^", "M-^_", "M- ",  "M-!",  "M-\"",
      "M-#",  "M-$",  "M-%%",  "M-&",  "M-'",  "M-(",  "M-)",  "M-*",  "M-+",
      "M-,",  "M--",  "M-.",   "M-/",  "M-0",  "M-1",  "M-2",  "M-3",  "M-4",
      "M-5",  "M-6",  "M-7",   "M-8",  "M-9",  "M-:",  "M-;",  "M-<",  "M-=",
      "M->",  "M-?",  "M-@",   "M-A",  "M-B",  "M-C",  "M-D",  "M-E",  "M-F",
      "M-G",  "M-H",  "M-I",   "M-J",  "M-K",  "M-L",  "M-M",  "M-N",  "M-O",
      "M-P",  "M-Q",  "M-R",   "M-S",  "M-T",  "M-U",  "M-V",  "M-W",  "M-X",
      "M-Y",  "M-Z",  "M-[",   "M-\\", "M-]",  "M-^",  "M-_",  "M-`",  "M-a",
      "M-b",  "M-c",  "M-d",   "M-e",  "M-f",  "M-g",  "M-h",  "M-i",  "M-j",
      "M-k",  "M-l",  "M-m",   "M-n",  "M-o",  "M-p",  "M-q",  "M-r",  "M-s",
      "M-t",  "M-u",  "M-v",   "M-w",  "M-x",  "M-y",  "M-z",  "M-{",  "M-|",
      "M-}",  "M-~",  "M-^?"};

  memcpy(table, first, sizeof first);
  memcpy(&table[10 + 1], second, sizeof second);
  memcpy(&table['~' + 1], third, sizeof third);
}

void setTable(const char *table[256]) {
  const char *charTable[256] = {
      "\00",  "\01",  "\02",  "\03",  "\04",  "\05",  "\06",  "\a",   "\b",
      "\t",   "\n",   "\v",   "\f",   "\r",   "\016", "\017", "\020", "\021",
      "\022", "\023", "\024", "\025", "\026", "\027", "\030", "\031", "\032",
      "\033", "\034", "\035", "\036", "\037", " ",    "!",    "\"",   "#",
      "$",    "%%",   "&",    "\'",   "(",    ")",    "*",    "+",    ",",
      "-",    ".",    "/",    "0",    "1",    "2",    "3",    "4",    "5",
      "6",    "7",    "8",    "9",    ":",    ";",    "<",    "=",    ">",
      "?",    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",
      "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",    "P",
      "Q",    "R",    "S",    "T",    "U",    "V",    "W",    "X",    "Y",
      "Z",    "[",    "\\",   "]",    "^",    "_",    "`",    "a",    "b",
      "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",
      "l",    "m",    "n",    "o",    "p",    "q",    "r",    "s",    "t",
      "u",    "v",    "w",    "x",    "y",    "z",    "{",    "|",    "}",
      "~",    "\x7f", "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86",
      "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f",
      "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98",
      "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f", "\xa0", "\xa1",
      "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
      "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1", "\xb2", "\xb3",
      "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc",
      "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5",
      "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce",
      "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
      "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf", "\xe0",
      "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
      "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0", "\xf1", "\xf2",
      "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8", "\xf9", "\xfa", "\xfb",
      "\xfc", "\xfd", "\xfe", "\xff"};
  memcpy(table, charTable, sizeof charTable);
}

void cat(FILE *file, options opt, const char *table[256]) {
  int c = 0;
  int lastChar = '\n';
  int number_of_line = 1;
  int sqeeze = 0;
  while ((fread(&c, 1, 1, file)) > 0) {
    if (lastChar == '\n') {
      if (opt.sqeeze_blank && c == '\n') {
        if (sqeeze) continue;
        sqeeze = 1;
      } else
        sqeeze = 0;
      if (opt.numberNonBlank) {
        if (c != '\n') printf("%6d\t", number_of_line++);
      } else if (opt.numberAll)
        printf("%6d\t", number_of_line++);
    }
    printf("%s", table[c]);
    lastChar = c;
  }
}

void catFile(int argc, char *argv[], options opt, const char *table[256]) {
  for (char **currentArg = &argv[1], **endArg = &argv[argc];
       currentArg != endArg; currentArg++) {
    if (**currentArg == '-') continue;
    FILE *file = fopen(*currentArg, "rb");

    if (file == NULL) {
      fprintf(stderr, "%s: ", argv[0]);
      perror(*currentArg);
      continue;
    }
    cat(file, opt, table);
    fclose(file);
  }
}