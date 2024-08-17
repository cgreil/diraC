//
// Created by christoph on 11.08.24.
//

#include <string.h>
#include <assert.h>
#include "doubleLinkedList.h"

// FORWARD DECLARATION
static Node* dll_getLastNode(DoubleLinkedList dll);


DoubleLinkedList dll_create(){
    DoubleLinkedList list;

    list.root = NULL;
    list.numElements = 0;

    return list;
}

void *dll_getElementAtIndex(DoubleLinkedList dll, size_t index) {

    if (dll.numElements == 0 || index >= dll.numElements) {
       return NULL;
    }

    size_t currIndex = 0;
    Node *currentNode = dll.root;

    while(currIndex <  index) {
        currentNode = currentNode->next;
        currIndex++;
    }

    assert(currentNode != NULL);

    // TODO: Should return a complex_clone of the element
    return currentNode->element;
}

void dll_insertElement(DoubleLinkedList dll, void *element, size_t elementSize) {

    /**
     * Function inserting a element passed by a corresponding void ptr
     * of the size elementSize into the list
     *
     * The element will be copied in the process in order to not
     * demand the element to be valid throughout the entire dll_lifetime
     */

    // create Node instances
    Node *newNode = malloc(sizeof(Node));

    // complex_clone element into the node
    void *elementCopy = malloc(elementSize);
    memcpy(elementCopy, element, elementSize);
    newNode->element = elementCopy;

    if (dll.numElements == 0) {
        dll.root = newNode;
        newNode->next = NULL;
    } else {
        Node *lastNode = dll_getLastNode(dll);
        assert(lastNode->next == NULL);
        newNode->previous = lastNode;
        lastNode->next = newNode;
    }

    newNode->previous = NULL;
    dll.numElements++;
}

bool dll_removeElementAtIndex(DoubleLinkedList dll, size_t index) {

    /**
     * Remove the element at the given (zero-based) index
     * @returns true if an element could be removed at the given index
     * and false otherwise
     */


    if (dll.numElements <= index) {
        return false;
    }

    Node *nodeToDelete;
    if (index == 0){
        nodeToDelete = dll.root;
        //increment dll root
        dll.root = dll.root->next;
        dll.numElements--;
        goto cleanupNode;
    }

    Node *element = dll_getElementAtIndex(dll, index);

    if (element == NULL) {
        return false;
    }

    Node *previousElement = element->previous;
    Node *nextElement = element->next;

    previousElement->next = nextElement;
    if (nextElement != NULL) {
        nextElement->previous = previousElement;
    }
    nodeToDelete = element;
    dll.numElements--;

    cleanupNode:
    free(nodeToDelete->element);
    free(nodeToDelete);
    memset(nodeToDelete, 0, sizeof(Node));

    return true;

}


static Node* dll_getLastNode(DoubleLinkedList dll) {

    // no elements are contained return NULL pointer
    if (dll.numElements == 0) {
        return NULL;
    }

    Node *current = dll.root;

    // return last node of the list
    while(current->next != NULL) {
        current = current->next;
    }

    return current;
}



