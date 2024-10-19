//
// Created by christoph on 11.08.24.
//

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "doubleLinkedList.h"

// FORWARD DECLARATION
static Node* dll_getLastNode(DoubleLinkedList *dll);


bool dll_free(DoubleLinkedList *dll) {

    size_t index = 0;
    while (dll->numElements != 0) {
        dll_removeElementAtIndex(dll, index);
    }

    if (dll->root != NULL) {
        free(dll->root);
    }

    memset(dll, 0, sizeof(DoubleLinkedList));
    return true;
}

Node* dll_getNodeAtIndex(DoubleLinkedList* dll, size_t index) {
    if (dll->numElements == 0 || index >= dll->numElements) {
        return NULL;
    }

    size_t currIndex = 0;
    Node* currentNode = *(dll->root);

    while(currIndex <  index) {
        currentNode = currentNode->next;
        currIndex++;
    }

    assert(currentNode != NULL);

    return currentNode;
}

void* dll_getElementAtIndex(DoubleLinkedList* dll, size_t index) {

    Node* node = dll_getNodeAtIndex(dll, index);

    return node->element;
}

void dll_insertElement(DoubleLinkedList* dll, void *element, size_t elementSize) {

    /**
     * Function inserting a element passed by a corresponding void ptr
     * of the size elementSize into the list
     *
     * The element will be copied in the process in order to not
     * demand the element to be valid throughout the entire dll_lifetime
     */
     assert(dll != NULL);
     assert(element != NULL);
     if (elementSize == 0) {
         fprintf(stderr, "Cannot insert element of size 0 into double linked list. Returning ... \n");
         return;
     }

    if (dll->root == NULL) {
        // allocate heap memory for ptr2ptr to node:
        dll->root = malloc(sizeof(Node *));
        // store return value of malloc in heap, since ptr has to
        // stay valid even after return of funtion
        *(dll->root) = malloc(sizeof(Node));
    }

    assert(dll->root != NULL);
    assert(*(dll->root) != NULL);

    // complex_clone element into the node
    void *elementCopy = malloc(elementSize);
    memcpy(elementCopy, element, elementSize);
    // put element reference into node, set initial values
    // to previous and next ptr
    Node* nodeToInsert = malloc(sizeof(Node));

    nodeToInsert->element = elementCopy;
    nodeToInsert->next = NULL;
    nodeToInsert->previous = NULL;

    if (dll->numElements == 0) {
        *(dll->root) = nodeToInsert;
    } else {
        Node *lastNode = dll_getLastNode(dll);
        assert(lastNode->next == NULL);
         nodeToInsert->previous = lastNode;
        lastNode->next = nodeToInsert;
    }
    dll->numElements++;
}

bool dll_removeElementAtIndex(DoubleLinkedList* dll, size_t index) {

    /**
     * Remove the element at the given (zero-based) index
     * @returns true if an element could be removed at the given index
     * and false otherwise
     */
    if (dll == NULL || dll->root == NULL || dll->numElements == 0){
        return false;
    }
    else if (dll->numElements <= index) {
        return false;
    }

    Node *nodeToDelete;
    if (index == 0){
        nodeToDelete = *(dll->root);
        //increment dll root
        *(dll->root) = (*(dll->root))->next;
        dll->numElements--;
        goto cleanupNode;
    }

    Node* removalCandidate = dll_getNodeAtIndex(dll, index);

    if (removalCandidate == NULL) {
        return false;
    }

    Node *previousNode = removalCandidate->previous;
    Node *nextNode = removalCandidate->next;

    if (previousNode != NULL) {
        previousNode->next = nextNode;
    }
    if (nextNode != NULL) {
        nextNode->previous = previousNode;
    }
    nodeToDelete = removalCandidate;
    dll->numElements--;

    cleanupNode:
    free(nodeToDelete->element);
    free(nodeToDelete);
    memset(nodeToDelete, 0, sizeof(Node));

    return true;
}


static Node* dll_getLastNode(DoubleLinkedList* dll) {

    // no elements are contained return NULL pointer
    if (dll->numElements == 0) {
        return NULL;
    }

    Node *current = (*(dll->root));

    // return last node of the list
    while(current->next != NULL) {
        current = current->next;
    }

    return current;
}



