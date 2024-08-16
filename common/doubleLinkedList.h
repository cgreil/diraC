//
// Created by christoph on 11.08.24.
//

#ifndef LEIBNITZ_DOUBLELINKEDLIST_H
#define LEIBNITZ_DOUBLELINKEDLIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node_t{
    void *element;
    struct Node_t *next;
    struct Node_t *previous;
} Node;

typedef struct{
    Node *root;
    size_t numElements;
}DoubleLinkedList;

DoubleLinkedList dll_create();

void *dll_getElementAtIndex(DoubleLinkedList dll, size_t index);

void dll_insertElement(DoubleLinkedList dll, void *element, size_t elementSize);

bool dll_removeElementAtIndex(DoubleLinkedList dll, size_t index);





#endif //LEIBNITZ_DOUBLELINKEDLIST_H
