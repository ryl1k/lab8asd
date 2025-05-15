#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "header\dir.h"   // Structure
#include "header\cases.h" // JSON + constants

#define MAX_HEIGHT 40

#pragma execution_character_set("UTF-8")

void selection_sort_books(Book arr[], int n, Counters* cnt);
void selection_sort_doubles(double arr[], int n, Counters* cnt);
int* calculate_distances(int count, ...);
void visualize();
void _visualize(const double* arr, size_t size, int highlight1, int highlight2);
void shell_sort(double arr[], const size_t n, Counters* cnt, int __visualize);
void randomize(const size_t len);
size_t count_unique(double* arr, size_t n);
void print_books(Book* books, size_t size);
int issorted(double* arr, size_t len);

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

void selection_sort_doubles(double arr[], int n, Counters* cnt) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            cnt->comps++;
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        if (min != i) {
            double tmp = arr[i];
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

void visualize() {
    FILE* file;
    double* arr = (double*)calloc(20, sizeof(double));
    if (!arr) {
        perror("malloc error [MAIN VISUALIZE]");
        exit(81);
    }
    randomize(20);
    Counters* cnt = (Counters*)malloc(sizeof(Counters));
    counter_set_zero(cnt);

    if (fopen_s(&file, "./Testcases/" SHELL_TEST_FILE, "rb")) {
        exit(82);
    }
    size_t read = fread(arr, sizeof(double), 20, file);
    if (read < 20) {
        exit(83);
    }

    shell_sort(arr, 20, cnt, 1);
    Sleep(1000);
    free(arr);
}

void _visualize(const double* arr, size_t size, int highlight1, int highlight2) {
    system("cls");
    double min = -130.0;
    double max = 130.0;
    double range = max - min;              
    double scale = range / MAX_HEIGHT;

    // +130 до -130
    for (int level = MAX_HEIGHT; level >= 0; level--) {
        double threshold = min + level * scale; 

        for (size_t i = 0; i < size; i++) {
            if (arr[i] >= threshold) {
                if ((int)i == highlight1 || (int)i == highlight2)
                    printf(" ## ");
                else
                    printf(" || ");
            }
            else {
                printf("    ");
            }
        }
        printf("\n");
    }

    // Низ
    for (size_t i = 0; i < size; i++) printf("----");
    printf("\n");
    for (size_t i = 0; i < size; i++) printf("%3.0f ", arr[i]);
    printf("\n");

    Sleep(1);
}

void shell_sort(double arr[], const size_t n, Counters* cnt,int __visualize) {
    const int gaps_count = 3;
    int* gaps = calculate_distances(gaps_count,8,3,1); // 8 3 1

    for (int gi = 0; gi < gaps_count; gi++) {
        int gap = gaps[gi];
        if (gap == 0) continue;
        for (int i = gap; i < n; i++) {
            double temp = arr[i];
            int j = i;
            
            while (j >= gap) {
                cnt->comps++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                    cnt->swaps++;
                }
                else {
                    break;
                }

                if (__visualize) {
                    _visualize(arr, n, j, j + gap);
                }
            }
            arr[j] = temp;
            if (__visualize) {
                _visualize(arr, n, j, i);
            }
        }
    }
    free(gaps);
}

void randomize(const size_t len) {
    FILE* log = NULL;
    char path[100];
    snprintf(path, sizeof(path), "./Testcases/" SHELL_TEST_FILE);
    if (fopen_s(&log, path, "wb")) exit(91);

    
    for (size_t i = 0; i < len; i++) {
        int int_part = LOWER_LIMIT + rand() % (UPPER_LIMIT - LOWER_LIMIT + 1);
        double frac_part = ((double)(rand() % 1000000)) / 1000000.0;
        double num = int_part >= 0 ? (int_part + frac_part) : (int_part - frac_part);
        fwrite(&num, sizeof(double), 1, log);
    }
    fclose(log);
}

size_t count_unique(double* arr, size_t n) {
    size_t count = 1;
    for (size_t i = 1; i < n; i++) {
        if (arr[i] != arr[i - 1]) count++;
    }
    return count;
}

void print_books(Book* books, size_t size) {
    for (int i = 0; i < size; i++) {
        printf("%5d | %-10s | %s\n", i + 1, books[i].author, books[i].title);
    }
}

int issorted(double* arr, size_t len) {
    for (size_t i = 0; i < (len - 1); i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
	SetConsoleOutputCP(65001);
    srand(time(NULL));

    //              --- Ініціалізація ---
    // Ініціалізація масивів для сортування книжок, лічильників
    // Створення структури файлів для запису результатів
    
    __init__();

    Book books_worst_case[10];
    Book books_average_case[10];
    Book books_best_case[10];

    best_case(books_best_case, 10);
    average_case(books_average_case, 10);
    worst_case(books_worst_case, 10);

    Counters** bookcounter = (Counters*)malloc(3 * sizeof(void*));
    Counters** shellDoubleCounter = (Counters*)malloc(TESTCASES * sizeof(void*));
    Counters** selectionDoubleCounter = (Counters*)malloc(TESTCASES * sizeof(void*));
    if (!bookcounter || !shellDoubleCounter || !selectionDoubleCounter) { 
        exit(101);
    }
    for (int i = 0; i < 3; i++) {
        bookcounter[i] = (Counters*)malloc(sizeof(Counters));
        if(!bookcounter[i]){
            exit(102);
        }
        counter_set_zero(bookcounter[i]);
    }
    for (int i = 0; i < TESTCASES; i++) {
        selectionDoubleCounter[i] = (Counters*)malloc(sizeof(Counters));
        shellDoubleCounter[i] = (Counters*)malloc(sizeof(Counters));
        if (!selectionDoubleCounter[i] || !shellDoubleCounter[i]) {
            exit(103);
        }
        counter_set_zero(shellDoubleCounter[i]);
        counter_set_zero(selectionDoubleCounter[i]);
    }

    //          --- Сортування книжок ---
    // Сортування масивів книжок, та обрахунок часу на їх сортування
    // Обрахунок операцій та запис посортованого масиву

    clock_t start_books_worst_case = clock();
    selection_sort_books(books_worst_case, 10, bookcounter[0]);
    clock_t end_books_worst_case = clock();
    double elapsed_books_worst_case = (double)(end_books_worst_case - start_books_worst_case) / CLOCKS_PER_SEC;
    printf("[BOOKS_WORST_CASE] sorted 10 values, in %.4f seconds\n\n", elapsed_books_worst_case);
    cnt_write_to("Sorted\\Books\\Worst_case.txt", bookcounter[0], sizeof(Counters), 1);
    book_add_to("Sorted\\Books\\Worst_case.txt", books_worst_case, sizeof(Book), 10);
    
    clock_t start_books_average_case = clock();
    selection_sort_books(books_average_case, 10, bookcounter[1]);
    clock_t end_books_average_case = clock();
    double elapsed_books_average_case = (double)(end_books_average_case - start_books_average_case) / CLOCKS_PER_SEC;
    printf("[BOOKS_AVERAGE_CASE] sorted 10 values, in %.4f seconds\n\n", elapsed_books_average_case);
    cnt_write_to("Sorted\\Books\\Average_case.txt", bookcounter[1], sizeof(Counters), 1);
    book_add_to("Sorted\\Books\\Average_case.txt", books_average_case, sizeof(Book), 10);

    clock_t start_books_best_case = clock();
    selection_sort_books(books_best_case, 10, bookcounter[2]);
    clock_t end_books_best_case = clock();
    double elapsed_books_best_case = (double)(end_books_best_case - start_books_best_case) / CLOCKS_PER_SEC;
    printf("[BOOKS_BEST_CASE] sorted 10 values, in %.4f seconds\n\n", elapsed_books_best_case);
    cnt_write_to("Sorted\\Books\\Best_case.txt", bookcounter[2], sizeof(Counters), 1);
    book_add_to("Sorted\\Books\\Best_case.txt", books_best_case, sizeof(Book), 10);
    

    //          --- Сортування дійсних чисел ---
    // Сортування масивів чисел, та обрахунок часу на їх сортування
    // Обрахунок операцій та запис посортованого масиву
    // Показові сортування алгоритмами:
    // Шелла, вставкою до 100000 елементів

    double* shell_time = (double*)malloc(TESTCASES*sizeof(double));
    double* sel_time = (double*)malloc(TESTCASES*sizeof(double));

    clock_t start_overall_shell = clock();
    for (int i = 0; i < TESTCASES; i++) {
        printf("sorting for case %d (%zu values), waiting...\n", i + 1, TEST_VALUES[i]);

        FILE* file = NULL;
        counter_set_zero(shellDoubleCounter[i]);
        randomize(TEST_VALUES[i]);

        if (fopen_s(&file, "./Testcases/" SHELL_TEST_FILE, "rb")) {
            exit(111);
        }
        double* arr = (double*)calloc(TEST_VALUES[i], sizeof(double));
        double* selarr = (double*)calloc(TEST_VALUES[i], sizeof(double));
        if (!arr || !selarr) {
            perror("malloc error [MAIN FOR LOOP]");
            exit(112);
        }
        
        size_t read_shell = fread(arr, sizeof(double), TEST_VALUES[i], file);
        rewind(file);
        size_t read_sel = fread(selarr, sizeof(double), TEST_VALUES[i], file);
        if (read_shell < TEST_VALUES[i] || read_sel < TEST_VALUES[i]) {
            exit(113);
        }
        fclose(file);

        clock_t start_sel = clock();
        selection_sort_doubles(selarr, TEST_VALUES[i], selectionDoubleCounter[i]);
        clock_t end_sel = clock();

        clock_t start_shell = clock();
        shell_sort(arr, TEST_VALUES[i], shellDoubleCounter[i], 0);
        clock_t end_shell = clock();

        shell_time[i] = (double)(end_shell - start_shell) / CLOCKS_PER_SEC;
        sel_time[i] = (double)(end_sel - start_sel) / CLOCKS_PER_SEC;



        printf("[DEBUG] unique values: %zu / %zu\n", count_unique(arr, TEST_VALUES[i]), TEST_VALUES[i]);
        if (issorted(arr, TEST_VALUES[i]) && issorted(selarr, TEST_VALUES[i])) {
            FILE* log;
            char path[100];
            snprintf(path, sizeof(path), "./Sorted/Shell/Shell_case_%d.txt", i + 1);
            if (fopen_s(&log, path, "a")) {
                exit(114);
            }
            fprintf(log, "shell comps: %lld\t swaps: %lld\n", shellDoubleCounter[i]->comps, shellDoubleCounter[i]->swaps);
            fprintf(log, "sel comps: %lld\t swaps: %lld\n", selectionDoubleCounter[i]->comps, selectionDoubleCounter[i]->swaps);
            fprintf(log, "%zu values, shell done in: %.4f seconds, sel done in: %.4f seconds\n", 
                TEST_VALUES[i], shell_time[i],sel_time[i]);
            for (size_t j = 0; j < TEST_VALUES[i]; j++) {
                fprintf(log, "%.6lf\n", arr[j]);
            }
            fclose(log);
        }
        else {
            exit(115);
        }

        printf("[CASE %d] sorted %zu values, shell done in: %.4f seconds, sel done in: %.4f seconds\n\n"
            , i + 1, TEST_VALUES[i],shell_time[i],sel_time[i]);
        free(arr);
        free(selarr);
    }
    clock_t end_overall_shell = clock();
    double elapsed_overall_shell = (double)(end_overall_shell - start_overall_shell) / CLOCKS_PER_SEC;
    printf("[SORTING DONE] in %.4f seconds", elapsed_overall_shell);
    Sleep(3000);
    system("cls");

    // --- Візуалізація ---
    // visualize();
    printf("\n\t*---------------------------------------------------------------------------------------------------*\n");
    printf("\t|%-8s | %-12s | %-12s | %-12s || %-12s | %-12s | %-12s|\n",
        "N",
        "Selec time", "Selec comps", "Selec swaps",
        "Shell time", "Shell comps", "Shell swaps");
    printf("\t*---------------------------------------------------------------------------------------------------*\n");
    for (int i = 0; i < TESTCASES; i++) {
        printf("\t|%-8zu | %-12.4f | %-12lld | %-12lld || %-12.4f | %-12lld | %-12lld|\n",
            TEST_VALUES[i],
            sel_time[i], 
            selectionDoubleCounter[i]->comps, 
            selectionDoubleCounter[i]->swaps,
            shell_time[i], 
            shellDoubleCounter[i]->comps,
            shellDoubleCounter[i]->swaps);
    }
    printf("\t*---------------------------------------------------------------------------------------------------*\n");
    // А тепер окремо selection_sort_books (N = 10) для трьох кейсів
    printf("\n\t*----------------------------------*\n");
    printf("\t|%8s | %10s | %10s|\n", "Books", "Comps", "Swaps");
    printf("\t*----------------------------------*\n");
    printf("\t|%8s | %10lld | %10lld|\n",
        "Worst", bookcounter[0]->comps, bookcounter[0]->swaps);
    printf("\t|%8s | %10lld | %10lld|\n",
        "Average", bookcounter[1]->comps, bookcounter[1]->swaps);
    printf("\t|%8s | %10lld | %10lld|\n",
        "Best", bookcounter[2]->comps, bookcounter[2]->swaps);
    printf("\t*----------------------------------*\n");

	return 0;
}
