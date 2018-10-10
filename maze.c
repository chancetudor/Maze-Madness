//
//  maze.c
//  project1
//  This file contains necessary code for the maze
//
//  Created by Chance Tudor on 10/2/18.
//

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "maze.h"
#include "cell.h"
#include "stack.h"
#include "queue.h"
#include "da.h"

extern void srandom(unsigned int);
extern long int random(void);
static void printName(void);
static void setInFile(MAZE * m, char * iFile);
static void setOutFile(MAZE * m, char * oFile);
static void setBuild(MAZE * m);
static void setSolve(MAZE * m);
static void setDraw(MAZE * m);
static void createNeighbors(MAZE * m);

struct cell {
  int row;
  int column;
  bool rightWall;
  bool bottomWall;
  int value;
  int nCount;
  CELL * neighbors[4];
};

struct maze {
  CELL * (**matrix);
  int rows;
  int columns;
  unsigned int seed;
  char * inFile;
  char * outFile;
  int numDashes;
  int numBars;
  bool build;
  bool solve;
  bool draw;
};

extern MAZE * newMAZE(void) {
  MAZE * m = malloc(sizeof(MAZE)); // FIXME: free!
  assert(m != 0);
  m->rows = 0;
  m->columns = 0;
  m->seed = 1;
  m->inFile = 0;
  m->outFile = 0;
  m->matrix = 0;
  m->numDashes = 0;
  m->numBars = 0;
  m->build = false;
  m->solve = false;
  m->draw = false;

  return m;
}

extern void freeMAZE(MAZE * m) {
  for (int i = 0; i < getMAZERows(m); ++i) {
   for (int j = 0; j < getMAZEColumns(m); ++j) {
     freeCELL(m->matrix[i][j]);
   }
 }

 free(m);
}

static void Fatal(char *fmt, ...) {
  va_list ap;

  fprintf(stderr,"An error occured: ");
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  exit(-1);
}

static int ProcessOptions(MAZE * m, int argc, char **argv) {
  int start = 0;
  int argIndex = 1;
  int argsUsed = 0;
  char *arg = 0;
  char *iFile = 0; // create char array to hold name of input file
  char *oFile = 0; // create char array to hold name of output file
  int rows = 0;
  int columns = 0;
  unsigned int seed = 0;

  while (argIndex < argc && *argv[argIndex] == '-') {
  /* check if stdin, represented by "-" is an argument */
  /* if so, the end of options has been reached */
    if (argv[argIndex][1] == '\0') { return argIndex; }

    argsUsed = 0;
    start = argIndex;

    /* advance argIndex to point to the first argument to the option */
    if (argv[start][2] == '\0') {
      arg = argv[start + 1];
      ++argIndex;
    }
    else {
      /* first arg is connected to option, so don't advance */
      arg = argv[start] + 2;
    }

    switch (argv[start][1]) {
      /*
      * when option has an argument, do this
      *
      *     examples are -m4096 or -m 4096
      *
      *     case 'm':
      *         MemorySize = atol(arg);
      *         argsUsed = 1;
      *         break;
      * ///////////////////////////////////////////////
      * when option has multiple arguments, do this
      *
      *     examples are -r4096 1280 or -r 4096 1280
      *
      *     case 'r':
      *         Rows = atoi(arg);
      *         Cols = atoi(argv[argIndex+1]);
      *         argsUsed = 2;
      *         break;
      * ///////////////////////////////////////////////
      * when option does not have an argument, do this
      *
      *     example is -a
      *
      *     case 'a':
      *         PrintActions = 1;
      *         break;
      */
      case 'v': // prints name, then exits
        printName();
        exit(0);
      case 'r': // seeds a pseudo-random number generator
        seed = atoi(arg);
        setMAZESeed(m, seed);
        argsUsed = 1;
        break;
      case 'c': // creates new maze
        rows = atoi(arg);
        columns = atoi(argv[argIndex + 1]);
        oFile = argv[argIndex + 2];
        setMAZESize(m, rows, columns);
        setOutFile(m, oFile);
        createMatrix(m);
        createNeighbors(m);
        setBuild(m);
        argsUsed = 3;
        break;
      /*case 's': // solve the maze in file III placing solution in file OOO
        setSolve(m);
      */
      case 'd': // draw the created/solved maze in file III
        // number of '----' = (columns * 4) + 1
        setMAZEDashes(m, (getMAZEColumns(m) * 4) + 1);
        // number of '|' = # of columns given
        setMAZEBars(m, getMAZERows(m));
        setDraw(m);
        iFile = arg;
        setInFile(m, iFile);
        argsUsed = 1;
        break;
      default:
        fprintf(stderr, "option %s not understood\n", argv[start]);
        exit(-1);
    }

    argIndex += argsUsed;
  }

  return argIndex;
}

static void printName(void) { printf("Written by Chance Tudor\n"); }

extern void setMAZESize(MAZE * m, int r, int c) {
  m->rows = r;
  m->columns = c;
}

extern void setMAZESeed(MAZE * m, int s) { m->seed = s; }

static void setOutFile(MAZE * m, char * file) {
  m->outFile = file;
}

static void setInFile(MAZE * m, char * file) { m->inFile = file; }

static void setBuild(MAZE * m) { m->build = true; }

static void setSolve(MAZE * m) { m->solve = true; }

static void setDraw(MAZE * m) { m->draw = true; }

extern void setMAZEDashes(MAZE * m, int d) { m->numDashes = d; }

extern void setMAZEBars(MAZE * m, int b) { m->numBars = b; }

extern int getMAZEDashes(MAZE * m) { return m->numDashes; }

extern int getMAZEBars(MAZE * m) { return m->numBars; }

extern int getMAZERows(MAZE * m) { return m->rows; }

extern int getMAZEColumns(MAZE * m) { return m->columns; }

extern void createMatrix(MAZE * m) {
  // create a one-dimensional array of row pointers
  m->matrix = malloc(sizeof(CELL **) * getMAZERows(m)); // FIXME: free!
  // make all the rows
  for (int i = 0; i < getMAZERows(m); ++i) {
   // create a single row
   m->matrix[i] = malloc(sizeof(CELL *) * getMAZEColumns(m)); // FIXME: free!
   // initialize the slots in the row
   for (int j = 0; j < getMAZEColumns(m); ++j) {
     m->matrix[i][j] = newCELL(); // FIXME: free!
     // set row and column vals for each new cell
     CELL * ptr = m->matrix[i][j];
     setCELLLocation(ptr, i, j);
     printf("i = %d || j = %d\n", i, j);
     printf("curr cell row = %d || curr cell col = %d\n", getRow(ptr), getColumn(ptr));
   }
 }
}

static void createNeighbors(MAZE * m) {
  for (int i = 0; i < getMAZERows(m); i++) {
    for (int j = 0; j < getMAZEColumns(m); j++) {
      CELL * ptr = m->matrix[i][j];
      // ptr, top, left, right, bottom
      if (i == 0 && j == 0) { // start of maze
        setCELLNeighbors(ptr, 0, 0, m->matrix[i+1][j], m->matrix[i][j+1]);
      }
      else if ((i == getMAZERows(m) - 1) && j == 0) { // far right of first row
        setCELLNeighbors(ptr, 0, m->matrix[i-1][j], 0, m->matrix[i][j+1]);
      }
      else if (i == getMAZERows(m) - 1) { // far right of any row
        setCELLNeighbors(ptr, m->matrix[i][j-1], m->matrix[i-1][j], 0, m->matrix[i][j+1]);
      }
      else if (i == 0 && (j == getMAZEColumns(m) - 1)) { // bottom of first col.
        setCELLNeighbors(ptr, m->matrix[i][j-1], 0, m->matrix[i+1][j], 0);
      }
      else if (i == 0) {
        setCELLNeighbors(ptr, m->matrix[i][j-1], 0, m->matrix[i+1][j], m->matrix[i][j+1]);
      }
      else if (j == getMAZEColumns(m) - 1) { // bottom of any col.
        setCELLNeighbors(ptr, m->matrix[i][j-1], m->matrix[i-1][j], m->matrix[i+1][j], 0);
      }
      else if (j == 0) {
        setCELLNeighbors(ptr, 0, m->matrix[i-1][j], m->matrix[i+1][j], m->matrix[i][j+1]);
      }
      else if ((i == getMAZERows(m) - 1) && (j == getMAZEColumns(m) - 1)) { // end of maze
        setCELLNeighbors(ptr, m->matrix[i][j-1], m->matrix[i-1][j], 0, 0);
      }
      else { // middle of array
        printf("i = %d\n", i);
        printf("j = %d\n", j);
        setCELLNeighbors(ptr, m->matrix[i][j-1], m->matrix[i-1][j], m->matrix[i+1][j], m->matrix[i][j+1]);
      }
    }
  }
}

extern CELL * findNeighbor(CELL * input) {
  printf("count = %d\n", input->nCount);
  unsigned int index = random() % input->nCount;
  CELL * val = getCELLNeighbors(input, index);
  return val;
}

extern void buildMAZE(MAZE * m) {
  STACK * s = newSTACK(); // FIXME: free!
  srand(m->seed);
  CELL * curr = m->matrix[0][0];
  push(s, curr);

  while (sizeSTACK(s) != 0) {
    curr = peekSTACK(s);
    CELL * neighbor = findNeighbor(curr);
    if (neighbor == 0) {
      pop(s);
    }
    else {
      // if path went up
      if (getColumn(neighbor) < getColumn(curr)) {
        setBottom(neighbor, false);
      }
      // if path went down
      if (getColumn(neighbor) > getColumn(curr)) {
        setBottom(curr, false);
      }
      // if path went left
      if (getRow(neighbor) < getRow(curr)) {
        setRight(neighbor, false);
      }
      // if path went right
      if (getRow(neighbor) > getRow(curr)) {
        setRight(curr, false);
      }
    }
    push(s, neighbor);
  }

  freeSTACK(s);
}

extern void drawMAZE(MAZE * m) {
  FILE * outFile;
  char * oFile = m->outFile;
  outFile = fopen(oFile, "a");
  for (int i = 0; i < getMAZERows(m); i++) {
    for (int j = 0; j < getMAZEColumns(m); j++) {
      printf("%d ", getValue(m->matrix[i][j]));
    }
    printf("\n");
  }
  fclose(outFile);
}

extern void pushMAZE(MAZE * m) {
  FILE * outFile;
  char * oFile = m->outFile;
  outFile = fopen(oFile, "a");


  fclose(outFile);
}


int main(int argc, char **argv) {
  MAZE * m = newMAZE();
  // process command line arguments
  int argIndex = 0;
  if (argc == 1) { Fatal("%d arguments!\n", argc - 1); } // not enough arguments
  argIndex = ProcessOptions(m, argc, argv);
  if (argIndex == argc) { printf("No arguments\n"); }
  // build maze
  if (m->build == true) {
    buildMAZE(m);
  }
  /*if (m->solve == true) {
    solveMAZE(m);
  }*/
  if (m->draw == true) {
    drawMAZE(m);
  }



  freeMAZE(m);
  return 0;
}
