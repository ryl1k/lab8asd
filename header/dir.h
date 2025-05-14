#ifndef DIR_H
#define DIR_H

#include "constants.h"
#include <stdlib.h>

void __init__();

int book_write_to(const char* path, void* context, size_t size, size_t elementcount);
int book_add_to(const char* path, void* context, size_t size, size_t elementcount);

int cnt_write_to(const char* path, void* context, size_t size, size_t elementcount);
int cnt_add_to(const char* path, void* context, size_t size, size_t elementcount);

#endif // DIR_H
