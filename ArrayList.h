typedef struct tuple {
  char* key;
  int value;
} tuple;

typedef struct arraylist {
  tuple** list;
  int length;
  int size;
} arraylist;

tuple* createTuple();

arraylist* createArraylist();

void arraylistAdd(arraylist* a, tuple* t);

tuple* arraylistGet(arraylist* a, int index);

int arraylistFind(arraylist* a, char* key);
