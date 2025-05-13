#ifndef CONSTANTS_H
#define CONSTANTS_H


#ifndef AUTHOR_LEN
#define AUTHOR_LEN 100
#endif
#ifndef TITLE_LEN
#define TITLE_LEN 100
#endif

#define BOOKS_TEST_FILE    "books.json"
#define SHELL_TEST_FILE    "testcases.txt"

#define LOWER_LIMIT -130.0
#define UPPER_LIMIT 130.0


typedef struct {
    char author[AUTHOR_LEN];
    char title[TITLE_LEN];
} Book;

typedef struct {
    long long comps;
    long long swaps;
} Counters;


#endif // CONSTANTS_H