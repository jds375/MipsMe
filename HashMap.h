typedef struct tuple {
  char* key;
  int value;
} tuple;

typedef struct arraylist {
  tuple** list;
  int length;
  int size;
} arraylist;

typedef struct hashmap {
  arraylist** map;
  int size;
  int buckets;
} hashmap;

hashmap* createHashmap();

void hashmapPut(hashmap* h, char* k, int v);

int hashmapGet(hashmap* h, char* key);
