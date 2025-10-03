#ifndef CAT_HEADER
#define CAT_HEADER
#include <stdio.h>
typedef struct {
  int numberNonBlank;
  int showEndl;
  int numberAll;
  int sqeeze_blank;
  int showTabs;
  int showNonPrintable;
} options;

options getOptions(int argc, char *argv[]);
void setTable(const char *table[256]);
void cat(FILE *file, options opt, const char *table[256]);
void setEndl(const char *table[256]);
void setTab(const char *table[256]);
void setNonPrinting(const char *table[256]);
void catFile(int argc, char *argv[], options opt, const char *table[256]);
#endif
