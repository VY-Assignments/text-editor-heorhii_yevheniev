#include <stdio.h>
#include <stdlib.h>

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