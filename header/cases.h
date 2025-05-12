// cases.h
#ifndef CASES_H
#define CASES_H

#include "json.h"


// Завантажує середній випадок (averageCase) з books.json
 
int average_case(Book books[], int max_books);

 
// Завантажує найкращий випадок (bestCase) з books.json

int best_case(Book books[], int max_books);

// Завантажує найгірший випадок (worstCase) з books.json

int worst_case(Book books[], int max_books);

// Заполнює лічильники нулями

void counter_set_zero(Counters* counter);

#endif // CASES_H
