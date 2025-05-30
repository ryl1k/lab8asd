﻿#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>

#define AUTHOR_LEN 100
#define TITLE_LEN 100

#define BOOKS_TEST_FILE    "books.json"
#define SHELL_TEST_FILE    "testcases.bin"

#define LOWER_LIMIT -130
#define UPPER_LIMIT 130

#define TESTCASES 7


/*
Тестові значення:
    10
    100
    500
    2500
    10000
    50000
    100000
*/
static const size_t TEST_VALUES[TESTCASES] = { 
    10,
    100,
    500,
    2500,
    10000,
    50000,
    100000
};

typedef enum {
    SEQ_POW2 = 1,        // Формула 1
    SEQ_2C_POW_3_D = 3,  // Формула 3 
    SEQ_SEDGEWICK2 = 8   // Формула 8 
} SequenceType;

typedef struct {
    char author[AUTHOR_LEN];
    char title[TITLE_LEN];
} Book;

typedef struct {
    long long comps;
    long long swaps;
} Counters;



#endif // CONSTANTS_H