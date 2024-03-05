#ifndef LISTS_H
#define LISTS_H

#include <stdio.h>
#include <stdlib.h>

// List elements for Verlet-lists
struct node {
    int value;
    struct node *next;
};
typedef struct node Node;

void push(Node *head, int val);
int removeLast(Node *head);
void clearList(Node *head);
int removeByValue(Node *head, int valR);
int countListMembers(Node *head);
void printList(Node *head);

#endif
