#ifndef JSON_H
#define JSON_H

#include "cJSON.h"
#include "constants.h"


char* _json_read_file(const char* filename);


// Завантажує масив книг із JSON-файлу.
int load_books(const char* filename, const char* array_name, Book books[], int max_books);
    

#endif // JSON_H
