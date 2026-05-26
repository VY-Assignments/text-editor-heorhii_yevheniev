#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemoStor {
    char** lines;
    int lines_count;
    int capacity;
};

struct MemoStor* call_editor() {
    struct MemoStor* arr = (struct MemoStor*)malloc(sizeof(struct MemoStor));
    arr->lines_count = 1;
    arr->capacity = 2;
    arr->lines = (char**)malloc(arr->capacity * sizeof(char*));
    arr->lines[0] = (char*)malloc(1 * sizeof(char));
    arr->lines[0][0] = '\0';
    return arr;
}

void Append(struct MemoStor* arr) {
    char input[256];
    printf("enter text you want to append\n");
    while (getchar() != '\n');

    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    char *current_line = arr->lines[arr->lines_count - 1];
    int currentL_len = strlen(current_line);
    int new_len = strlen(input);
    char *tryy = realloc(arr->lines[arr->lines_count - 1], (currentL_len + new_len+1) * sizeof(char));
    if (tryy != NULL) {
        arr->lines[arr->lines_count - 1] = tryy;
        strcat(arr->lines[arr->lines_count - 1], input);
        printf("text appended successfully\n");
    }
    else {
        printf("Malloc failed\n");
    }
}

void start_new_line(struct MemoStor* arr) {
    if (arr->lines_count >= arr->capacity) {
        int new_capacity = arr->capacity *= 2;
        char** tryy = realloc(arr->lines, new_capacity * sizeof(char*));
        if (tryy == NULL) {
            printf("malloc failed\n");
            return;
        }
        arr->lines = tryy;
        arr->capacity = new_capacity;
    }

    arr->lines[arr->lines_count] = (char*)malloc(1 * sizeof(char));
    if (arr->lines[arr->lines_count] == NULL) {
        printf("malloc failed\n");
        return;
    }
    arr->lines[arr->lines_count][0] = '\0';
    arr->lines_count++;
    printf("new line started\n");
}

void save_to_file(struct MemoStor* arr) {
    char filename[100];
    printf("enter name for file to save text in(.txt)\n");
    scanf("%s", filename);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("file could not be opened\n");
        return;
    }
    for (int i = 0; i < arr->lines_count; i++) {
        fprintf(file, "%s\n", arr->lines[i]);
    }
    fclose(file);
    printf("text saved to %s\n", filename);
}

void load_from_file(struct MemoStor* arr) {
    char filename[100];
    printf("enter name for file to load text from(.txt)");
    scanf("%s", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("file could not be opened\n");
        return;
    }
    char input[200];
    while (fgets(input, sizeof(input), file) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (arr->lines_count == 1 && arr->lines[0][0] == '\0') {
            int str_len = strlen(input);
            char* tryy = realloc(arr->lines[arr->lines_count - 1], (str_len + 1) * sizeof(char));
            if (tryy != NULL) {
                arr->lines[arr->lines_count - 1] = tryy;
                strcat(arr->lines[arr->lines_count - 1], input);
            }
            else {
                printf("Malloc failed\n");
            }
        }
        else {
            start_new_line(arr);
            int str_len = strlen(input);
            char* tryy = realloc(arr->lines[arr->lines_count - 1], (str_len + 1) * sizeof(char));
            if (tryy != NULL) {
                arr->lines[arr->lines_count - 1] = tryy;
                strcat(arr->lines[arr->lines_count - 1], input);
            }
            else {
                printf("Malloc failed\n");
            }
        }
    }
    fclose(file);
    printf("text from file aploaded successfully\n");
}

void print_current(struct MemoStor* arr) {
    for (int i = 0; i < arr->lines_count; i++) {
        printf("%d: %s\n", i + 1, arr->lines[i]);
    }
}

void insert_text(struct MemoStor* arr) {
    int line;
    int pos;
    char input[200];
    printf("which line insert to?\n");
    scanf("%d", &line);
    if (line < 0 || line > arr->lines_count) {
        printf("Error: line number out of range");
        return;
    }
    printf("to which position insert?\n");
    scanf("%d", &pos);
    while (getchar() != '\n');
    printf("enter text you want to insert\n");

    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    int currentL_len = strlen(arr->lines[line]);
    int new_len = strlen(input);

    if (pos > currentL_len) pos = currentL_len;
    char* tryy = realloc(arr->lines[line], (currentL_len + new_len + 1) * sizeof(char));
    if (tryy != NULL) {
        arr->lines[line] = tryy;
    }
    else {
        printf("Malloc failed\n");
        return;
    }
    memmove(arr->lines[line] + pos + new_len, arr->lines[line] + pos, currentL_len - pos + 1);
    memcpy(arr->lines[line] + pos, input, new_len);
    printf("Text was inserted\n");
}

void search_text(struct MemoStor* arr) {
    char search[100];
    printf("enter text to search for\n");
    while (getchar() != '\n');
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < arr->lines_count; i++) {
        char* found_index = strstr(arr->lines[i], search);

        while (found_index != NULL) {
            printf("Found in line %d at index %d\n", i + 1, found_index - arr->lines[i]);
            found = 1;
            found_index = strstr(found_index + 1, search);
        }
    }
    if (!found) {
        printf("text not found\n");
    }
}
void free_all(struct MemoStor* arr) {
    for (int i = 0; i < arr->lines_count; i++) {
        free(arr->lines[i]);
        arr->lines[i] = NULL;
    }
    free(arr->lines);
    arr->lines = NULL;
    free(arr);
    arr = NULL;
}
void run_editor(struct MemoStor* arr) {
    int choice;
    int running = 1;
    while (running) {
        printf("\navaliable commands:\n");
        printf("1.Append text\n");
        printf("2.Start new line\n");
        printf("3.Save into file\n");
        printf("4.Load from file\n");
        printf("5.Print current text\n");
        printf("6.Insert text by line and index\n");
        printf("7.Search text\n");
        printf("0.Leave\n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number\n");
            while (getchar() != '\n') {
            continue;
            }
        }
        switch (choice) {
        case 1:Append(arr); break;
        case 2:start_new_line(arr); break;
        case 3:save_to_file(arr); break;
        case 4:load_from_file(arr); break;
        case 5:print_current(arr); break;
        case 6:insert_text(arr); break;
        case 7:search_text(arr); break;
        case 0:printf("Exiting\n");
            running = 0;
            break;
        default: printf("Function not implemented\n");
        }

    }
    
}

int main() {
    struct MemoStor* arr = call_editor();
    run_editor(arr);

    free_all(arr);
    return 0;
}