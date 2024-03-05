#include "lists.h"

// ----------------------------------------------------------------------------------------

// Adds particle to end of list
void push(Node *head, int val) {
    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = malloc(sizeof(Node));
    current->next->value = val;
    current->next->next = NULL;
}

// ----------------------------------------------------------------------------------------

// Removes last partilce of list
int removeLast(Node *head) {
    int val = 0;
    if (head->next == NULL) {
        val = head->value;
        free(head);
        return val;
    }
    Node *current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    val = current->next->value;
    free(current->next);
    current->next = NULL;
    return val;
}

// ----------------------------------------------------------------------------------------

// Resets list
void clearList(Node *head) {
    while (head->next != NULL) {
        removeLast(head);
    }
}

// ----------------------------------------------------------------------------------------

// Removes particle with specific value
int removeByValue(Node *head, int valR) {
    int val = 0;
    if (head->next == NULL) {
        if (head->value == valR) {
            val = head->value;
            free(head);
            return val;
        }
    }
    Node *previous = head;
    Node *current = head->next;
    while (current->next != NULL) {
        if (current->value == valR) {
            Node *tmp = current->next;
            val = current->value;
            previous->next = current->next;
            current->next = NULL;
            free(current);
            current = tmp;
        } else {
            previous = current;
            current = current->next;
        }
    }
    if (current->value == valR) {
        val = current->value;
        free(current);
        previous->next = NULL;
    }

    return val;
}

// ----------------------------------------------------------------------------------------

// Counts number of nodes of a list
int countListMembers(Node *head) {

    int counter = 0;
    Node *current = head->next;
    while (current != NULL) {
        counter++;
        current = current->next;
    }

    return counter;
}

// ----------------------------------------------------------------------------------------

// This function is not used in the program, however it can be used to check and
// debug the data in lists
void printList(Node *head) {

    printf("\n");
    Node *current = head->next;
    while (current != NULL) {
        printf("%d\t", current->value);
        current = current->next;
    }

    printf("\n");
}

// ----------------------------------------------------------------------------------------
