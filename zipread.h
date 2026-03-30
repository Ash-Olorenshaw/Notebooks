#ifndef ZIPREAD_H
#define ZIPREAD_H

#include "./zip/entries.h"

NotebookEntry *read_archive();
int cleanup_archive(NotebookEntry *root_node);

#endif
