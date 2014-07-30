#include "ArrayList.c"

#define NUM_REGISTERS 32
#define STACK_SIZE 8000

typedef struct command {
  char* call;
  char* arg1;
  char* arg2;
  char* arg3;
  char* text;
  int header;
} command;

typedef struct program {
  command** script;
  arraylist* sectionTable;
  int linePos;
  int size;
} program;

typedef struct cpu {
  int registers[NUM_REGISTERS];
  int stack[STACK_SIZE];
  program* prog;
} cpu;
