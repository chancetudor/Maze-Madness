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
#include "maze.h"
#include "cell.h"
#include "stack.h"
#include "queue.h"

extern void srandom(unsigned int);
extern long int random(void);
static void printName(void);
static void setInFile(MAZE * m, char * iFile);
static void setOutFile(MAZE * m, char * oFile);

struct cell {
  int row;
  int column;
  bool rightWall;
  bool bottomWall;
  int value;
};

struct maze {
  CELL * (**matrix);
  int rows;
  int columns;
  unsigned int seed;
  char * inFile;
  char * outFile;
  int numDashes;
  int numVertBars;
};

extern MAZE * newMAZE(void) {
  MAZE * m = malloc(sizeof(MAZE));
  assert(m != 0);
  m->rows = 0;
  m->columns = 0;
  m->seed = 1;
  m->inFile = 0;
  m->outFile = 0;
  m->matrix = 0;
  m->numDashes = 0;
  m->numVertBars = 0;

  return m;
}

static void Fatal(char *fmt, ...) {
  va_list ap;

  fprintf(stderr,"An error occured: ");
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  exit(-1);
}

static int ProcessOptions(int argc, char **argv) {
  int start = 0;
  int argIndex = 1;
  int argsUsed = 0;
  char *arg = 0;
  char *iFile = 0; // create char m to hold name of input file
  char *oFile = 0; // create char m to hold name of output file
  int rows = 0;
  int columns = 0;
  unsigned int seed = 0;

  MAZE * m = newMAZE(); // creates new MAZE object

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
        setMazeSeed(m, seed);
        argsUsed = 1;
        break;
      case 'c': // creates new maze
        rows = atoi(arg);
        columns = atoi(argv[argIndex + 1]);
        oFile = argv[argIndex + 2];
        setMazeSize(m, rows, columns);
        setOutFile(m, oFile);
        createMatrix(m);
        argsUsed = 3;
        break;
      //case 's': // solve the maze in file III placing solution in file OOO

      /*case 'd': // draw the created/solved maze in file III
        // number of '----' = (columns * 4) + 1
        setMazeDashes(m, (getMazeColumns(m) * 4) + 1);
        // number of '|' = # of columns given
        setMazeBars(m, getMazeRows(m));
      */
      default:
        fprintf(stderr, "option %s not understood\n", argv[start]);
        exit(-1);
    }

    argIndex += argsUsed;
  }

  return argIndex;
}

static void printName(void) { printf("Written by Chance Tudor\n"); }

extern void setMazeSize(MAZE * m, int r, int c) {
  m->rows = r;
  m->columns = c;
}

extern void setMazeSeed(MAZE * m, int s) { m->seed = s; }

static void setOutFile(MAZE * m, char * file) { m->outFile = file; }

static void setInFile(MAZE * m, char * file) { m->inFile = file; }

extern int getMazeRows(MAZE * m) { return m->rows; }

extern int getMazeColumns(MAZE * m) { return m->columns; }

extern void createMatrix(MAZE * m) {
  // create a one-dimensional array of row pointers
  m->matrix = malloc(sizeof(CELL **) * getMazeRows(m));
  // make all the rows
  for (int i = 0; i < getMazeRows(m); ++i) {
   // create a single row
   m->matrix[i] = malloc(sizeof(CELL *) * getMazeColumns(m));
   // initialize the slots in the row
   for (int j = 0; j < getMazeColumns(m); ++j) {
     m->matrix[i][j] = newCELL();
     // set row and column vals for each new cell
     setLocation(m->matrix[i][j], i, j);
   }
 }
}

int main(int argc, char **argv) {
  int argIndex = 0;
  if (argc == 1) { Fatal("%d arguments!\n", argc - 1); } // not enough arguments
  argIndex = ProcessOptions(argc, argv);
  if (argIndex == argc) { printf("No arguments\n"); }





  return 0;
}
