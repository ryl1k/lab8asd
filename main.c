#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

#include "header\dir.h"   // Structure
#include "header\cases.h" // JSON + constants

#pragma execution_character_set("UTF-8")

void selection_sort_books(Book arr[], int n, Counters* cnt) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            cnt->comps++;
            if (strcmp(arr[j].author, arr[min].author) < 0) {
                min = j;
            }
        }
        if (min != i) {
            Book tmp = arr[i];
            arr[i] = arr[min];
            arr[min] = tmp;
            cnt->swaps += 3; 
        }
    }
}


/*
@param кількість елементів масиву
@param кількість відстаней
@param відстані
*/
int* calculate_distances(int count, ...) {
    va_list args;
    int* res = malloc(count * sizeof * res);
    if (!res) {
        perror("malloc error [calculate_distances]");
        exit(EXIT_FAILURE);
    }
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        res[i] = va_arg(args, int);
    }
    va_end(args);
    return res;
}


void shell_sort(double arr[], int n, Counters* cnt) {
    const int gaps_count = 3;
    int* gaps = calculate_distances(gaps_count, 8, 3, 1);

    for (int gi = 0; gi < gaps_count; gi++) {
        int gap = gaps[gi];
        for (int i = gap; i < n; i++) {
            double temp = arr[i];
            int j = i;
            cnt->comps++;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                cnt->swaps++;
                j -= gap;
                cnt->comps++;
            }
            arr[j] = temp;
        }
    }
    free(gaps);
}

void randomize(double arr[],size_t len) {
    for (int i = 0; i < len; i++) {
        arr[i] = LOWER_LIMIT + (rand() / (double)RAND_MAX) * (UPPER_LIMIT - LOWER_LIMIT);
    }
}




void print_books(Book* books, size_t size) {
    for (int i = 0; i < 10; i++) {
        printf("%5d | %-10s | %s\n", i + 1, books[i].author, books[i].title);
    }
}

int main(void) {
	SetConsoleOutputCP(65001);
    srand(time(NULL));

    // Створення структури файлів та папок
    __init__();

    Book books_worst_case[10];
    Book books_average_case[10];
    Book books_best_case[10];

    best_case(books_best_case, 10);
    average_case(books_average_case, 10);
    worst_case(books_worst_case, 10);

    Counters* counter = (Counters*)malloc(sizeof(long long) * 2);
    if (!counter) exit(101);
    counter_set_zero(counter);
    selection_sort_books(books_worst_case, 10, counter);
    write_to("Sorted\\Books\\Worst_case.txt", books_worst_case,sizeof(Book), 10);
    printf("[BOOKS WORST CASE]\t swaps: %lld \t comparisions: %lld\n", counter->swaps, counter->comps);
    
    counter_set_zero(counter);
    selection_sort_books(books_average_case, 10, counter);
    printf("[BOOKS AVERAGE CASE]\t swaps: %lld \t comparisions: %lld\n", counter->swaps, counter->comps);

    counter_set_zero(counter);
    selection_sort_books(books_best_case, 10, counter);
    printf("[BOOKS BEST CASE]\t swaps: %lld \t comparisions: %lld\n", counter->swaps, counter->comps);



    
    
    




	return 0;
}
