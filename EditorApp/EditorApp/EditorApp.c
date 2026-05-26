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
    printf("new line started");
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
void run_editor() {
    int choice;
    int running = 1;
    while (running) {
        int choice;
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
        case 1:printf("Function not implemented\n"); break;
        case 2:printf("Function not implemented\n"); break;
        case 3:printf("Function not implemented\n"); break;
        case 4:printf("Function not implemented\n"); break;
        case 5:printf("Function not implemented\n"); break;
        case 6:printf("Function not implemented\n"); break;
        case 7:printf("Function not implemented\n"); break;
        case 0:printf("Exiting\n");
            running = 0;
            break;
        default: printf("Function not implemented\n");
        }

    }
}

int main() {
    

    return 0;
}