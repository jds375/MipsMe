#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ArrayList.h"

#define LOAD_FACTOR 0.75

tuple* createTuple() {
  tuple* t = (tuple*)malloc(sizeof(tuple));
  t->key = (char*)malloc(sizeof(char));
  return t;
}

void freeTuple(tuple* t) {
  free(t->key);
  free(t);
}

/*
Creates an arraylist of size 1 that is initially empty.
*/
arraylist* createArraylist() {
  arraylist* a = (arraylist*)malloc(sizeof(arraylist));
  a->list = (tuple**)malloc(sizeof(tuple*));
  a->length = 0;
  a->size = 1;
  return a;
}

/*
Frees an arraylist, but not the elements inside the arraylist.
*/
void freeArraylist(arraylist* a) {
  free(a->list);
  free(a);
}

/*
Appends the tuple to the arraylist. Resizes to 2x size when full.
*/
void arraylistAdd(arraylist* a, tuple* t) {
  if (a->length == a->size) {
    a->list = realloc(a->list, 2*a->size*sizeof(tuple**));
    a->size = 2*a->size;
  }
  a->list[a->length] = t;
  a->length++;
}

/*
Gets the tuple at the specified index. Assumes the
index is in bounds.
*/
tuple* arraylistGet(arraylist* a, int index) {
  return a->list[index];
}

/*
Removes an element at a given index from the arraylist.
Assumes the index is in bounds. Returns the tuple
at that position and sets the element there to null.
*/
tuple* arraylistRemove(arraylist* a, int index) {
  tuple* removed = a->list[index];
  int i;
  for (i = index; i < a->length; i++) {
    if (i == a->length-1) {
      a->list[i] = NULL;
    } else {
      a->list[i] = a->list[i+1];
    }
  }
  return removed;
}

/*
Returns the value associated with the key of the tuples
in the arraylist. Returns -1 if fails.
*/

int arraylistFind(arraylist* a, char* search) {
  int i;
  for (i = 0; i < a->length; i++) {
    if (!strcmp(a->list[i]->key, search)) {
      return a->list[i]->value;
    } 
  }
  return -1;
}
