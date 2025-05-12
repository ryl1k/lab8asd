#include "header/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* _json_read_file(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) { perror("fopen"); return NULL; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    char* buf = (char*)malloc(len + 1);
    if (!buf) { fclose(f); return NULL; }
    if (fread(buf, 1, len, f) != (size_t)len) {
        perror("fread");
        free(buf);
        buf = NULL;
    }
    else {
        buf[len] = '\0';
    }
    fclose(f);
    return buf;
}

int load_books(const char* filename, const char* array_name, Book books[], int max_books){
    char* json_text = _json_read_file(filename);
    if (!json_text) return -1;

    cJSON* root = cJSON_Parse(json_text);
    free(json_text);
    if (!root) {
        fprintf(stderr, "JSON parse error: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    cJSON* arr = cJSON_GetObjectItemCaseSensitive(root, array_name);
    if (!cJSON_IsArray(arr)) {
        fprintf(stderr, "Error: '%s' is not an array in JSON\n", array_name);
        cJSON_Delete(root);
        return -1;
    }

    int count = 0;
    cJSON* item = NULL;
    cJSON_ArrayForEach(item, arr) {
        if (count >= max_books) break;
        cJSON* ja = cJSON_GetObjectItemCaseSensitive(item, "author");
        cJSON* jt = cJSON_GetObjectItemCaseSensitive(item, "title");
        if (cJSON_IsString(ja) && cJSON_IsString(jt)) {
            strcpy_s(books[count].author, AUTHOR_LEN, ja->valuestring);
            strcpy_s(books[count].title, TITLE_LEN, jt->valuestring);
            count++;
        }
    }

    cJSON_Delete(root);
    return count;
}
