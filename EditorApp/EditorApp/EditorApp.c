#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemoStor {
    char** lines;
    int lines_count;
    int capacity;

    struct MemoStor** undo_stack;
    int undo_top;
    int undo_capacity;

    struct MemoStor** redo_stack;
    int redo_top;
    int redo_capacity;

    char* buffer;
};

struct MemoStor* call_editor() {
    struct MemoStor* arr = (struct MemoStor*)malloc(sizeof(struct MemoStor));
    arr->lines_count = 1;
    arr->capacity = 2;
    arr->lines = (char**)malloc(arr->capacity * sizeof(char*));
    arr->lines[0] = (char*)malloc(1 * sizeof(char));
    arr->lines[0][0] = '\0';

    arr->undo_capacity = 5;
    arr->undo_top = 0;
    arr->undo_stack = (struct MemoStor**)malloc(arr->undo_capacity * sizeof(struct MemoStor*));
    for (int i = 0; i < arr->undo_capacity; i++) {
        arr->undo_stack[i] = NULL;
    }

    arr->redo_capacity = 5;
    arr->redo_top = 0;
    arr->redo_stack = (struct MemoStor**)malloc(arr->redo_capacity * sizeof(struct MemoStor*));
    for (int i = 0; i < arr->redo_capacity; i++) {
        arr->redo_stack[i] = NULL;
    }

    arr->buffer = NULL;
    return arr;
}

struct MemoStor* memo_copy(struct MemoStor* arr) {
    struct MemoStor* copy = (struct MemoStor*)malloc(sizeof(struct MemoStor));
    copy->lines_count = arr->lines_count;
    copy->capacity = arr->capacity;
    copy->lines = (char**)malloc(copy->capacity * sizeof(char*));
    if (!copy->lines) {
        free(copy);
        return NULL;
    }
    for (int i = 0; i < arr->lines_count; i++) {
        int len = strlen(arr->lines[i]);
        copy->lines[i] = (char*)malloc((len + 1) * sizeof(char));
        if (!copy->lines[i]) {
            for (int j = 0; j < i;j++) free(copy->lines[j]);
            free(copy->lines);
            free(copy);
            return NULL;
        }
        strcpy(copy->lines[i], arr->lines[i]);
    }
    return copy;
}

void save_state(struct MemoStor* arr) {
    if (arr->undo_top < arr->undo_capacity) {
        arr->undo_stack[arr->undo_top] = memo_copy(arr);
        arr->undo_top++;
    }
}

void undo(struct MemoStor* arr) {
    if (arr->undo_top <= 0) {
        printf("no actions to undo\n");
        return;
    }
    if (arr->redo_top < arr->redo_capacity) {
        arr->redo_stack[arr->redo_top] = memo_copy(arr);
        arr->redo_top++;
    }
    for (int i = 0; i < arr->lines_count; i++) free(arr->lines[i]);
    free(arr->lines);
    arr->undo_top--;
    struct MemoStor* copy = arr->undo_stack[arr->undo_top];
    arr->lines = copy->lines;
    arr->lines_count = copy->lines_count;
    arr->capacity = copy->capacity;

    free(copy);
    printf("undo completed\n");
}

void redo(struct MemoStor* arr) {
    if (arr->redo_top <= 0) {
        printf("no actions to redo\n");
        return;
    }
    save_state(arr);
    for (int i = 0; i < arr->lines_count; i++) free(arr->lines[i]);
    free(arr->lines);
    arr->redo_top--;
    struct MemoStor* copy = arr->redo_stack[arr->redo_top];

    arr->lines = copy->lines;
    arr->lines_count = copy->lines_count;
    arr->capacity = copy->capacity;
    free(copy);
    arr->redo_stack[arr->redo_top] = NULL;
    printf("Redo completed\n");
}

void clear_redo_stack(struct MemoStor* arr) {
    if (arr->redo_stack == NULL) return;

    for (int i = 0; i < arr->redo_top; i++) {
        if (arr->redo_stack[i] != NULL) {
            for (int j = 0; j < arr->redo_stack[i]->lines_count; j++) {
                free(arr->redo_stack[i]->lines[j]);
                arr->redo_stack[i]->lines[j] = NULL;
            }
            free(arr->redo_stack[i]->lines);
            arr->redo_stack[i]->lines = NULL;
            free(arr->redo_stack[i]);
            arr->redo_stack[i] = NULL;
        }
    }
    arr->redo_top = 0;
}

void Append(struct MemoStor* arr) {
    save_state(arr);
    clear_redo_stack(arr);
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
    save_state(arr);
    clear_redo_stack(arr);
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
    save_state(arr);
    clear_redo_stack(arr);
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
    if (pos > strlen(arr->lines[line])) {
        printf("insertion index out of range\n");
        return;
    }
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

void insert_replace_text(struct MemoStor* arr) {
    save_state(arr);
    clear_redo_stack(arr);
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
    if (pos > strlen(arr->lines[line])) {
        printf("insertion index out of range\n");
        return;
    }
    while (getchar() != '\n');
    printf("enter text you want to insert\n");

    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    int currentL_len = strlen(arr->lines[line]);
    int insert_len = strlen(input);
    int new_len = currentL_len;
    if (pos > currentL_len) pos = currentL_len;
    if (currentL_len < pos + insert_len) {
        char* tryy = realloc(arr->lines[line], (pos + insert_len + 1) * sizeof(char));
        if (tryy != NULL) {
            arr->lines[line] = tryy;
        }
        else {
            printf("Malloc failed\n");
            return;
        }
        new_len = pos + insert_len;
    }
    
    
    memcpy(arr->lines[line] + pos, input, insert_len);
    arr->lines[line][new_len] = '\0';
    printf("Text was replaced/inserted\n");
}

void delete_text(struct MemoStor* arr) {
    save_state(arr);
    clear_redo_stack(arr);
    int line, pos, count;
    printf("enter line, index and number of symbols\n");
    scanf("%d %d %d", &line, &pos, &count);

    if (line < 0 || line >= arr->lines_count || pos < 0 || pos >= strlen(arr->lines[line])) {
        printf("invalid input\n");
        return;
    }

    
    char* new_line = arr->lines[line];
    int len = strlen(new_line);
    if (pos + count > len) count = len - pos;

    memmove(new_line + pos, new_line + pos + count, len - pos - count + 1);
    char* tryy = realloc(new_line, (len - count + 1) * sizeof(char));
    if (tryy != NULL) {
        arr->lines[line] = tryy;
    }
    printf("text deleted\n");
}

void copy_text(struct MemoStor* arr) {
    int line, pos, count;
    printf("enter line, index and number of symbols\n");
    scanf("%d %d %d", &line, &pos, &count);
    if (line < 0 || line >= arr->lines_count || pos < 0 || pos >= strlen(arr->lines[line])) {
        printf("wrong input\n");
        return;
    }
    free(arr->buffer);
    arr->buffer = (char*)malloc((count + 1) * sizeof(char));
    if (arr->buffer) {
        strncpy(arr->buffer, arr->lines[line] + pos, count);
        arr->buffer[count] = '\0';
    }
}

void cut_text(struct MemoStor* arr) {
    save_state(arr);
    clear_redo_stack(arr);
    copy_text(arr);
    if (arr->buffer == NULL) {
        printf("unable to load text to buffer\n");
        return;
    }
    delete_text(arr);
    printf("text cut to buffer\n");
}

void paste_text(struct MemoStor* arr) {
    if (arr->buffer == NULL) {
        printf("buffer is empty\n");
        return;
    }
    int line, pos;
    printf("enter line and index\n");
    scanf("%d %d", &line, &pos);
    if (line < 0 || line >= arr->lines_count || pos < 0 || pos >= strlen(arr->lines[line])) {
        printf("wrong input\n");
        return;
    }
    save_state(arr);
    clear_redo_stack(arr);

    int buffer_len = strlen(arr->buffer);
    int current_len = strlen(arr->lines[line]);
    char* tryy = realloc(arr->lines[line], (current_len + buffer_len + 1) * sizeof(char));
    if (tryy) {
        arr->lines[line] = tryy;
        memmove(arr->lines[line] + pos + buffer_len, arr->lines[line] + pos, current_len - pos + 1);
        memcpy(arr->lines[line] + pos, arr->buffer, buffer_len);
    }
}

void free_all(struct MemoStor* arr) {
    for (int i = 0; i < arr->undo_top; i++) {
        if (arr->undo_stack[i] != NULL) {
            for (int j = 0; j < arr->undo_stack[i]->lines_count; j++) {
                free(arr->undo_stack[i]->lines[j]);
                arr->undo_stack[i]->lines[j] = NULL;
            }
            free(arr->undo_stack[i]->lines);
            arr->undo_stack[i]->lines = NULL;
            free(arr->undo_stack[i]);
            arr->undo_stack[i] = NULL;
        }
    }
    free(arr->undo_stack);
    arr->undo_stack = NULL;
    
    for (int i = 0; i < arr->redo_top;i++) {
        if (arr->redo_stack[i]) {
            for (int j = 0; j < arr->redo_stack[i]->lines_count; j++) {
                free(arr->redo_stack[i]->lines[j]);
                arr->redo_stack[i]->lines[j] = NULL;
            }
            free(arr->redo_stack[i]->lines);
            arr->redo_stack[i]->lines = NULL;
            free(arr->redo_stack[i]);
            arr->redo_stack[i] = NULL;
        }
    }
    free(arr->redo_stack);
    arr->redo_stack = NULL;

    free(arr->buffer);
    arr->buffer = NULL;

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
        printf("8.Insert with replacement by line and index\n");
        printf("9.undo\n");
        printf("10.redo\n");
        printf("11.cut\n");
        printf("12.copy\n");
        printf("13.paste\n");
        printf("14.delete text\n");
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
        case 8:insert_replace_text(arr); break;
        case 9:undo(arr);break;
        case 10:redo(arr);break;
        case 11:cut_text(arr);break;
        case 12:copy_text(arr);break;
        case 13:paste_text(arr);break;
        case 14:delete_text(arr);break;
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