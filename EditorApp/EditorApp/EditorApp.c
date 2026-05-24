#include <stdio.h>
#include <stdlib.h>
struct Node {
    int value;
    struct Node* next;
};
struct Node* create(int value) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}
void push_back(struct Node** head, int value) {
    struct Node* extra = malloc(sizeof(struct Node));
    extra->value = value;
    extra->next = NULL;
    struct Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = extra;
}
void push_front(struct Node** head, int value) {
    struct Node* extra = malloc(sizeof(struct Node));
    extra->value = value;
    extra->next = *head;
    *head = extra;
}
void print(struct Node** head) {
    struct Node* current = *head;
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}
void destroy(struct Node** head) {
    struct Node* current = *head;
    while (current != NULL) {
        struct Node* next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
void reverse(struct Node** head) {
    struct Node* prev = NULL;
    struct Node* current = *head;
    struct Node* next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}
int main() {
    struct Node* head = create(1);
    push_back(&head, 2);
    push_back(&head, 3);
    push_back(&head, 4);
    push_front(&head, 0);
    printf("before reverse\n");
    print(&head);
    reverse(&head);
    printf("after reverse\n");
    print(&head);
    destroy(&head);

    return 0;
}