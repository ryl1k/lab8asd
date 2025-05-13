#ifndef DIR_H
#define DIR_H

#include "constants.h"
#include <stdlib.h>

void __init__();

int write_to(const char* path, void* context, size_t size, size_t elementcount);
int add_to(const char* path, void* context, size_t size, size_t elementcount);


#endif // DIR_H
