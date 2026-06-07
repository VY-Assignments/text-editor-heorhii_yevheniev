#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void PasswordChecker() {
    int score = 0;
    char password[256];
    bool hasUpper = true;
    bool hasLower = true;
    printf("enter password to check\n");
    while (getchar() != '\n');
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    int pass_len = strlen(password);
    bool cont_space = false;
    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] == ' ') {
            printf("password contains spaces(quit programm)\n");
            cont_space = true;
            break;
        }
        else {
            continue;
        }
    }
    if (cont_space) {
        return;
    }

    if (pass_len < 6) {
        printf("twin? your password is ahhhh/ i quit\n");
        return;
    }
    else if (pass_len > 6 && pass_len < 10) {
        score++;
    }
    else {
        score += 2;
    }

    {
        bool hasUpper = false;
        bool hasLower = false;
        bool hasSymb = false;
        for (int i = 0; password[i] != '\0'; i++) {
            if (isupper(password[i])) {
                hasUpper = true;
            }
            if (islower(password[i])) {
                hasLower = true;
            }
            if (ispunct(password[i])) {
                hasSymb = true;
            }
        }
        if (hasUpper && hasLower) {
            score += 2;
        }
        if (hasSymb) {
            score += 2;
        }
    }

    bool hasDigit = false;
    int i = 0;
    do {
        if (isdigit(password[i])) {
            hasDigit = true;
            i++;
        }
        else {
            i++;
        }
    } while (password[i] != '\0' && !hasDigit);
    if (hasDigit) {
        score += 2;
    }
    printf("score: %d\n", score);
    if (score >= 6) {
        printf("password is strong\n");
    }
    else if (score > 3 && score < 6) {
        printf("password is mediocre\n");
    }
    else {
        printf("password is weak\n");
    }

}

void run_editor() {
    int choice;
    int running = 1;
    while (running) {
        printf("\navaliable commands:\n");
        printf("1.check password\n");
        printf("2.Exit\n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number\n");
            while (getchar() != '\n') {
                continue;
            }
        }
        switch (choice) {
        case 1:PasswordChecker(); break;
        case 2:printf("Exiting\n");
            running = 0;
            break;
        default: printf("Function not implemented\n");
        }

    }

}
int main() {
    run_editor();
}