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

static errno_t errcode = 0;

static int cmp_desc(const void* a, const void* b);
void selection_sort_books(Book arr[], int n, Counters* cnt);
void selection_sort_doubles(double arr[], int n, Counters* cnt);
int* calculate_distances(SequenceType seq, size_t n, int* out_count);
errno_t visualize(void);
void _visualize(const double* arr, size_t size, int highlight1, int highlight2);
void shell_sort(double arr[], const size_t n, Counters* cnt, SequenceType seq, int __visualize);
double* randomize(double* arr, const size_t len);
errno_t randomize_to_file(const size_t len);
size_t count_unique(double* arr, size_t n);
void print_books(Book* books, size_t size);
int issorted(double* arr, size_t len);

static int cmp_desc(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ib - ia;
}

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

int* calculate_distances(SequenceType seq, size_t n, int* out_count) {
    int count = 0;
    switch (seq) {
    case SEQ_POW2:
        for (size_t t = n >> 1; t >= 1; t >>= 1) {
            count++;
        }
        break;
    case SEQ_2C_POW_3_D: {
        for (size_t c = 1; c <= n; c <<= 1) {
            for (size_t d = c; d <= n; d *= 3) {
                count++;
            }
        }
        break;
    }
    case SEQ_SEDGEWICK2: {
        for (int p = 1;; p++) {
            double res = (p % 2 == 0)
                ? 9.0 * (pow(2.0, p) - pow(2.0, p / 2)) + 1.0
                : 8.0 * pow(2.0, p) - 6.0 * pow(2.0, (p + 1) / 2) + 1.0;
            if (res > (double)n) {
                break;
            }
            count++;
        }
        break;
    }
    }

    if (count == 0) {
        *out_count = 0;
        return NULL;
    }

    int* gaps = malloc(count * sizeof * gaps);
    if (!gaps) {
        *out_count = 0;
        return NULL;
    }

    int idx = 0;
    switch (seq) {
    case SEQ_POW2:
        for (size_t temp = n >> 1; temp >= 1; temp >>= 1) {
            gaps[idx++] = (int)temp;
        }
        break;

    case SEQ_2C_POW_3_D: {
        int* temp = malloc(count * sizeof(int));
        if (!temp) {
            *out_count = 0;
            return NULL;
        }
        int tempcount = 0;
        for (size_t c = 1; c <= n; c <<= 1) {
            for (size_t d = c; d <= n; d *= 3) {
                temp[tempcount++] = (int)d;
            }
        }
        qsort(temp, tempcount, sizeof temp[0], cmp_desc);
        for (int i = 0; i < tempcount; i++) {
            gaps[idx++] = temp[i];
        }

        free(temp);
        break;
    }

    case SEQ_SEDGEWICK2:
        for (int p = 1;; p++) {
            double res = (p % 2 == 0)
                ? 9.0 * (pow(2.0, p) - pow(2.0, p / 2)) + 1.0
                : 8.0 * pow(2.0, p) - 6.0 * pow(2.0, (p + 1) / 2) + 1.0;
            if (res > (double)n) {
                break;
            }
            gaps[idx++] = (int)res;
        }
        break;
    }

    if (count == 0 || gaps[count - 1] != 1) {
        gaps = realloc(gaps, sizeof(int) * (count + 1));
        if (!gaps) {
            *out_count = 0;
            return NULL;
        }
        gaps[count++] = 1;
    }
    
    *out_count = count;
    return gaps;
}

errno_t visualize(void) {
    double* arr = (double*)calloc(20, sizeof(double));
    if (!arr) {
        perror("malloc error [MAIN VISUALIZE]");
        return 81;
    }
    randomize(arr,20);
    Counters* cnt = (Counters*)malloc(sizeof(Counters));
    counter_set_zero(cnt);

    shell_sort(arr, 20, cnt, SEQ_SEDGEWICK2, 1);
    Sleep(1000);
    free(arr);
    free(cnt);
    return 0;
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

void shell_sort(double arr[], const size_t n, Counters* cnt, SequenceType seq, int __visualize) {
    int gap_count = 0;
    int* gaps = calculate_distances(seq,n,&gap_count); 

    for (int gi = 0; gi < gap_count; gi++) {
        int gap = gaps[gi];
        if (gap == 0) {
            continue;
        }
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

double* randomize(double* arr, const size_t len) {
    for (size_t i = 0; i < len; i++) {
        int int_part = LOWER_LIMIT + rand() % (UPPER_LIMIT - LOWER_LIMIT + 1);
        double frac_part = ((double)(rand() % 1000000)) / 1000000.0;
        arr[i] = ((int_part >= 0) ? (int_part + frac_part) : (int_part - frac_part));
    }
    return arr;
}

errno_t randomize_to_file(const size_t len) {
    FILE* log = NULL;
    char path[100];
    snprintf(path, sizeof(path), "./Testcases/" SHELL_TEST_FILE);
    if (fopen_s(&log, path, "wb")) {
        return 91;
    }
    double* arr = (double*)calloc(len, sizeof(double));
    randomize(arr, len);
    
    for (size_t i = 0; i < len; i++) {
        fwrite(&arr[i], sizeof(double), 1, log);
    }
    free(arr);
    fclose(log);
    return 0;
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
    srand((unsigned)time(NULL));

    //              --- Ініціалізація ---
    // Ініціалізація масивів для сортування книжок, лічильників
    // Створення структури файлів для запису результатів
    
    __init__();


    Book books_worst_case[10];
    Book books_average_case[10];
    Book books_best_case[10];

    double shell8time[TESTCASES];
    double shell3time[TESTCASES];
    double shell1time[TESTCASES];
    double sel_time[TESTCASES];

    best_case(books_best_case, 10);
    average_case(books_average_case, 10);
    worst_case(books_worst_case, 10);

    Counters** bookcounter = (Counters*)malloc(3 * sizeof(*bookcounter));
    Counters** shell8Counter = (Counters*)malloc(TESTCASES * sizeof(*shell8Counter));
    Counters** shell3Counter = (Counters*)malloc(TESTCASES * sizeof(*shell3Counter));
    Counters** shell1Counter = (Counters*)malloc(TESTCASES * sizeof(*shell1Counter));
    Counters** selectionDoubleCounter = (Counters*)malloc(TESTCASES * sizeof(*selectionDoubleCounter));
    if (!bookcounter || !selectionDoubleCounter || !shell8Counter|| !shell3Counter || !shell1Counter) {
        errcode = 101;
        goto cleanup;
    }
    for (int i = 0; i < 3; i++) {
        bookcounter[i] = (Counters*)malloc(sizeof(*bookcounter[i]));
        if(!bookcounter[i]){
            errcode = 102;
            goto cleanup;
        }
        counter_set_zero(bookcounter[i]);
    }
    for (int i = 0; i < TESTCASES; i++) {
        selectionDoubleCounter[i] = (Counters*)malloc(sizeof(*selectionDoubleCounter[i]));
        shell1Counter[i] = (Counters*)malloc(sizeof(*shell1Counter[i]));
        shell3Counter[i] = (Counters*)malloc(sizeof(*shell3Counter[i]));
        shell8Counter[i] = (Counters*)malloc(sizeof(*shell8Counter[i]));
        if (!selectionDoubleCounter[i] || !shell1Counter[i] ||
            !shell3Counter[i] || !shell8Counter[i]) {
            errcode = 103;
            goto cleanup;
        }
        counter_set_zero(shell1Counter[i]);
        counter_set_zero(shell3Counter[i]);
        counter_set_zero(shell8Counter[i]);
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

    clock_t start_overall_shell = clock();
    for (int i = 0; i < TESTCASES; i++) {
        printf("sorting for case %d (%zu values), waiting...\n", i + 1, TEST_VALUES[i]);

        FILE* file = NULL;

        counter_set_zero(shell1Counter[i]);
        counter_set_zero(shell3Counter[i]);
        counter_set_zero(shell8Counter[i]);
        counter_set_zero(selectionDoubleCounter[i]);
        if (randomize_to_file(TEST_VALUES[i])) {
            goto cleanup;
        }

        if (fopen_s(&file, "./Testcases/" SHELL_TEST_FILE, "rb")) {
            errcode = 111;
            goto cleanup;
        }
        double* arrshell1 = (double*)calloc(TEST_VALUES[i], sizeof(double));
        double* arrshell3 = (double*)calloc(TEST_VALUES[i], sizeof(double));
        double* arrshell8 = (double*)calloc(TEST_VALUES[i], sizeof(double));
        double* selarr = (double*)calloc(TEST_VALUES[i], sizeof(double));
        if (!arrshell1 || !arrshell3 || !arrshell8 || !selarr) {
            perror("malloc error [MAIN FOR LOOP]");
            errcode = 112;
            goto cleanup;
        }
        
        size_t read_shell1 = fread(arrshell1, sizeof(double), TEST_VALUES[i], file);
        rewind(file);
        size_t read_shell3 = fread(arrshell3, sizeof(double), TEST_VALUES[i], file);
        rewind(file);
        size_t read_shell8 = fread(arrshell8, sizeof(double), TEST_VALUES[i], file);
        rewind(file);
        size_t read_sel = fread(selarr, sizeof(double), TEST_VALUES[i], file);
        if (read_shell1 < TEST_VALUES[i] || read_sel < TEST_VALUES[i] ||
            read_shell3 < TEST_VALUES[i] || read_shell8 < TEST_VALUES[i]) {
            errcode = 113;
            goto cleanup;
        }
        fclose(file);

        clock_t start_sel = clock();
        selection_sort_doubles(selarr, TEST_VALUES[i], selectionDoubleCounter[i]);
        clock_t end_sel = clock();

        clock_t start_shell8 = clock();
        shell_sort(arrshell8, TEST_VALUES[i], shell8Counter[i], SEQ_POW2, 0);
        clock_t end_shell8 = clock();

        clock_t start_shell3 = clock();
        shell_sort(arrshell3, TEST_VALUES[i], shell3Counter[i], SEQ_2C_POW_3_D, 0);
        clock_t end_shell3 = clock();

        clock_t start_shell1 = clock();
        shell_sort(arrshell1, TEST_VALUES[i], shell1Counter[i], SEQ_SEDGEWICK2, 0);
        clock_t end_shell1 = clock();

        shell8time[i] = (double)(end_shell8 - start_shell8) / CLOCKS_PER_SEC;
        shell3time[i] = (double)(end_shell3 - start_shell3) / CLOCKS_PER_SEC;
        shell1time[i] = (double)(end_shell1 - start_shell1) / CLOCKS_PER_SEC;
        sel_time[i] = (double)(end_sel - start_sel) / CLOCKS_PER_SEC;

        printf("[DEBUG] unique values: %zu / %zu\n", count_unique(selarr, TEST_VALUES[i]), TEST_VALUES[i]);
        if (issorted(arrshell1, TEST_VALUES[i]) && issorted(selarr, TEST_VALUES[i])) {
            FILE* log;
            char path[100];
            snprintf(path, sizeof(path), "./Sorted/Shell/Shell_case_%d.txt", i + 1);
            if (fopen_s(&log, path, "a")) {
                errcode = 114;
                goto cleanup;
            }
            fprintf(log, "shell (sequence 1)  comps: %lld\t swaps: %lld\n", 
                shell1Counter[i]->comps, shell1Counter[i]->swaps);
            fprintf(log, "shell (sequence 3) comps: %lld\t swaps: %lld\n",
                shell3Counter[i]->comps, shell3Counter[i]->swaps);
            fprintf(log, "shell (sequence 8) comps: %lld\t swaps: %lld\n",
                shell8Counter[i]->comps, shell8Counter[i]->swaps);
            fprintf(log, "sel comps: %lld\t swaps: %lld\n", selectionDoubleCounter[i]->comps, selectionDoubleCounter[i]->swaps);
            fprintf(log, "%zu values\n", TEST_VALUES[i]);
            fprintf(log, "Shell sort with sequence 1 done in: %.4f seconds\n", shell1time[i]);
            fprintf(log, "Shell sort with sequence 3 done in: %.4f seconds\n", shell3time[i]);
            fprintf(log, "Shell sort with sequence 8 done in: %.4f seconds\n", shell8time[i]);
            fprintf(log, "Selection sort done in: %.4f seconds\n", sel_time[i]);

            for (size_t j = 0; j < TEST_VALUES[i]; j++) {
                fprintf(log, "%.6lf\n", arrshell8[j]);
            }
            fclose(log);
        }
        else {
            errcode = 115;
            goto cleanup;
        }

        printf("[CASE %d] sorted %zu values, shell (8) done in: %.4f seconds, sel done in: %.4f seconds\n\n"
            , i + 1, TEST_VALUES[i],shell8time[i],sel_time[i]);
        free(arrshell1);
        free(arrshell3);
        free(arrshell8);
        free(selarr);
    }
    clock_t end_overall_shell = clock();
    double elapsed_overall_shell = (double)(end_overall_shell - start_overall_shell) / CLOCKS_PER_SEC;
    printf("[SORTING DONE] in %.4f seconds", elapsed_overall_shell);
    Sleep(3000);
    system("cls");

    //              --- Візуалізація ---
    // 
    // visualize();


    printf("\n*------------------------------------------------------- Shell Sort ----------------------------------------------------------------*\n");
    printf("|   N    | 8-seq t(s) | 8-seq comps | 8-seq swaps | 3-seq t(s) | 3-seq comps | 3-seq swaps | 1-seq t(s) | 1-seq comps | 1-seq swaps |\n");
    printf("|--------|------------|-------------|-------------|------------|-------------|-------------|------------|-------------|-------------|\n");
    for (int i = 0; i < TESTCASES; i++) {
        printf("| %6zu | %10.4f | %11lld | %11lld | %10.4f | %11lld | %11lld | %10.4f | %11lld | %11lld |\n",
            TEST_VALUES[i],
            shell8time[i], shell8Counter[i]->comps, shell8Counter[i]->swaps,
            shell3time[i], shell3Counter[i]->comps, shell3Counter[i]->swaps,
            shell1time[i], shell1Counter[i]->comps, shell1Counter[i]->swaps
        );
    }
    printf("*-----------------------------------------------------------------------------------------------------------------------------------*\n\n");

    // Додаємо вивід таблиці Selection Sort (double):
    printf("*---------------- Selection Sort ----------------*\n");
    printf("|   N    | Sel t(s)  | Sel comps   | Sel swaps   |\n");
    printf("|--------|-----------|-------------|-------------|\n");
    for (int i = 0; i < TESTCASES; i++) {
        printf("| %6zu | %9.4f | %11lld | %11lld |\n",
            TEST_VALUES[i],
            sel_time[i],
            selectionDoubleCounter[i]->comps,
            selectionDoubleCounter[i]->swaps
        );
    }
    printf("*------------------------------------------------*\n\n");

    // І окрема коротка таблиця для сортування книжок (N=10):
    printf("*------- Books Sort (N=10) -------*\n");
    printf("|  Case   | Comps     | Swaps     |\n");
    printf("|---------|-----------|-----------|\n");
    printf("| Worst   | %9lld | %9lld |\n",
        bookcounter[0]->comps,
        bookcounter[0]->swaps
    );
    printf("| Average | %9lld | %9lld |\n",
        bookcounter[1]->comps,
        bookcounter[1]->swaps
    );
    printf("| Best    | %9lld | %9lld |\n",
        bookcounter[2]->comps,
        bookcounter[2]->swaps
    );
    printf("*---------------------------------*\n");


    //          --- Очистка пам'яті ---

cleanup:
    for (int i = 0; i < 3; i++) {
        free(bookcounter[i]);
    }
    for (int i = 0; i < TESTCASES; i++) {
        free(shell1Counter[i]);
        free(shell3Counter[i]);
        free(shell8Counter[i]);
        free(selectionDoubleCounter[i]);
    }
    free(bookcounter);
    free(shell1Counter);
    free(shell3Counter);
    free(shell8Counter);
    free(selectionDoubleCounter);


	return errcode;
}
