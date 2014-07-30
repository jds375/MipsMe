#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MipsMe.h"

#define LINE_SIZE 128
#define SPACE_ASCII 32
#define COLON_ASCII 58
#define CALL_SIZE 100
#define ARG1_SIZE 100
#define ARG2_SIZE 100
#define ARG3_SIZE 100
#define NUM_REGISTERS 32
#define STACK_SIZE 8000

#define V0 2
#define V1 3
#define A0 4
#define A1 5
#define A2 6
#define A3 7
#define T0 8
#define T1 9
#define T2 10
#define T3 11
#define T4 12
#define T5 13
#define T6 14
#define T7 15
#define S0 16
#define S1 17
#define S2 18
#define S3 19
#define S4 20
#define S5 21
#define S6 22
#define S7 23
#define T8 24
#define T9 25
#define GP 28
#define SP 29
#define FP 30
#define RA 31

/*
Creates a command type representing a mips command
with a command type and whatever its arguments
might be. Assumes command is 5 or less characters
and each argument is 100 or less characters.
*/
command* createCommand() {
  command *c = (command*)malloc(sizeof(command));
  c->call = (char*)malloc(sizeof(char)*CALL_SIZE);
  *(c->call) = '\0';
  c->arg1 = (char*)malloc(sizeof(char)*ARG1_SIZE);
  *(c->arg1) = '\0';
  c->arg2 = (char*)malloc(sizeof(char)*ARG2_SIZE);
  *(c->arg2) = '\0';
  c->arg3 = (char*)malloc(sizeof(char)*ARG3_SIZE);
  *(c->arg3) = '\0';
  c->text = (char*)malloc(sizeof(char)*CALL_SIZE);
  *(c->text) = '\0';
  return c;
}

/*
Creates a program to hold all commands
and current position.
*/
program* createProgram(int programSize) {
  program *p = (program*)malloc(sizeof(program));
  p->script = (command**)malloc(sizeof(command*)*programSize);
  p->sectionTable = (arraylist*)malloc(sizeof(arraylist*));
  p->sectionTable = createArraylist();
  p->linePos = 0;
  p->size = programSize;
  return p;
}

/*
Creates a cpu type holding all of the registers and
a program.
*/
cpu* createCpu(program* prog) {
  cpu *c = (cpu*)malloc(sizeof(cpu));
  int i;
  for (i = 0; i < NUM_REGISTERS; i++) {
    c->registers[i] = 0;
  }
  for (i = 0; i < STACK_SIZE; i++) {
    c->stack[i] = 0;
  }
  c->prog = (program*)malloc(sizeof(program));
  c->prog = prog;
  return c;
}

/*
Parses an argument to unsigned number format.
Assumes it's a correctly-formatted number. Only
works on argument 3.
*/
int parseNumberArg(char* numberArg) {
  return ((int)atoi(numberArg));
}

/*
Ensures that an argument representing a register
is parsed into the correct int value. Errors
on values not in range. Capable of parsing
register names in lowercase. Assumes
registers are correctly formatted letters
or numbers.
*/
int parseRegisterArg(char* registerArg) { 
  if (*(registerArg) == '$') {
    if (((*(registerArg+2) == ',') || (*(registerArg+2) == '\n')) && (*(registerArg+1)-'0' >= 0) && (*(registerArg+1)-'0' <= 9)) {
      return (*(registerArg+1)-'0');  
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\n')) && (((*(registerArg+1)-'0')*10 + (*(registerArg+2)-'0')) >= 0) && (((*(registerArg+1)-'0')*10 + (*(registerArg+2)-'0')) <= 32)) {
      return ((*(registerArg+1)-'0')*10 + (*(registerArg+2)-'0'));
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 'v') && (*(registerArg+2) == '0')) { 
      return V0;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 'v') && (*(registerArg+2) == '1')) {
      return V1;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '0')) {
      return T0;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '1')) {
      return T1;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '2')) {
      return T2;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '3')) {
      return T3;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '4')) {
      return T4;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '5')) {
      return T5;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '6')) {
      return T6;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '7')) {
      return T7;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '0')) {
      return S0;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '1')) {
      return S1;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '2')) {
      return S2;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '3')) {
      return S3;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '4')) {
      return S4;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '5')) {
      return S5;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '6')) {
      return S6;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == '7')) {
      return S7;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '8')) {
      return T8;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 't') && (*(registerArg+2) == '9')) {
      return T9;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 'g') && (*(registerArg+2) == 'p')) {
      return GP;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 's') && (*(registerArg+2) == 'p')) {
      return SP;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 'f') && (*(registerArg+2) == 'p')) {
      return FP;
    } else if (((*(registerArg+3) == ',') || (*(registerArg+3) == '\0')) && (*(registerArg+1) == 'r') && (*(registerArg+2) == 'a')) {
      return RA;
    } else if (*(registerArg+2) == '\n') {
      return ((*(registerArg+1)-'0'));
    } else if (*(registerArg+3) == '\n') {
      return ((*(registerArg+1)-'0')*10 + (*(registerArg+2)-'0'));
    } else {
      perror("Bad register address.\n"); 
    }
  }
  return -1;
}

/*
Removes a newline from the end of a string
if there is one.
*/
void cutNewline(char* c) {
  if (c[strlen(c)-1] == '\n') {
    c[strlen(c)-1] = '\0';
  }
}
/*
Removes a comma from the end of a string
if there is one.
*/
void cutComma(char* c) {
  if (c[strlen(c)-1] == ',') {
    c[strlen(c)-1] = '\0';
  }
}
/*
Executes the command corresponding to the line the
current cpu is on.
*/
void executeCurCommand(cpu *cp) {
  command *co = cp->prog->script[cp->prog->linePos];
  int rd;
  int rs;
  int rt;
  int jump = -1;
  int isJr = 1;
  if (co->header) {
    cp->prog->linePos++;
    return;
  }
  printf("Command: %s",co->text);
  if (!strncmp(co->call, "add", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] + cp->registers[rt];
  } else if (!strncmp(co->call, "sub", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] - cp->registers[rt];
  } else if (!strncmp(co->call, "or", CALL_SIZE)) { 
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] | cp->registers[rt];
  } else if (!strncmp(co->call,"and", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] =cp->registers[rs] & cp->registers[rt];
  } else if (!strncmp(co->call,"xor", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] =cp->registers[rs] ^ cp->registers[rt];
  } else if (!strncmp(co->call,"nor", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = ~(cp->registers[rs] | cp->registers[rt]);
  } else if (!strncmp(co->call,"sllv", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] << cp->registers[rt];
  } else if (!strncmp(co->call,"srlv", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] >> cp->registers[rt];
  } else if (!strncmp(co->call,"slt", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] < cp->registers[rt];
  } else if (!strncmp(co->call,"slt", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] < cp->registers[rt];
  } else if (!strncmp(co->call,"addi", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseNumberArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] + rt;
  } else if (!strncmp(co->call,"andi", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseNumberArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] & rt;
  } else if (!strncmp(co->call,"ori", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseNumberArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] | rt;
  } else if (!strncmp(co->call,"xori", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseNumberArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] ^ rt;
  } else if (!strncmp(co->call,"nori", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseNumberArg(co->arg3);
    cp->registers[rd] = ~(cp->registers[rs] | rt);
  } else if (!strncmp(co->call,"sll", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rt = parseRegisterArg(co->arg2);
    rs = parseNumberArg(co->arg3);
    cp->registers[rd] = cp->registers[rt] << rs;
  } else if (!strncmp(co->call,"srl", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rt = parseRegisterArg(co->arg2);
    rs = parseNumberArg(co->arg3);
    cp->registers[rd] = cp->registers[rt] >> rs;
  } else if (!strncmp(co->call,"sltiu", CALL_SIZE)) {
    rd = parseRegisterArg(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseNumberArg(co->arg3);
    cp->registers[rd] = cp->registers[rs] < rt;
  } else if (!strncmp(co->call,"j", CALL_SIZE)) {
    cutNewline(co->arg1);
    jump = arraylistFind(cp->prog->sectionTable,co->arg1);
  } else if (!strncmp(co->call,"jr", CALL_SIZE)) {
    cutNewline(co->arg1);
    rd = parseRegisterArg(co->arg1);
    jump = cp->registers[rd];
    isJr = 0;
  } else if (!strncmp(co->call,"jal",CALL_SIZE)) {
    cutNewline(co->arg1);
    cp->registers[RA] = cp->prog->linePos+1;
    jump = arraylistFind(cp->prog->sectionTable,co->arg1);
  } else if (!strncmp(co->call,"beq",CALL_SIZE)) {
    cutComma(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    if (cp->registers[rs] == cp->registers[rt]) {
      jump = arraylistFind(cp->prog->sectionTable,co->arg1);
    }
  } else if (!strncmp(co->call,"bne",CALL_SIZE)) {
    cutComma(co->arg1);
    rs = parseRegisterArg(co->arg2);
    rt = parseRegisterArg(co->arg3);
    if (cp->registers[rs] != cp->registers[rt]) {
      jump = arraylistFind(cp->prog->sectionTable,co->arg1);
    }
  } else if (!strncmp(co->call,"lb",CALL_SIZE)) {
    co->arg2[strlen(co->arg2)-1] = ',';
    rs = parseRegisterArg(co->arg1);
    rt = parseRegisterArg(co->arg2);
    cp->registers[rs] = cp->stack[cp->registers[rt]];
  } else if (!strncmp(co->call,"sb",CALL_SIZE)) {
    co->arg2[strlen(co->arg2)-1] = ',';
    rs = parseRegisterArg(co->arg1);
    rt = parseRegisterArg(co->arg2);
    cp->stack[cp->registers[rt]] = cp->registers[rs];
  }
  if (jump >= 0) {
    cp->prog->linePos = jump+isJr;
  } else {
    cp->prog->linePos++;
  }
}

/*
Takes a line from the mips file and parses
its instructions into a command object. A header
is simply the call alone.
*/
command* parseCommand(char *instruction) {
  command *c = createCommand();
  strcpy(c->text, instruction);
  int previ = 0;
  int i;
  if ((*(instruction+strlen(instruction)-2)) == COLON_ASCII) {
    strncpy(c->call, instruction, strlen(instruction)-2);
    c->header = 1;
  } else {
    for (i = 0; i <= strlen(instruction); i++) {
      if ((*(instruction+i) == SPACE_ASCII) || (*(instruction+i) == '\0')) {
        if (*(c->call) == '\0') {
          strncpy(c->call, instruction+previ, i-previ);
          *(c->call+i-previ) = '\0';
          previ = i;
        } else if (*(c->arg1) == '\0') {
          strncpy(c->arg1, instruction+previ+1, i-previ);
          *(c->arg1+i-previ-1) = '\0';
	  previ = i;
        } else if (*(c->arg2) == '\0') {
	  strncpy(c->arg2, instruction+previ+1, i-previ);
          *(c->arg2+i-previ-1) = '\0';
	  previ = i;
        } else if (*(c->arg3) == '\0') {
	  strncpy(c->arg3, instruction+previ+1, i-previ);
	  *(c->arg3+i-previ-1) = '\0';
	  previ = i;
        }
      }
    }
    c->header = 0;
  }
  return c;  
}

/*
Counts the number of lines in a file.
*/
int getNumLines(const char *filename) {
  FILE *file = fopen(filename,"r");
  char line [LINE_SIZE];
  int numLines = 0;
  if (file != NULL) {
    while (fgets(line, sizeof(line), file) != NULL ) {
      numLines++;
    }
    fclose(file);
  } else {
    perror("Bad filename.\n");
  }
  return numLines;
}

/*
Loads a mips cpu given a program file.
*/
cpu* loadCpu(const char *filename) {
  FILE *file = fopen(filename,"r");
  char line [LINE_SIZE];
  int numLines = getNumLines(filename);
  int i = 0;
  program* p = createProgram(numLines);
  cpu* cp = createCpu(p);
  if (file != NULL) {
    while (fgets(line, sizeof(line), file) != NULL ) {
      cp->prog->script[i] = parseCommand(line);
      if (cp->prog->script[i]->header) {
	tuple* t = createTuple();
	t->key = cp->prog->script[i]->call;
	t->value = i;
	arraylistAdd(p->sectionTable, t);
      }
      i++;
    }
    fclose(file);
  } else {
    perror("Bad filename.\n");
  }  
  return cp;
}

/*
Executes a loaded cpu
*/
void runCpu(cpu* cp) {
  int i;
  while (cp->prog->linePos != cp->prog->size) {
    executeCurCommand(cp);
  }  
}

/*
Has the cpu execute the given number
of steps
*/
void stepCpu(cpu* cp, int steps) {
  for (int i = 0; (i < steps) && (cp->prog->linePos != cp->prog->size); i++) {
    if (cp->prog->script[cp->prog->linePos]->header) {
      steps++;
    }
      executeCurCommand(cp);
  }
}

/*
prints contents of registers
*/
void displayRegisters(cpu *cp) {
  int i;
  printf("Register Contents:\n");
  for (i = 0; i < NUM_REGISTERS; i++) {
    printf("$%d = %d\n", i, cp->registers[i]);
  }
}

int main(int argc, char *argv[]) {
  if (argc == 2) {
    cpu* cp = loadCpu(argv[1]);
    int go = 1;
    while (go) {
      int input = 0;
      int i = 0;
      char* entry = (char*)malloc(sizeof(char)*CALL_SIZE);
      printf("\n> ");
      while (input != '\n') {
	input = getchar();
        entry[i] = input;
	i++;
      }
      if (!strcmp(entry,"run\n")) {
	runCpu(cp);
	printf("Program Completed! Dumping Registers:\n");
	displayRegisters(cp);
	go = 0;
      } else if (!strncmp(entry, "step ", 5)) {
	int steps = 0;
	int j;
	int p = 1;
	for (j = strlen(entry)-2; entry[j] != ' '; j--) {
	  steps = (entry[j]-48)*p+steps;
	  p = p*10;
	}
	stepCpu(cp, steps);
	displayRegisters(cp);
        if (cp->prog->linePos == cp->prog->size) {
          go = 0;
          printf("Program Completed! Terminating.\n");
        }
      } else if (!strncmp(entry,"display stack ", 13)) {
	int index = 0;
        int j;
	int p = 1;
        for (j = strlen(entry)-2; entry[j] != ' '; j--) {
          index = (entry[j]-48)*p+index;
          p = p*10;
        }
	printf("Stack[%d] = %d\n", index, cp->stack[index]);
      }
      free(entry);
    }
  } else {
    perror("Please enter a single file name.\n");
  }  
  return 0;
}
