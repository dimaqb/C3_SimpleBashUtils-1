#include <stdio.h>

#include "cat_header.h"

int main(int argc, char *argv[]) {
  options opt = getOptions(argc, argv);
  const char *table[256];
  setTable(table);
  if (opt.showEndl) setEndl(table);
  if (opt.showTabs) setTab(table);
  if (opt.showNonPrintable) setNonPrinting(table);
  catFile(argc, argv, opt, table);
  return 0;
}
