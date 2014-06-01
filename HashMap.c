#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HashMap.h"

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

hashmap* createHashmap() {
  hashmap* h = (hashmap*)malloc(sizeof(hashmap));
  h->map = (arraylist**)malloc(2*sizeof(arraylist*));
  h->map[0] = createArraylist();
  h->map[1] = createArraylist();
  h->size = 0;
  h->buckets = 2;
  return h;
}

/*
djb2 hash
*/
int hashCode(char *key, int n) {
  unsigned long hash = 5381;
  int c;
  for (int i = 0; i < n; i++) {
    c = key[i];
    hash = hash * 33 + c;
  }
  return (int)hash;
}

/*
Puts a key-value pair in the table. Assumes no repeats.
Assumes all values are non-negative.
*/
void hashmapPut(hashmap* h, char* k, int v) {
  tuple* t = createTuple();
  t->key = k;
  t->value = v;
  h->size++;
  if (((float)h->size/(float)h->buckets) > LOAD_FACTOR) {
    h->map = realloc(h->map, 2*h->buckets*sizeof(arraylist**));
    h->buckets = 2*h->buckets;
    int i;
    for (i = h->buckets/2; i < h->buckets; i++) {
      h->map[i] = createArraylist();
    }
  }
  int hash = hashCode(t->key, strlen(t->key)) % h->buckets;
  arraylistAdd(h->map[hash], t);
}

/*
Returns the corresponding value. If not in the table,
returns -1.
*/
int hashmapGet(hashmap* h, char* key) {
  int hash = hashCode(key, strlen(key)) % h->buckets;
  if (h->map[hash]->length > 0) {
    int i;
    for (i = 0; i < h->map[hash]->length; i++) {
      if (!strcmp(key, arraylistGet(h->map[hash], i)->key)) {
	return arraylistGet(h->map[hash], i)->value;
      }
    }
    return -1;
  } else {
    return -1;
  }
}

/*int main(int argc, char *argv[]) {
  return 0;
}*/
