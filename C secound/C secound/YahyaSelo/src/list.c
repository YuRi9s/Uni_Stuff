#include "list.h"

#include <assert.h>
#include <stdlib.h>

#include "err.h"
#include "util.h"

/**
 * Struct for encapsulating a single list element.
 */
typedef struct ListItem {
    struct ListItem* next;  // pointer to the next element (NULL if last)
    void* data;             // pointer to the data
} ListItem;

List mkList(void) {
    List res;
    res.head = NULL;
    return res;
}

void clearList(List* s) {
    // assign the head of the list to a temporary variable
    ListItem* current = s->head;
    while (current != NULL) {
        // if there are still nodes in the list
        // assign the next node of the current one to a temporary variable
        ListItem* temp = current->next;
        // free the current node
        free(current);
        // update the current variable
        current = temp;
    }
    s->head = NULL;
}

void push(List* s, void* data) {
    // allocate memory for a new list item
    ListItem* item = (ListItem*)malloc(sizeof(ListItem));
    if (item == NULL) {
        // if the memory allocation was not successful print failed
        printf("Failed to allocate memory for list item.\n");
        // to indicate a failure.
        exit(1);
    }
    // assign data parameter passed to the function to data
    item->data = data;
    // The next field of the new item is set to the current head of the list
    item->next = s->head;
    // the head of the list is updated to point to the new item,
    s->head = item;
}

void* peek(List* s) {
    // check the list if its empty by verifying if the head pointer
    if (s->head == NULL) {
        return NULL;
    }
    // returns the data field
    return s->head->data;
}

void pop(List* s) {
    // check the list if its empty by verifying if the head pointer
    if (s->head == NULL) {
        return;
    }
    /*
     * 1) creates a temporary pointer
     * 2) assign the value of the head pointer.
     * 3) update pointer of the list to point to the next item in the list
     */
    ListItem* temp = s->head;
    s->head = s->head->next;
    free(temp);
}

char isEmpty(List* s) { return s->head == NULL; }

ListIterator mkIterator(List* list) {
    ListIterator res;
    res.list = list;
    res.prev = NULL;
    res.current = list->head;

    return res;
}

void* getCurr(ListIterator* it) {
    assert(it->current != NULL);
    return it->current->data;
}

void next(ListIterator* it) {
    assert(isValid(it));
    it->prev = it->current;
    it->current = it->current->next;
}

char isValid(ListIterator* it) { return it->current != NULL; }
