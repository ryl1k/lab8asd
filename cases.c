#include "header\cases.h"

int average_case(Book books[], int max_books) {
    return load_books("./Testcases/" BOOKS_TEST_FILE, "averageCase", books, max_books);
}

int best_case(Book books[], int max_books) {
    return load_books("./Testcases/" BOOKS_TEST_FILE, "bestCase", books, max_books);
}

int worst_case(Book books[], int max_books) {
    return load_books("./Testcases/" BOOKS_TEST_FILE, "worstCase", books, max_books);
}

void counter_set_zero(Counters* counter) {
    counter->comps = 0;
    counter->swaps = 0;
}