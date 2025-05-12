#include "header\dir.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <errno.h>
#include <string.h>

#define stat _stat
#define MKDIR(path) _mkdir(path)

void __init__() {
    create_folder_structure("."); // З папки проекту (де розташований main.c)
}

/*
Структура:
    root/
     Sorted/
        Shell/
            -Shell_case_1.txt
            -Shell_case_2.txt
            -Shell_case_3.txt
            -Shell_case_4.txt
            -Shell_case_5.txt
        Books/
            -Worst_case.txt
            -Average_case.txt
            -Best_case.txt
     Testcases/
        -testcases.txt
        -books.json
*/

static int create_folder(const char* path) {
    if (MKDIR(path) != 0) {
        if (errno == EEXIST) return 0;
        fprintf(stderr, "Не вдалося створити теку \"%s\"\n", path);
        return -1;
    }
    return 0;
}


static int create_file(const char* path) {
    struct stat st;
    // Якщо stat повернув 0 — файл існує
    if (stat(path, &st) == 0) {
        return 1;
    }

    FILE* f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "Не вдалося створити файл \"%s\": %s\n",
            path, strerror(errno));
        return -1;
    }
    fclose(f);
    return 0;
}



static int create_folder_structure(const char* root_path) {
    char buf[1024];

    if (create_folder(root_path) < 0) return -1;

    snprintf(buf, sizeof(buf), "%s/Sorted", root_path);
    if (create_folder(buf) < 0) return -1;

    snprintf(buf, sizeof(buf), "%s/Sorted/Shell", root_path);
    if (create_folder(buf) < 0) return -1;

    for (int i = 1; i <= 5; ++i) {
        snprintf(buf, sizeof(buf), "%s/Sorted/Shell/Shell_case_%d.txt", root_path, i);
        if (create_file(buf) < 0) return -1;
    }

    snprintf(buf, sizeof(buf), "%s/Sorted/Books", root_path);
    if (create_folder(buf) < 0) return -1;

    const char* book_files[] = {
        "Worst_case.txt",
        "Average_case.txt",
        "Best_case.txt"
    };
    for (int i = 0; i < 3; ++i) {
        snprintf(buf, sizeof(buf), "%s/Sorted/Books/%s", root_path, book_files[i]);
        if (create_file(buf) < 0) return -1;
    }

    snprintf(buf, sizeof(buf), "%s/Testcases", root_path);
    if (create_folder(buf) < 0) return -1;

    snprintf(buf, sizeof(buf), "%s/Testcases/" SHELL_TEST_FILE, root_path);
    if (create_file(buf) < 0) return -1;
    snprintf(buf, sizeof(buf), "%s/Testcases/" BOOKS_TEST_FILE, root_path);
    if (create_file(buf) < 0) return -1;

    return 0;
}